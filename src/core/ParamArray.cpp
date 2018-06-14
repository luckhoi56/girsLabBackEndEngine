// $Id: ParamArray.cpp 2240 2014-02-23 02:14:31Z Dongfeng Zhu $

#include "ParamArray.h"
#include "NormalDensityOperator.h"
#include "BRASSError.h"
#include "SliceSamplerAids.h"
#include "NetCorrelation.h"
#include <math.h>

/**
 * Constructor.
 * @param len number of variables.
 */
ParamArray::ParamArray(const int len)
{
	this->len = len;
	operators = new NetOperatorPtr[len];
	models = new NetModelRefPtr[len];
	correlations = new NetCorrelationPtr[len];
	params = new float[len];
	ranges = new short[len];

	sampler_aids = new SliceSamplerAid * [len];

	for (int index = 0 ; index < len ; index++) 
	{
		operators[index] = 0;
		models[index] = 0;
		params[index] = 1;
		ranges[index] = RANGE_UNDEFINED;
		sampler_aids[index] = 0;
		correlations[index] = 0;
	}
}

/**
 * Deletes the array and all linked operator objects.
 */
ParamArray::~ParamArray(void)
{
	for (int index = 0 ; index < len ; index++) 
	{
		delete operators[index];
		delete models[index];
		delete sampler_aids[index];
		delete correlations[index];
	}

	delete [] params;
	delete [] operators;
	delete [] models;
	delete [] sampler_aids;
	delete [] correlations;
}

/**
 * Associate a copy of the operator with the specified parameter.
 * Ownership of the operator that is passed in is NOT assumed. 
 * @param index parameter index
 * @param op operator
 */
void ParamArray::addOperator(const int index, const NetOperator & op)
{
	operators[index] = op.copy()->link(operators[index]);
}

/**
 * Marks the variables as correlated.
 * i1 must be smaller than i2.
 */
void ParamArray::addCorrelation(const int i1, const int i2)
{
	NetCorrelationPtr c = new NetCorrelation();
	c->setTarget(i2);
	c->setNext(correlations[i1]);
	correlations[i1] = c;
}

/**
 * Associate a copy of the operator with all applicable parameters.
 * If the operator determines the value of a variable based on the 
 * values of others, the variable will be marked as RANGE_DEPENDENT.
 *
 * This method checks for conflicts, which could occur if
 * the value of a variable is overdefined, i.e., when multiple operators
 * want to decide on the value of the variable. The method returns false
 * if a conflict is detected. The method retunrs true if the operator
 * was succesfully added without a conflict.
 *
 * @param op operator
 */
bool ParamArray::addOperator(const NetOperator & op)
{
	bool conflict = false;

	for (int index = 0 ; index < len ; index++) 
	{
		if (op.appliesTo(index)) 
		{
			operators[index] = op.copy()->link(operators[index]);
		}
	}
	return !conflict;
}

/**
 * Adds a new deterministic model to the network.
 * The models are used to create deterministic relationships 
 * between the variables in the model. 
 * The method creates a copy of the model, and will not 
 * maintain references to the model that is passed in.
 */
bool ParamArray::addModel(NetModel & md) 
{
	NetModelPtr model = md.copy();
	for (int i = 0 ; i < len ; i++) 
	{
		if (model->appliesTo(i)) {
			NetModelRefPtr ref = new NetModelRef();
			ref->setNext(models[i]);
			ref->setModel(model);
			models[i] = ref;
		}
	}
	return true;
}

/**
 * Sets the sampler aid for the specified variable.
 * If an aid was previously specified, the old aid is destructed.
 */
void ParamArray::setAid(const int index, SliceSamplerAid * aid)  
{
	delete sampler_aids[index];
	sampler_aids[index] = aid;
	if (aid != 0) 
		setRange(index,RANGE_VARIABLE);
	else
		setRange(index,RANGE_UNDEFINED);

}

/**
 * Computes the (unnormalized) full conditional density function.
 *
 * The function returns the unnormalized full conditional density for 
 * the variable specified by index. 
 * 
 * First, the list of linked (deterministic) models is traversed to 
 * update the values of dependent variables. Then, the density function
 * is computed by iterating through the list of density operators for
 * the variable.
 *
 * @param index variable index.
 */
