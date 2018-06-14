// UpdatedTabularDistribution.cpp: implementation of the UpdatedTabularDistribution class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UpdatedTabularDistribution.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UpdatedTabularDistribution::UpdatedTabularDistribution(double * vals, 
		double * cdfs, int length, ParametricDistribution * likelihood)
{
	valArray = vals;
	cdfArray = cdfs;
	size = length;
	upperBound = valArray[size - 1];
	lowerBound = valArray[0];
	like = likelihood;

	integral = 0;
	for (int i = 1 ; i < size ; i++) {
		integral += (cdfArray[i] - cdfArray[i - 1]) * 
			(like->getCDF(valArray[i]) - like->getCDF(valArray[i - 1])) / 
			(valArray[i] - valArray[i - 1]);
	}
}

UpdatedTabularDistribution::~UpdatedTabularDistribution()
{
	delete [] valArray;
	delete [] cdfArray;
	delete like;
}

double UpdatedTabularDistribution::getCDF(double x) const
{
	if (x <= lowerBound) return 0;
	if (x >= upperBound) return 1;

	double cdf = 0;
	int i = 1;

	while (x > valArray[i]) {
		cdf += (cdfArray[i] - cdfArray[i - 1]) * 
			(like->getCDF(valArray[i]) - like->getCDF(valArray[i - 1])) / 
			(valArray[i] - valArray[i - 1]);
		i++;
	}

	cdf += (cdfArray[i] - cdfArray[i - 1]) *  
		(like->getCDF(x) - like->getCDF(valArray[i - 1])) / 
		(valArray[i] - valArray[i - 1]);

	cdf /= integral;

	return cdf;
}

double UpdatedTabularDistribution::getQuantile(double z) const
{
	if (z <= 0.5 / size) return 0;
	if (z >= 1 - 0.5 / size) return 1;
	// todo : implement function
	return -1;
}