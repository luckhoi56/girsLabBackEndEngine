//
// File:      $Id: NHLikelihoodBetaBinomial.cpp 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//


#include "NHLogBinomialSupport.h"
#include "NHLikelihoodBetaBinomial.h"
#include "../dcdf/DCDFLIBInterface.h"

#include <math.h>
#include <iostream>

NHLikelihoodBetaBinomial::NHLikelihoodBetaBinomial()
{
		
}

NHLikelihoodBetaBinomial::~NHLikelihoodBetaBinomial()
{

}

/**
 * Returns log of the likelihood.
 * This computes the likelihood for a list of observations.
 * Each observation falls into one of three categories.
 *  - demand based data plus estimate
 *  - demand based data only
 *  - estimate only
 * If neither demand based data nor estimate is available
 * for an observation, observation is effectively skipped.
 * If problems are encountered, 
 * @param params       contains Beta distribution parameters
 * @param it           observation list iterator
 * @param observations number of observations
 */
double NHLikelihoodBetaBinomial::getLogLikelihood(const Parameters &params, NHObservationIterator &it, int observations)
{
	int empty_observations = 0;

	double alpha = params.getValue(0);
 	double beta = params.getValue(1);
	double ln_like = 0;
	double k,n;
	double LOG_ZERO = -1e300;
	NHLogBinomialSupport integrator;
	bool fail = false;
	for (int i = 0 ; i < observations && !fail; i++, it++) {
		k = (*it).events;
		n = (*it).exposure;
		if ((*it).sigma > 0 && (*it).median > 0) {
			// use integrator
			double nu = (*it).median > 0 ? log((*it).median) : (*it).median;
			double tau = (*it).sigma;
			ln_like += integrator.integrate(nu,tau,alpha + k,beta + n - k,fail);

		} else if (n > 0) {
			// conjugate case: demand based only
			ln_like += 	DCDFLIBInterface::computeLnGamma(alpha + k) + 
						DCDFLIBInterface::computeLnGamma(beta + n - k) -
						DCDFLIBInterface::computeLnGamma(alpha + beta + n);	
		} else {
			empty_observations++;
		}
	}

	if (!fail) {
		ln_like += (observations - empty_observations) * (
			DCDFLIBInterface::computeLnGamma(alpha + beta) - 
			DCDFLIBInterface::computeLnGamma(alpha) -
			DCDFLIBInterface::computeLnGamma(beta)
		);
	} else {
		ln_like = LOG_ZERO;
	}

	return ln_like;
}	

/// Does nothing.
void NHLikelihoodBetaBinomial::preprocess(const NHObservation &observation)
{
}