double ParamArray::getLnDensity(const int index) 
{
	double ln_like = 0;
	double ln_total = 0;
	int next_var = -1;

	// update the dependent variables
	ln_total = updateLinks(index);

	// iterate through the density operators
	NetOperatorPtr op = operators[index];
	while (op != 0) 
	{
		ln_like = op->compute(params,0);
		ln_total += ln_like;
		op = op->getNext();
	}
	return ln_total;
}

/**
 * Sets the value of the specified variable.
 * This function has no impact on linked or correlated variables.
 */
void ParamArray::setParam(const int index, const float value) 
{
	params[index] = value;
}

/**
 * Sets the value of the specified variable.
 * This function also moves linked or correlated variables.
 */
void ParamArray::updateParam(const int index, const float value)
{
	float distance = value - params[index];
	params[index] = value;
	updateCorrelated(index,distance);
	updateLinks(index);
}

/**
 * Updates the value of the correlated variables.
 *
 * This causes any free variable marked as correlated to be 
 * translated by the specified distance. In addition, links to any
 * translated variable will also be updated.
 */
void ParamArray::updateCorrelated(const int index, const float distance)
{
	NetCorrelationPtr c = correlations[index];
	while (c != 0) 
	{
		int t = c->getTarget();
		if (ranges[t] == this->RANGE_VARIABLE) 
		{
			// Only update the value of the variable if the type is
			// RANGE_VARIABLE; for other types, the value should be
			// calculated instead of updated by this function.
			params[t] += distance;
			updateLinks(t);
		}
		c = c->getNext();
	}
}

/**
 * Recursively updates the values in the array.
 * This method updates the values in the model according to the 
 * deterministic models during the analysis.
 *
 * @param index index of the variable that was last changed
 *
 * @return double 0
 */
double ParamArray::updateLinks(const int index) 
{
	int next_var;
	NetModelRef * md = models[index];
	while (md != 0) 
	{
		md->getModel()->update(params,next_var);
		md = md->getNext();
	}
	return 0;
}

/**
 * Explores the model links to build a list of relevant operators.
 *
 * This function builds a list of operators associated with all 
 * variables that are directly or indirectly affected by the 
 * variable with the specified index.
 * 
 * This function relies on the fact that the model link lists have
 * been reduced, and thus that all affected variables can be found
 * simply by traversing through the model linked list.
 * 
 * The implementation is fairly simple, and requires more computations
 * than absolutely necessary, but should still be fast enough.
 */
void ParamArray::exploreLinks(const int index) 
{
	NetOperatorPtr ops_lst = operators[index];

	// loop through all model links in the list
	NetModelRef * md = models[index];
	while (md != 0) 
	{
		int next_var;
        // determine the next affected/dependent variable
		next_var = md->getModel()->getSpecified();
		// loop through all the operators for the dependent variable
		NetOperatorPtr var_ops = this->operators[next_var];
		while (var_ops != 0) {
			if (!ops_lst->contains(var_ops)) {
				// add a copy of varops to the list
				ops_lst = var_ops->copy()->link(ops_lst);
			}
			var_ops = var_ops->getNext();
		}
		// move to next model link in the list
		md = md->getNext();
	}

	operators[index] = ops_lst;
}

/**
 * Absorbs any operators related to correlated variables.
 *
 * This function causes operators for variables correlated to the 
 * specified variable to be absorbed. This is necessary when 
 * the correlation causes the other variables to be moved along
 * with the specified variable.
 */
void ParamArray::exploreCorrelated(const int index)
{
	NetOperatorPtr ops_lst = operators[index];
	NetCorrelationPtr c = correlations[index];
	while (c != 0) 
	{
		int t = c->getTarget();
		if (this->ranges[t] == RANGE_VARIABLE) 
		{
			NetOperatorPtr var_ops = this->operators[t];
			while (var_ops != 0) 
			{
				if (!ops_lst->contains(var_ops)) {
					// add a copy of varops to the list
					ops_lst = var_ops->copy()->link(ops_lst);
				}
				var_ops = var_ops->getNext();
			}
		}
		c = c->getNext();
	}
	operators[index] = ops_lst;
}

