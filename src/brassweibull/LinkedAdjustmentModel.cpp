// $Id: LinkedAdjustmentModel.cpp 169 2005-01-28 14:17:28Z saulo $

#include "linkedadjustmentmodel.h"
#include "WeibullModelLayout.h"

/**
 * Constructor.
 */
LinkedAdjustmentModel::LinkedAdjustmentModel(const int node0, const int node1, const int node_cnt, const int mode, const int src_mode)
{
	WeibullModelLayout wml;

	dst_alpha = wml.getScaleIndex(node0,mode,node_cnt);
	dst_beta = wml.getShapeIndex(mode,node_cnt);
	dst_gamma = wml.getScaleIndex(node1,mode,node_cnt);

	src_alpha = wml.getScaleIndex(node0,src_mode,node_cnt);
	src_beta = wml.getShapeIndex(src_mode,node_cnt);
	src_gamma = wml.getScaleIndex(node1,src_mode,node_cnt);

	direction = this->DIR_UNDECIDED;
}

LinkedAdjustmentModel::~LinkedAdjustmentModel(void)
{
}

/**
 * Returns true if the model has the variable as its dependent variable.
 */
bool LinkedAdjustmentModel::specifies(const int var_index) const 
{
	return 
		(direction == DIR_A_TO_E && var_index == dst_gamma) || 
		(direction == DIR_E_TO_A && var_index == dst_alpha);
}

/**
 * Returns the index of the variable specified by this model.
 */
int LinkedAdjustmentModel::getSpecified(void) const 
{
	return direction == DIR_A_TO_E ? dst_gamma : dst_alpha;
}

/**
 * Initializes the model.
 *
 * Initialization is used to decide the direction of the model 
 * link. If the direction can not be decided based on the input
 * variable, or has already been decided, the function returns
 * -1. Otherwise, the model returns the index of the variable
 * that was made into a dependent variable.
 */
int LinkedAdjustmentModel::init(const int var_index)
{
	if (direction == DIR_UNDECIDED) {
		if (var_index == dst_alpha) {
			//cout << alpha << " -> " << gamma << endl;
			direction = DIR_A_TO_E;
			return dst_gamma;
		} else if (var_index == dst_gamma) {
			//cout << gamma << " -> " << alpha << endl;
			direction = DIR_E_TO_A;
			return dst_alpha;
		} else {
			return -1;
		}
	} else return -1;
}

/**
 * Prints the model to the stream.
 */
void LinkedAdjustmentModel::print(ostream & os) const 
{
	if (direction == DIR_A_TO_E) {
		os << "(" << dst_alpha << " -> " << dst_gamma << ")";
	} else if (direction == DIR_E_TO_A) {
		os << "(" << dst_gamma << " -> " << dst_alpha << ")";
	} else {
		os << "(?? -> ??)";
	}
}

///////////////////////////////////////////////////////////////

/**
 * Updates the values in the parameter array.
 * @param params array of parameter values.
 * @param next_var index of variable modified by this method.
 */
double LinkedRateAdjustmentModel::update(float * params, int & next_var) 
{
	if (direction == DIR_UNDECIDED) return -1;

	float lnz = params[src_beta] * (params[src_alpha] - params[src_gamma]);

	if (direction == DIR_A_TO_E) {
		float lna = params[dst_alpha];
		float b = params[dst_beta];
		float lng = lna - lnz / b;
		params[dst_gamma] = lng;
		next_var = dst_gamma;
	} else {
		float lng = params[dst_gamma];
		float b = params[dst_beta];
		float lna = lng + lnz / b;
		params[dst_alpha] = lna;
		next_var = dst_alpha;
	}
	return 0;
}

/**
 * Returns true if the model/link is triggered by the variable.
 * @param var_index variable index.
 */ 
bool LinkedRateAdjustmentModel::appliesTo(const int var_index) const 
{
	return 	var_index == dst_gamma || var_index == dst_alpha || var_index == dst_beta || 
		var_index == src_alpha || var_index == src_beta || var_index == src_gamma ;
}

/**
 * Returns a copy of the model.
 */
NetModel * LinkedRateAdjustmentModel::copy() const {
	return new LinkedRateAdjustmentModel(*this);
}


//////////////////////////////////////////////////////////////////////

/**
 * Updates the values in the parameter array.
 * @param params array of parameter values.
 * @param next_var index of variable modified by this method.
 */
double LinkedTimeAdjustmentModel::update(float * params, int & next_var) 
{
	if (direction == DIR_UNDECIDED) return -1;

	float lnz = (params[src_alpha] - params[src_gamma]);

	if (direction == DIR_A_TO_E) {
		float lna = params[dst_alpha];
		float lng = lna - lnz;
		params[dst_gamma] = lng;
		next_var = dst_gamma;
	} else {
		float lng = params[dst_gamma];
		float lna = lng + lnz;
		params[dst_alpha] = lna;
		next_var = dst_alpha;
	}
	return 0;
}

/**
 * Returns true if the model/link is triggered by the variable.
 * @param var_index variable index.
 */ 
bool LinkedTimeAdjustmentModel::appliesTo(const int var_index) const 
{
	return 	var_index == dst_gamma || var_index == dst_alpha ||  
		var_index == src_alpha || var_index == src_gamma ;
}

/**
 * Returns a copy of the model.
 */
NetModel * LinkedTimeAdjustmentModel::copy() const {
	return new LinkedTimeAdjustmentModel(*this);
}
