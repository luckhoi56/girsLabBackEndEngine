// $Header: C:/cvsrepo/ptdev/native/redcas/parametric/ParametricDistribution.cpp,v 1.2 2001/08/23 16:16:07 fgroen Exp $

// ParametricDistribution.cpp: implementation of the ParametricDistribution class.
//
//////////////////////////////////////////////////////////////////////

#include "ParametricDistribution.h"
#include <stdlib.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

double ParametricDistribution::getMedian() const
{
	return getQuantile(0.50);
}

double ParametricDistribution::getSample() const
{
	double x = rand() * 1.0 / RAND_MAX;
	return getQuantile(x);
}

void ParametricDistribution::fillSampleArray(double *values, int size) const
{
	for (int i = 0 ; i < size ; i++) values[i] = getSample();
}

double ParametricDistribution::getPDF(double x) const
{
	return exp(getLogPDF(x));
}