/**
 * Recursively initializes the sampling order in the model.
 * The purpose of this method is to set up the sampling order,
 * and the direction in which deterministic model links are
 * evaluated.
 *
 * The code returns an error code ERR_LOOP_IN_MODEL when a loop
 * is detected in the model. In this case, the links will not be 
 * initialized correctly.
 */
int ParamArray::initLinks(const int index) 
{
	int next_var;
	int retval = 0;
	// recursively call initLinks for models associated with current variable
	// NetOperatorPtr op_copy = 0;
	NetModelRef * md = models[index];
	while (md != 0 && retval == 0) 
	{
		next_var = md->getModel()->init(index);
		
		if (next_var != -1) 
		{
			int cur_range = getRange(next_var);

			if ((cur_range != RANGE_VARIABLE) && (cur_range != RANGE_FIXED))
			{
				retval = BRASSError::ERR_LOOP_IN_MODEL;
			}
			else 
			{
				if (cur_range != RANGE_FIXED)
				{
					short range = getRange(index);
					// temporarily modify range to allow loop detection
					setRange(index,RANGE_DEPENDENT);
					setRange(next_var,RANGE_DEPENDENT);
					retval = initLinks(next_var);
					// restore original range parameter
					setRange(index,range);
				}
			}
		}
		md = md->getNext();
	}
	return retval;
}

/**
 * Creates a reduced list of model links.
 *
 * This method first ensures that a 'reduced' list of model links is 
 * available for the specified variable. The list is a list of model links
 * that are directly or indirectly affected by changes to the specified 
 * variable. After the list is compiled, a simple traversal of the list 
 * updates all variables.
 *
 * @param index variable index.
 * @param reduced_nodes array indicating which nodes were already reduced.
 */
void ParamArray::reduceLinks(const int index, bool * reduced_vars) 
{
	if (!reduced_vars[index]) {

		// the model links for this node were not reduced:
		// explore the model links to build a single list
		// of model references that ensure that each variable
		// is updated only once.

		NetModelRefPtr ref = models[index]; // unreduced model refs
		NetModelRefPtr lst = 0;	// list of reduced model refs

		while (ref != 0) {

			// decouple from the list
			NetModelRefPtr nxt = ref->getNext();
			ref->setNext(0);

			int var = ref->getModel()->getSpecified();

			if (var == index) {
				// self reference: delete
				delete ref;
			} else {

				// recursively reduce the dependent variables
				reduceLinks(var,reduced_vars);

				// attach the model links list of dependent variables
				// to the current reference.
				if (models[var] != 0) {
					ref->setNext(models[var]->copy());
				}

				// merge with the main list of reduced nodes
				NetModelRefPtr ref_iterator = ref;
				NetModelRefPtr acc_iterator = 0;	// last accepted

				while (ref_iterator != 0) {

					if (!lst->contains(ref_iterator->getModel())) {
						// ref was not found: accept by moving acc_iterator
						acc_iterator = ref_iterator;
						// continue with next reference in the list
						ref_iterator = ref_iterator->getNext();
					} else {
						// stop the loop: all further references rejected
						// note that rejection implies that all subsequent
						// references would also be rejected.
						ref_iterator = 0;
					}
				}

				if (acc_iterator != 0) {
					// delete rejected references
					delete acc_iterator->getNext();
					// append prior list to accepted list
					acc_iterator->setNext(lst);
					lst = ref;
				} else {
					// list was fully rejected
					delete ref;
				}
			}

			// move to the next reference in the original list
			ref = nxt;
		}

		// store the reduced list
		models[index] = lst;
		reduced_vars[index] = true;
	}
}

