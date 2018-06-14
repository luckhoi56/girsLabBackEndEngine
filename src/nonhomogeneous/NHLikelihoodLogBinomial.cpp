//
// File:      $Id: NHLikelihoodLogBinomial.cpp 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//

#include "NHLikelihoodLogBinomial.h"
#include "NHLogBinomialSupport.h"

#include <math.h>


NHLikelihoodLogBinomial::NHLikelihoodLogBinomial(void)
{
}

NHLikelihoodLogBinomial::~NHLikelihoodLogBinomial(void)
{
}

/// Does nothing.
void NHLikelihoodLogBinomial::preprocess(const NHObservation &observation)
{
	// DO NOTHING
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
double NHLikelihoodLogBinomial::getLogLikelihood(const Parameters & params, 
												 NHObservationIterator & it, 
												 int observations)
{
	double LOG_ZERO = -1e300;

	double nu = log(params.getValue(0));
 	double tau  = params.getValue(1);

	double ln_like = 0;
	double k,n,mu,sigma,nu0,tau0,tau_sq,sigma_sq,ln_integral,ln_constant;

	NHLogBinomialSupport integrator;

	bool fail = false;

	for (int i = 0 ; i < observations && !fail; i++, ++it) {
	
		// runtime data
		k = (*it).events;
		n = (*it).exposure;

		// estimate data
		mu = (*it).median > 0 ? log((*it).median) : (*it).median;
		sigma = (*it).sigma;

		tau_sq = pow(tau,2);
		sigma_sq = pow(sigma,2);

		if (n > 0 && sigma > 0) {

			// estimate and runtime: use integrator
			nu0 = (mu / sigma_sq + nu / tau_sq) / (1 / sigma_sq + 1 / tau_sq);
			tau0 = pow(1 / sigma_sq + 1 / tau_sq , -0.5);
			double ln_cdf1 = 0;
			ln_integral = integrator.integrate(nu0,tau0,k,n-k+1,fail) - ln_cdf1;
			ln_constant = -0.5 * pow(mu - nu,2) / (sigma_sq + tau_sq) - log(tau);
			ln_like += ln_integral + ln_constant;

		} else if (n > 0) {

			// Demand data only.
			// Disabled truncation for now.
			double ln_cdf1 = 0;
			ln_like += integrator.integrate(nu,tau,k,n-k+1,fail) - log(tau) - ln_cdf1;

		} else if (sigma > 0) {

			// estimate only: special case
			ln_like += -(pow(mu,2) + pow(nu,2) 
				+ 2 * mu * (-nu + sigma_sq + tau_sq)) / 2 / (sigma_sq + tau_sq)
				- log(tau) - 0.5 * log(1 / sigma_sq + 1 / tau_sq);
		}
	}

	if (fail) {
		ln_like = LOG_ZERO;
	}

	return ln_like;
}

