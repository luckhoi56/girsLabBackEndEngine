// MinusFunction.cpp: implementation of the MinusFunction class.
//
//////////////////////////////////////////////////////////////////////

/****************************************************
 *
 *  Bayesian Weibull Sampler.
 *
 *	(c) 2000, Frank Groen. All Rights Reserved.
 *
 ****************************************************/


#include "MinusFunction.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MinusFunction::MinusFunction(Function & function)
{
	_function = &function;
}

MinusFunction::~MinusFunction()
{

}

int MinusFunction::getDimension() const
{
	if (_function != 0L) {
		return _function->getDimension();
	} else {
		return 0;
	}
}

double MinusFunction::evaluate(const Parameters & parameters) const
{
	if (_function != 0L) {
		return -(_function->evaluate(parameters));
	} else {
		return 0;
	}
}