/**
 * Initializes the model.
 *
 * This method should be called just before sampling starts, in order to
 * make the correct sampling order, and reduce the model to the extent 
 * possible.
 * 
 * <b>Explanation of initialization:</b>
 *
 * The initialization first sets the sampling
 * order, and determines which variables should be marked as dependent 
 * (meaning that their value is dependent on the value of other variables in
 * the model). This is done by traversing the model links. A list of such links
 * is maintained for each variable in the models member. For each link, the 
 * direction in which each link is evaluated is determined during this traversal.
 *
 * Secondly, unnecessary links are removed from the model. Links are unnecessary
 * when they point to the same variable from which the link starts (due to the 
 * direction of the link this is possible).
 *
 * Thirdly, a list of density operators (NetOperator) is built for each non-
 * dependent variable, by repeatedly traversing the network (following the 
 * links) according to the link directions determined earlier, and to create
 * copies of all operators encountered. Note that each variable encountered 
 * during the traversal is dependent on the starting variable. A first list
 * is created using exploreLinks(). The list is then reduced to remove 
 * duplicates. The heads of the (single-linked) lists of operators are stored
 * in the operators array.
 *
 * Finally, the operator lists for dependent variables are removed and destroyed.
 * 
 * The code returns an error code if the resulting model is not valid.
 * @see #validate()
 */
int ParamArray::initialize(void)
{
	// initialize the link directions / sampling order
	for (int i = 0 ; i < len ; i++)
	{
		if (this->ranges[i] != RANGE_DEPENDENT)
		{
			// init the model/links
			int retval = initLinks(i);
			// abort in case of invalid link structure
			if (retval != 0) return retval;
		}
	}

	bool * reduced_vars = new bool[len];
	for (int i = 0 ; i < len ; i++) reduced_vars[i] = false;

	// build model link lists for each variable.
	for (int i = 0 ; i < len ; i++)
	{
		reduceLinks(i,reduced_vars);
	}

	delete [] reduced_vars;

    // construct operator lists for non-dependent variables
	for (int i = 0 ; i < len ; i++)
	{
		if (ranges[i] != RANGE_DEPENDENT && ranges[i] != RANGE_FIXED )
		{
			exploreLinks(i);
		}
	}

	for (int i = 0 ; i < len ; i++)
	{
		if (ranges[i] != RANGE_DEPENDENT && ranges[i] != RANGE_FIXED )
		{
			exploreCorrelated(i);
		}
	}

	// clean up operators associated with dependent variables
	for (int i = 0 ; i < len ; i++)
	{
		if (ranges[i] == RANGE_DEPENDENT || ranges[i] == RANGE_FIXED) 
		{
			delete operators[i];
			operators[i] = 0;
		}
	}

	// validate the model
	int retval = validate();

	return retval;
}

/**
 * Dumps a summary of the array to cout.
 * Use for debugging purposes only.
 */
void ParamArray::dumpSummary(void) {

	cout << "VARIABLES : " << this->len << endl;
	
	for (int i  = 0 ; i < len ; i++) {
		cout << i << " (" << ranges[i] << ") ";
		NetOperatorPtr op = operators[i];
		while (op != 0) {
			cout << op->getOperatorID() << " (";
		//	cout << typeid(*op).name() << ") ";
			op = op->getNext();
		}
		cout << endl;
	}

	cout << "DEPENDENCIES" << endl;

	for (int i  = 0 ; i < len ; i++) {
		cout << i << " (" << ranges[i] << ") ";
		NetModelRefPtr ref = models[i];
		while (ref != 0) {
			cout << *(ref->getModel()) << " ";
			ref = ref->getNext();
		}
		cout << endl;
	}
}

/**
 * Stream operator for parameter arrays.
 */
ostream & operator<<(ostream & os, const ParamArray & a)
{
	int cnt = a.size();
	for (int i = 0 ; i < cnt ; i++)
	{
		os << a.getParam(i) << "\t";
	}
    return os;
}

/**
 * Validates the model.
 * Simple implementation that checks:
 *  - some density operator is defined for all free variables
 *  - no cycles are contained in the model.
 * The validation is executed as part of the initialization of 
 * the model.
 */
int ParamArray::validate(void)
{
	// check all free variables have a density model
	for (int i = 0 ; i < this->len ; i++) {
		if (this->ranges[i] != this->RANGE_DEPENDENT && 
			this->ranges[i] != this->RANGE_FIXED) 
		{
			// no density model for free variable
			if (this->operators[i] == 0 /* || 
				!this->operators[i]->isDensity(i) */ ) 
			{
				return BRASSError::ERR_NO_OPERATOR;
			}
		}
	}
	return 0;
}



