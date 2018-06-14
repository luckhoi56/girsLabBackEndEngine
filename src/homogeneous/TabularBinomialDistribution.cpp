// TabularBinomialDistribution.cpp: implementation of the TabularBinomialDistribution class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TabularBinomialDistribution.h"
#include "BetaDistribution.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * Constructs a new TabularBinomial distribution.
 * The newly created class takes ownership of arrays as well as likelihood function
 */
TabularBinomialDistribution::TabularBinomialDistribution(double * vals, double * cdfs, int length, double k, double n) :
	UpdatedTabularDistribution(vals,cdfs,length,new BetaDistribution(k + 1,n - k + 1))
{

}

TabularBinomialDistribution::~TabularBinomialDistribution()
{

}
