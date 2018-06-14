// TabularIntegrator.cpp: implementation of the TabularIntegrator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TabularIntegrator.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TabularIntegrator::TabularIntegrator()
{

}

TabularIntegrator::~TabularIntegrator()
{

}

/**
 * Returns the natural log of the normalization factor determined by the 
 * integrator. This is a fallback method.
 */
double TabularIntegrator::getLnNorm() {
	double p_tmp,lnnorm;
	computeLogIntegrant(1, p_tmp, lnnorm);
	return lnnorm;
}
