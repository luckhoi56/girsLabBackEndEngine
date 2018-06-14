// $Id: WeightedLikeOperator.cpp 275 2005-03-01 03:53:40Z fgroen $

#include "WeightedLikeOperator.h"

/**
 * Creates a Weighted Likelihood operator.
 * The operator applies the geometric weighting to the specified operator.
 */
WeightedLikeOperator::WeightedLikeOperator(const NetOperatorPtr op, const double weight)
{
	oper = op->copy();
	w = weight;
}

WeightedLikeOperator::~WeightedLikeOperator(void)
{
	delete oper;
}

/**
 * Computes the density.
 * The method applies the geometric weighting of the weighted likelihood
 * method to the operator.
 */
double WeightedLikeOperator::compute(float * params, float * derivs) const
{
	return w * oper->compute(params,derivs);
}

/**
 * Returns an unlinked copy of the operator.
 * A copy of the contained operator is automatically created.
 * @see NetOperator#copy(void)
 */
NetOperatorPtr WeightedLikeOperator::copy(void) const
{
	WeightedLikeOperator * op = new WeightedLikeOperator(oper,w);
	op->operator_id = this->operator_id;
	op->next = 0;
	return op;
}

bool WeightedLikeOperator::isDensity(const int index) const
{
	if (oper != 0 && oper->isDensity(index)) return true;
	if (next != 0) return next->isDensity(index);
	return false;
}
