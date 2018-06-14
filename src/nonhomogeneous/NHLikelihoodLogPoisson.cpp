//
// File:      $Id: NHLikelihoodLogPoisson.cpp 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//


#include "NHLikelihoodLogPoisson.h"

#include <iostream>

NHLikelihoodLogPoisson::NHLikelihoodLogPoisson()
{

}

NHLikelihoodLogPoisson::~NHLikelihoodLogPoisson()
{

}

/**
 * Does nothing.
 */
void NHLikelihoodLogPoisson::preprocess(const NHObservation &observation)
{
	// DO NOTHING
}

/**
 * Returns the natural log of the likelihood function.
 */
double NHLikelihoodLogPoisson::getLogLikelihood(const Parameters &params, NHObservationIterator &it, int observations)
{

	double LOG_ZERO = -1e300;

	double nu = log(params.getValue(0));
 	double tau  = params.getValue(1);

	double ln_like = 0;
	double k,t,mu,sigma,nu0,tau0,tau_sq,sigma_sq,ln_integral,ln_constant;

	NHLogPoissonSupport integrator;

	integrator.bounds_cycles = 0;
	integrator.integr_cycles = 0;

	bool fail = false;

	for (int i = 0 ; i < observations && !fail; i++, it++) {
	
		// runtime data
		k = (*it).events;
		t = (*it).exposure;

		// estimate data
		mu = (*it).median > 0 ? log((*it).median) : (*it).median;
		sigma = (*it).sigma;

		tau_sq = pow(tau,2);
		sigma_sq = pow(sigma,2);

		if (t > 0 && sigma > 0) {

			// estimate and runtime: use integrator
			nu0 = (mu / sigma_sq + nu / tau_sq) / (1 / sigma_sq + 1 / tau_sq);
			tau0 = pow(1 / sigma_sq + 1 / tau_sq , -0.5);
			ln_integral = integrator.integrate(nu0,tau0,k,t,fail);
			ln_constant = -0.5 * pow(mu - nu,2) / (sigma_sq + tau_sq) - log(tau);
			ln_like += ln_integral + ln_constant;

		} else if (t > 0) {

			// runtime data only: use integrator
			ln_like += integrator.integrate(nu,tau,k,t,fail) - log(tau);

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

	//cout << "NH-LOGPOIS AT (" << exp(nu) << " , " << exp(1.645*tau) << ") CYCLES (" << integrator.bounds_cycles << " , " << integrator.integr_cycles << ") LIKE " << ln_like << endl;

	return ln_like;
}

