//
// File:      $Id: NHInterpretation.cpp 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//


#include "NHInterpretation.h"

#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * Constructor.
 * Assumes ownership of the likelihood function.
 */
NHInterpretation::NHInterpretation(NHLikelihood * likelihood)
{
	this->likelihood = likelihood;
}

/**
 * Destructor.
 * Also destroys likelihood.
 */
NHInterpretation::~NHInterpretation()
{
	delete this->likelihood;
}

/**
 * Returns log of the likelihood for specified parameters.
 * Likelihood is the product of likelihoods of observations
 * added to the interpretation.
 */
double NHInterpretation::getLogLikelihood(const Parameters &params)
{
	double result = 0;

	int size = observations.size();

	if (likelihood != 0 &&  size > 0) {
		NHObservationIterator iterator = observations.begin();
		result = likelihood->getLogLikelihood(params,iterator,size);
	}

	return result;
}

/**
 * Adds a new observation to the interpretation.
 */
void NHInterpretation::addObservation(const NHObservation &observation)
{
	if (likelihood != 0) {
		likelihood->preprocess(observation);
		observations.push_back(observation);
	}
}

/**
 * ...
 */
void NHInterpretation::setLogLikeMode(Parameters * params)
{
	likelihood->setLogLikeMode(params);
}

//////////////////////////////////////////////////////////////////////

/**
 * Returns the total number of events contained in the data.
 * Applies to binomial and poisson type evidence.
 * The default_interpretation is applied to observations with undefined
 * interpretations. Types are defined in NHObservation.h
 */
double NHInterpretation::getTotalEvents(int default_interpretation)
{
	NHObservationIterator it = observations.begin();
	double sum = 0;
	while (it != observations.end()) {
		//int type = (*it).field3;
		//if (type == -1) type = default_interpretation;
		//if (type == 3 || type == 4) sum += (*it).field1;

		sum += (*it).events;

		it++;
	}
	return sum;
}

/**
 * Returns the total amount of exposure (number of events / run time)
 * contained in the data. Applies to binomial and poisson type evidence.
 * The default_interpretation is applied to observations with undefined
 * interpretations. Types are defined in NHObservation.h
 */
double NHInterpretation::getTotalExposure(int default_interpretation)
{
	NHObservationIterator it = observations.begin();
	double sum = 0;
	while (it != observations.end()) {
//		int type = (*it).field3;
//		if (type == -1) type = default_interpretation;
//		if (type == 3 || type == 4) 
		sum += (*it).exposure;
		it++;
	}
	return sum;
}

/**
 * Returns the number of observations.
 * The default_interpretation is applied to observations with undefined
 * interpretations. Types are defined in NHObservation.h
 */
int NHInterpretation::getObservationCount()
{
	return observations.size();
}

/**
 * Returns the mean of log(estimate_i) (not weighted).
 * The default_interpretation is applied to observations with undefined
 * interpretations. Types are defined in NHObservation.h
 * NOT TESTED!!!
 */
double NHInterpretation::getEstimateMean(int default_interpretation)
{
	NHObservationIterator it = observations.begin();
	int cnt = 0;
	double sum = 0;
	while (it != observations.end()) {
//		int type = (*it).field3;
//		if (type == -1) type = default_interpretation;
//		if (type == 2) {
//			sum += log((*it).field1);
			sum += log((*it).median);
			cnt ++;
//		}
		it++;
	}
	return (cnt > 0) ? sum / cnt : 0; 
}

/**
 * Returns the sdev of log(estimate_i)
 * The default_interpretation is applied to observations with undefined
 * interpretations. Types are defined in NHObservation.h
 * NOT TESTED!!!
 */
double NHInterpretation::getEstimateSDev(int default_interpretation)
{
	NHObservationIterator it = observations.begin();
	int cnt = 0;
	double sum = 0;
	while (it != observations.end()) {
//		int type = (*it).field3;
//		if (type == -1) type = default_interpretation;
//		if (type == 2) {
			sum += pow(log((*it).median),2);
			cnt ++;
//		}
		it++;
	}
	
	if (cnt > 1) {
		sum -= pow(getEstimateMean(default_interpretation),2);
		double var = sum / (cnt - 1);
		return sqrt(var);
	} else return 0;
}
