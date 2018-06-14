// Function.cpp: implementation of the Function class.
//
//////////////////////////////////////////////////////////////////////

/****************************************************
 *
 *  Bayesian Weibull Sampler.
 *
 *	(c) 2000, Frank Groen. All Rights Reserved.
 *
 ****************************************************/


#include "Function.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Function::Function()
{
	// DO NOTHING
}

Function::~Function()
{
	// DO NOTHING
}

double Function::evaluate(const Parameters & params) const 
{
	return 0;
}

/**
 * Evaluate for one dimensional functions.
 */
double Function::evaluate(double x) const 
{
	return evaluate(Parameters(x));
}
