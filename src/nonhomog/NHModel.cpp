//
// File:      $Id: NHModel.cpp 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//

#include "stdafx.h"
#include "NHModel.h"
#include "Distribution.h"

#include <math.h>

#include <iostream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NHModel::NHModel()
{

}

NHModel::~NHModel()
{

}

void NHModel::setSampleSet(SampleSetPtr sampleset)
{
	this->samples = sampleset;
}


double NHModel::findPercentile(double percent, double epsilon) {

	Distribution distribution;
	double lower, upper,value;
	computePercentile(percent,distribution,0);
	lower = distribution.getPercentile(0.01);
	upper = distribution.getPercentile(0.99);
	findPercentile(value,lower,upper,percent,epsilon);
	return value;
}

/**
 * Finds the percentile within the given range. If the range does not include the percentile,
 * the lower or upper specified bound is returned.
 */
double NHModel::findPercentile(double &value, double lower, double higher, double percent, double epsilon)
{
	Distribution distribution;
	double midval, lo, hi, mid;

	int iter = 0;
	
	lo = lower;
	computeMeasure(0,lo,NHMEASURE_POPVAR_CDF,distribution,0);
	double loval = distribution.getMean();

	hi = higher;
	computeMeasure(0,hi,NHMEASURE_POPVAR_CDF,distribution,0);
	double hival = distribution.getMean();

	mid = estimatePercentile(lo,loval,hi,hival,percent);
	computeMeasure(0,mid,NHMEASURE_POPVAR_CDF,distribution,0);
	midval = distribution.getMean();

	double maxval, minval;

	// decide on a tolerance interval for the CDF value
	if (percent < 0.5) {
		maxval = percent * epsilon;
		minval = percent / epsilon;
	} else {
		maxval = 1 - (1 - percent) / epsilon;
		minval = 1 - (1 - percent) * epsilon;
	}

	while ((midval > maxval || midval < minval) && iter < 100) {

		if (midval > percent) {
			hi = mid;
			hival = midval;
		} else {
			lo = mid;
			loval = midval;
		}

		mid = estimatePercentile(lo,loval,hi,hival,percent);
		computeMeasure(0,mid,NHMEASURE_POPVAR_CDF,distribution,0);
		midval = distribution.getMean();

		iter++;
	}
	value = mid;

//	cout << "NH MODEL located " << percent << " percentile (" << epsilon << ") at " << mid << " -> " << midval << " in " << iter << " iterations." << endl;

	return value;
}

/**
 * Computes the distribution over the given percentile of the distribution.
 */
int NHModel::computePercentile(double percentile, Distribution &dist, StatusPtr status)
{
	double prob,x;
	if (samples->size() == 0) {
		dist.clear();
		return -1;
	}
	dist.clear(samples->size());	
	samples->first();
	while (!samples->endOfSamples()) {
		Parameters params = samples->getParameters();
		x = computePercentile(params,percentile);
		prob = samples->getProbability();
		dist.add(x,prob);
		samples->next();
	}
	return 0;
}


/**
 * Computes the distribution over the mean of the distribution.
 */
int NHModel::computeMean(Distribution &dist, StatusPtr status)
{
	double prob,x;
	if (samples->size() == 0) {
		dist.clear();
		return -1;
	}

	dist.clear(samples->size());	
	samples->first();
	while (!samples->endOfSamples()) {
		Parameters params = samples->getParameters();
		x = computeMean(params);
		prob = samples->getProbability();
		dist.add(x,prob);
		samples->next();
	}
	return 0;
}


/**
 * Computes the distribution over the variance of the distribution.
 * Note: the mean of the variances corresponds to the 
 * variance of the mean distribution.
 */
int NHModel::computeVariance(Distribution &dist, StatusPtr status)
{
	double prob,x;
	if (samples->size() == 0) {
		dist.clear();
		return -1;
	}
	dist.clear(samples->size());	
	samples->first();
	while (!samples->endOfSamples()) {
		Parameters params = samples->getParameters();
		x = computeVariance(params);
		prob = samples->getProbability();
		dist.add(x,prob);
		samples->next();
	}
	return 0;
}

int NHModel::computeBounds(int parameter, double & lower, double & upper)
{
	lower = findPercentile(0.0005,1.1);
	upper = findPercentile(0.9995,1.1);
	return 0;
}
