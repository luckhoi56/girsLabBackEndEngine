// TabularPoissonDistribution.cpp: implementation of the TabularPoissonDistribution class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TabularPoissonDistribution.h"
#include "GammaDistribution.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * Constructs a new TabularPoisson distribution.
 * The newly created class takes ownership of arrays as well as likelihood function
 */
TabularPoissonDistribution::TabularPoissonDistribution(double * vals, double * cdfs, int length, double k, double t) :
	UpdatedTabularDistribution(vals,cdfs,length,new GammaDistribution(1 / t,k + 1))
{
	// no action
}

TabularPoissonDistribution::~TabularPoissonDistribution()
{
	// no action
}

