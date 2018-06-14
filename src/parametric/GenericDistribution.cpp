// $Header: C:/cvsrepo/ptdev/native/redcas/parametric/GenericDistribution.cpp,v 1.2 2001/08/23 16:16:07 fgroen Exp $

// GenericDistribution.cpp: implementation of the GenericDistribution class.
//
//////////////////////////////////////////////////////////////////////

#include "GenericDistribution.h"
#include <stdlib.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

double GenericDistribution::getMedian() const
{
	return getQuantile(0.50);
}

double GenericDistribution::getSample() const
{
	double x = rand() * 1.0 / RAND_MAX;
	return getQuantile(x);
}

void GenericDistribution::fillSampleArray(double *values, int size) const
{
	for (int i = 0 ; i < size ; i++) values[i] = getSample();
}

double GenericDistribution::getPDF(double x) const
{
	return exp(getLogPDF(x));
}
