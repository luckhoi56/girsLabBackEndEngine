//
// File:      $Id: NHLikelihoodGammaPoisson.cpp 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//

#include "stdafx.h"
#include "NHLikelihoodGammaPoisson.h"
#include "DCDFLIBInterface.h"
#include <iostream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NHLikelihoodGammaPoisson::NHLikelihoodGammaPoisson()
{
//	cout << "Constructing NHLikelihoodGammaPoisson" << endl;
}

NHLikelihoodGammaPoisson::~NHLikelihoodGammaPoisson()
{

}

/// Does nothing.
void NHLikelihoodGammaPoisson::preprocess(const NHObservation &observation)
{
	// DO NOTHING
}

/**
 * Returns the natural log of the likelihood function.
 * The likelihood function is the non-homogeneous gamma-poisson-lognormal
 * model. Each observation contains either or both poisson data or 
 * lognormal (estimate) data.
 */
double NHLikelihoodGammaPoisson::getLogLikelihood(const Parameters &params, NHObservationIterator &it, int observations)
{
	int empty_observations = 0;

	double alpha = params.getValue(0);
 	double theta = params.getValue(1);

	double ln_like = 0;
	double k,t;

	double LOG_ZERO = -1e300;

	NHLogPoissonSupport integrator;

	bool fail = false;

	for (int i = 0 ; i < observations && !fail; i++, ++it) {
	
		k = (*it).events;
		t = (*it).exposure;

		if ((*it).sigma > 0 && (*it).median > 0) {
			// use integrator
			double nu = (*it).median > 0 ? log((*it).median) : (*it).median;
			double tau = (*it).sigma;
			ln_like += integrator.integrate(nu,tau,alpha + k,theta + t,fail);
		} else if (t > 0) {
			// special case
			ln_like += DCDFLIBInterface::computeLnGamma(alpha + k) - (alpha + k) * log(theta + t);
		} else {
			empty_observations++;
		}
	}	

	if (!fail) {
		// multiply by (b^a / Gamma[a])^n
		ln_like += (observations - empty_observations) * 
			(alpha * log(theta) - DCDFLIBInterface::computeLnGamma(alpha));
	} else {
		ln_like = LOG_ZERO;
	}

	return ln_like;
}