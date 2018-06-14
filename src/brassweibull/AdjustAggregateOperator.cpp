// $Id: AdjustAggregateOperator.cpp 352 2005-03-11 17:08:23Z saulo $

#include "adjustaggregateoperator.h"

#include <math.h>

/**
 * Constructor <B>DO NOT USE</B>.
 * If a_node index is greater than e_node, a_node and e_node are swapped.
 * @param a_node int index of alpha node
 * @param e_node int index of eta node
 * @param median double median correction factor.
 * @param errfac double correction factor error factor.
 * @param node_cnt int number of nodes in the model
 */
AdjustAggregateOperator::AdjustAggregateOperator(int a_node,int e_node,double median,double errfac, int mode_cnt, int node_cnt)
{
	this->a_node = a_node;
	this->e_node = e_node;
	this->mean = log(median);
	this->sdev = log(errfac) / 1.645;
	this->mode_cnt = mode_cnt;
	this->node_cnt = node_cnt;
}

AdjustAggregateOperator::~AdjustAggregateOperator(void)
{
}

/**
 * Returns true if the operator applies to the specified variable.
 * @param index int index of the variable
 */
bool AdjustAggregateOperator::appliesTo(const int index) const
{
	int node = (index % (node_cnt + 1)) - 1;
	// only first scale of e-node is left undetermined.
	return node == -1 || node == a_node || index == getScaleIndex(e_node,1);
}

/**
 * Returns true if the operator determines the value of the specified variable.
 * @param index int index of the variable.
 */
bool AdjustAggregateOperator::specifies(const int index) const
{
	int node = (index % (node_cnt + 1)) - 1;
	// only first scale of e-node is left undetermined.
	return node == e_node && index != getScaleIndex(e_node,1);
}

/**
 * Absorbs the operators of dependent variables into the operator list.
 * @param operator_arr NetOperator * array of operator lists
 * @param index int index of current variable.
 */
NetOperatorPtr AdjustAggregateOperator::absorb(NetOperatorPtr * operator_arr, int index)
{
	if (next != 0) next = next->absorb(operator_arr,index);

	// absorb operators for all variables that this operator specifies
	int var_cnt = (node_cnt + 1) * mode_cnt;
	for (int i = index + 1 ; i < var_cnt ; i++) 
	{ 
		if (this->specifies(i)) 
		{
			NetOperatorPtr op = operator_arr[i];
			while (op != 0) 
			{
				NetOperatorPtr cpy = op->copy();
				next = cpy->link(next);
				op = op->getNext();
			}
		}
	}
	return this;
}

/**
 * Returns the sorting order of this operator.
 * The sorting order is the index of the highest order variable
 * that depends on this operator.
 *
 * The implementation method depends on the specifics of the WeibullModelLayout class.
 * @see WeibullModelLayout
 */
int AdjustAggregateOperator::getOrder() const
{
	// if only 1 mode, degrades to a proportional model
	if (mode_cnt == 1) return this->ORDER_LAST;
	return getScaleIndex(e_node,2);
}

/**
 * Computes the operator.
 * This operator performs two tasks. First it updates dependent 
 * scale parameters, which are scale parameters for the eta node
 * (failure mode 2 and up). It then computes and returns the 
 * density based on the realized z factor. The z factor is determined
 * based on the first failure mode, and then forced on the other 
 * failure modes.
 */
double AggregateRateOperator::compute(float * params, float * derivs) const
{
	int a = getScaleIndex(a_node,1);
	int e = getScaleIndex(e_node,1);
	int b = getShapeIndex(1);
	// determine value for lnz
	float lnz = (params[a] - params[e]) * params[b];
	// update dependent variables
	for (int mode = 2 ; mode <= mode_cnt ; mode++) 
	{
		int am = getScaleIndex(a_node,mode);
		int em = getScaleIndex(e_node,mode);
		int bm = getShapeIndex(mode);
		params[em] = params[am] - lnz / params[bm];
	}
	double f = -0.5 * pow((lnz - mean) / sdev,2) + log(params[b]);
	return f;
}

NetOperatorPtr AggregateRateOperator::copy() const {
	AggregateRateOperator * op = new AggregateRateOperator(*this);
	op->operator_id = operator_id;
	op->next = 0;
	return op;
}

/**
 * Computes the operator.
 * This operator performs two tasks. First it updates dependent 
 * scale parameters, which are scale parameters for the eta node
 * (failure mode 2 and up). It then computes and returns the 
 * density based on the realized z factor. The z factor is determined
 * based on the first failure mode, and then forced on the other 
 * failure modes.
 */
double AggregateTimeOperator::compute(float * params, float * derivs) const
{
	int a = getScaleIndex(a_node,1);
	int e = getScaleIndex(e_node,1);
	float lnz = params[a] - params[e];
	// update dependent variables
	for (int mode = 2 ; mode <= mode_cnt ; mode++) 
	{
		int am = getScaleIndex(a_node,mode);
		int em = getScaleIndex(e_node,mode);
		params[em] = params[am] - lnz;
	}
	// compute density for value of lnz
	double f = -0.5 * pow((lnz - mean) / sdev,2);
	return f;
}

NetOperatorPtr AggregateTimeOperator::copy() const {
	AggregateTimeOperator * op = new AggregateTimeOperator(*this);
	op->operator_id = operator_id;
	op->next = 0;
	return op;
}