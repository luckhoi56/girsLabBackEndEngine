//
// File:      $Id: NHModelLogBinomial.cpp 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//
 

#include "nhmodellogbinomial.h"

#include "NHPriorLogBinomial.h"

#include "NHModelLogPoisson.h"
#include <iostream>
#include "NHPriorLogNormalMedianEF.h"
#include "NHPriorLogNormalMedianSigma.h"
#include "LognormalDistribution.h"
#include "TrLognormalDistribution.h"

NHModelLogBinomial::NHModelLogBinomial(void)
{
}

NHModelLogBinomial::~NHModelLogBinomial(void)
{
}


NHInterpretation * NHModelLogBinomial::createInterpretation(int type)
{
	NHLikelihoodLogBinomial *likelihood = new NHLikelihoodLogBinomial();
	return new NHInterpretation( likelihood );
}

MetropolisGenerator * NHModelLogBinomial::createGenerator()
{
	//return new MetropolisGammaGenerator();
	return new MetropolisLNGenerator();
}

NHPrior * NHModelLogBinomial::createDefaultPrior()
{
	return new NHPriorLogBinomial();
}

NHPrior * NHModelLogBinomial::createUserPrior(int mode, double nu1, double tau1, double nu2, double tau2)
{
	if (mode == 2) { // median and error factor
		return new NHPriorLogNormalMedianEF(nu1,tau1,nu2,tau2);
	} else {
		return new NHPriorLogNormalMedianSigma(nu1,tau1,nu2,tau2);
	}
}

/**
 * Computes distribution over the specified measure.
 * Method iterates through sample set, computes measure values
 * for each sample, and stores them in the distribution object.
 */
int NHModelLogBinomial::computeMeasure(int parameter, double value, int measure,
										Distribution &distribution, StatusPtr status)
{
	double prob,msr;

	if( measure == NHMEASURE_POPVAR_PDF || measure == NHMEASURE_POPVAR_CDF || measure == NHMEASURE_POPVAR_MEAN) {
		samples->first();
		distribution.clear(samples->size());
		while ( !samples->endOfSamples()){

			double nu = log( samples->getParameters().getValue(0) );
			double tau = samples->getParameters().getValue(1);

			double cdf1 = DCDFLIBInterface::computeLognormalCDF(nu,tau,1);

			if (measure == NHMEASURE_POPVAR_PDF) 
				msr = DCDFLIBInterface::computeLognormalPDF( nu , tau , value ) / cdf1;
			else if (measure == NHMEASURE_POPVAR_CDF)
				msr = DCDFLIBInterface::computeLognormalCDF( nu , tau , value ) / cdf1;
			else
				msr = TrLogNormalDistribution(nu,tau).getMean();

			if( msr >= 0 ){
				prob = samples->getProbability();
				distribution.add( msr, prob );
			}
			samples->next();
		}

		return 0;
	}
	else
		return -1;
}

/**
 * Always returns true.
 */
bool NHModelLogBinomial::accept(NHInterpretation & interpretation)
{
	return true; // accept all data sets.
}

/**
 * Computes the percentile of the specified truncated lognormal.
 */
double NHModelLogBinomial::computePercentile(const Parameters &params, double percentile) const
{
	double nu = log(params.getValue(0));
	double tau = params.getValue(1);
	double cdf1 = DCDFLIBInterface::computeLognormalCDF(nu,tau,1);
	return DCDFLIBInterface::computeLognormalQuantile(nu,tau,percentile * cdf1);
}

/**
 * Returns the mean of the specified truncated lognormal distribution.
 */
double NHModelLogBinomial::computeMean(const Parameters &params) const
{
	double nu = log(params.getValue(0));
	double tau = params.getValue(1);
	return TrLogNormalDistribution(nu,tau).getMean();
}

/**
 * Computes mean of truncated lognormal with specified parameters.
 */
double NHModelLogBinomial::computeVariance(const Parameters &params) const
{
	double nu = log(params.getValue(0));
	double tau = params.getValue(1);
	return TrLogNormalDistribution(nu,tau).getVariance();
}

/**
 * Estimates the location of the specified quantile.
 */
double NHModelLogBinomial::estimatePercentile(double lo, double lo_cdf, double hi, double hi_cdf, double cdf) const
{
	// this code originally from the NHModelLogPoisson. Keep for now.
	LogNormalDistribution lnd(0,1);
	double lnlo = log(lo);
	double lnhi = log(hi);
	double lnzlo = log(lnd.getQuantile(lo_cdf));
	double lnzhi = log(lnd.getQuantile(hi_cdf));
	double nu = lnlo + (lnhi - lnlo)*(-lnzlo)/(lnzhi - lnzlo);
	double tau = (lnhi - lnlo)/(lnzhi - lnzlo);
	LogNormalDistribution fit(nu,tau);
	double x = fit.getQuantile(cdf);

	if (x > 1) {
		x = 1;
	}

	return x;
}
