// ThreeDeeFunction.cpp: implementation of the ThreeDeeFunction class.
//
//////////////////////////////////////////////////////////////////////

#include "ThreeDeeFunction.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ThreeDeeFunction::ThreeDeeFunction()
{

}

ThreeDeeFunction::~ThreeDeeFunction()
{

}

double ThreeDeeFunction::evaluate(const Parameters & params) const
{
	double a = params.getValue(0);
	double b = params.getValue(1);
	double l = params.getValue(2);
	Parameters ab(a,b);
	double retval = nonhomogeneous->getLogLikelihood(ab) + 
		log(DCDFLIBInterface::computeGammaPDF(a,b,l));
	if (homogeneous) retval += homogeneous->getLogPDF(l);
	return retval;
}