// $Id: WeightedLikeOperator.h 352 2005-03-11 17:08:23Z saulo $

#pragma once
#include "netoperator.h"

/**
 * Operator that implements weighted likelihood weighting on an operator.
 *
 * The operation can be appleid to any other operator.
 */
class WeightedLikeOperator : public NetOperator
{
public:
	WeightedLikeOperator(const NetOperatorPtr op, const double weight);
	~WeightedLikeOperator(void);
	double compute(float * params, float * derivs) const;
	NetOperatorPtr copy(void) const;
	virtual bool appliesTo(const int index) const 
		{return oper->appliesTo(index);}
	virtual bool specifies(const int index) const
		{return false;}
	bool isDensity(const int index) const;

private:
	NetOperatorPtr oper;	// operator
	double w;				// weight factor
};
