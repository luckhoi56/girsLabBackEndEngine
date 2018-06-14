//
// File:      $Id: NHModelLogPoisson.cpp 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//


#include "NHModelLogPoisson.h"
#include "NHPriorLogNormalMedianEF.h"
#include "NHPriorLogNormalMedianSigma.h"
#include "../parametric/LogNormalDistribution.h"

#include <iostream>

NHModelLogPoisson::NHModelLogPoisson()
{

}

NHModelLogPoisson::~NHModelLogPoisson()
{

}


NHInterpretation * NHModelLogPoisson::createInterpretation(int type)
{
	NHLikelihoodLogPoisson *likelihood = new NHLikelihoodLogPoisson();
	return new NHInterpretation( likelihood );
}

MetropolisGenerator * NHModelLogPoisson::createGenerator()
{
	//return new MetropolisGammaGenerator();
	return new MetropolisLNGenerator();
}

NHPrior * NHModelLogPoisson::createDefaultPrior()
{
	return new NHPriorLogPoisson();

}

NHPrior * NHModelLogPoisson::createUserPrior(int mode, double nu1, double tau1, double nu2, double tau2)
{
	if (mode == 2) { // median and error factor
		return new NHPriorLogNormalMedianEF(nu1,tau1,nu2,tau2);
	} else {
		return new NHPriorLogNormalMedianSigma(nu1,tau1,nu2,tau2);
	}
}

int NHModelLogPoisson::computeMeasure(int parameter, double value, int measure,
										Distribution &distribution, StatusPtr status)
{
	double prob,msr;

	if( measure == NHMEASURE_POPVAR_PDF || measure == NHMEASURE_POPVAR_CDF || measure == NHMEASURE_POPVAR_MEAN) {
		samples->first();
		distribution.clear(samples->size());
		while ( !samples->endOfSamples() ){

			double nu = log( samples->getParameters().getValue(0) );
			double tau = samples->getParameters().getValue(1);

			if (measure == NHMEASURE_POPVAR_PDF) 
				msr = DCDFLIBInterface::computeLognormalPDF( nu , tau , value );
			else if (measure == NHMEASURE_POPVAR_CDF)
				msr = DCDFLIBInterface::computeLognormalCDF( nu , tau , value );
			else
				msr = LogNormalDistribution(nu,tau).getMean();

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
/*
int NHModelLogPoisson::computeBoundsEx(int parameter, double &lower, double &higher)
{
	double alpha_i, beta_i, prob, lower_i, higher_i;

	Distribution *DistribLower = new Distribution( samples->size() );
	Distribution *DistribUpper = new Distribution( samples->size() );

	samples->first();
	while ( !samples->endOfSamples() ){
		alpha_i = samples->getParameters().getValue(0);
		beta_i = samples->getParameters().getValue(1);
		prob = samples->getProbability();

		lower_i = DCDFLIBInterface::computeLognormalQuantile( log( alpha_i ), beta_i, 0.0001);
		if( !lower_i )
			lower_i = 0.001;
		DistribLower->add( lower_i, prob );

		higher_i = DCDFLIBInterface::computeLognormalQuantile( log( alpha_i ), beta_i, 0.9999);
		if( !higher_i )
			higher_i = 0.999;
		DistribUpper->add( higher_i, prob );
		
		samples->next();
	}

	lower = DistribLower->getPercentile( 0.0001 );
	higher = DistribUpper->getPercentile( 0.9999 );

	// Keep lower and upper limits in the float data type range to avoid underflow and
	// overflow in double operations, typically in divisions...
	if( lower < 1e-38 )
		lower = 1e-38;
	if( higher > 1e38 )
		higher = 1e38;
	
	delete DistribLower;
	delete DistribUpper;

	this->restrictBoundsEx(lower,higher);

	return 0;
}
*/

bool NHModelLogPoisson::accept(NHInterpretation & interpretation)
{
//	int pnts = interpretation.getObservationCount();
//	int events = interpretation.getTotalEvents(NHINTERPRETATION_EVENTS_TIME);
//	cout << "NHLP: " << pnts << " POINTS; " << events << " EVENTS" << endl;
//	return (pnts >= 2 && events >= 1);
	return true; // accept all data sets.
}

double NHModelLogPoisson::computePercentile(const Parameters &params, double percentile) const
{
	double nu = log(params.getValue(0));
	double tau = params.getValue(1);
	return DCDFLIBInterface::computeLognormalQuantile(nu,tau,percentile);
}

double NHModelLogPoisson::computeMean(const Parameters &params) const
{
	double nu = log(params.getValue(0));
	double tau = params.getValue(1);
	return exp(nu + (tau * tau / 2));
}

double NHModelLogPoisson::computeVariance(const Parameters &params) const
{
	double nu = log(params.getValue(0));
	double tau = params.getValue(1);
	return exp(2 * nu) * (exp(2 * tau * tau) - exp(tau * tau));
}

double NHModelLogPoisson::estimatePercentile(double lo, double lo_cdf, double hi, double hi_cdf, double cdf) const
{

// lo = Exp[-4]; cdflo = 0.90;
// hi = Exp[-2]; cdfhi = 0.95;
// lnlo = Log[lo];
// lnhi = Log[hi];
// lnzlo = Log[Quantile[LogNormalDistribution[0, 1], cdflo]];
// lnzhi = Log[Quantile[LogNormalDistribution[0, 1], cdfhi]];
// nu = lnlo + (lnhi - lnlo)(-lnzlo)/(lnzhi - lnzlo);
// tau = (lnhi - lnlo)/(lnzhi - lnzlo);
// dist = LogNormalDistribution[nu, tau];
// Plot[CDF[dist, x], {x, lo, hi}];
// CDF[dist, lo]
// CDF[dist, hi]

	LogNormalDistribution lnd(0,1);
	double lnlo = log(lo);
	double lnhi = log(hi);
	double lnzlo = log(lnd.getQuantile(lo_cdf));
	double lnzhi = log(lnd.getQuantile(hi_cdf));
	double nu = lnlo + (lnhi - lnlo)*(-lnzlo)/(lnzhi - lnzlo);
	double tau = (lnhi - lnlo)/(lnzhi - lnzlo);
	LogNormalDistribution fit(nu,tau);
	double x = fit.getQuantile(cdf);
	return x;
}