//
// File:      $Id: NHModelGammaPoisson.cpp 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//


#include "NHModelGammaPoisson.h"
#include "NHGammaSampleGenerator.h"
#include "../parametric/GammaDistribution.h"
#include "../parametric/Gamma0595InputMode.h"

#include <iostream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NHModelGammaPoisson::NHModelGammaPoisson()
{

}

NHModelGammaPoisson::~NHModelGammaPoisson()
{

}


NHInterpretation * NHModelGammaPoisson::createInterpretation(int type)
{
//	if ( type == NHINTERPRETATION_EVENTS_DEMANDS ) {
		NHLikelihoodGammaPoisson *likelihood = new NHLikelihoodGammaPoisson();
		return new NHInterpretation( likelihood );
//	}
//	else
//		return 0L;
}

MetropolisGenerator * NHModelGammaPoisson::createGenerator()
{
	return new NHGammaSampleGenerator();
}

NHPrior * NHModelGammaPoisson::createDefaultPrior()
{
	return new NHPriorGammaPoisson();
}


NHPrior * NHModelGammaPoisson::createUserPrior(int mode, double nu1, double tau1, double nu2, double tau2)
{
	return new NHPriorGammaPoisson(nu1,tau1,nu2,tau2);
}

int NHModelGammaPoisson::computeMeasure(int parameter, double value, int measure,
										Distribution &distribution, StatusPtr status)
{
	double prob,msr;

	if( measure == NHMEASURE_POPVAR_PDF || measure == NHMEASURE_POPVAR_CDF || measure == NHMEASURE_POPVAR_MEAN) {
		samples->first();
		distribution.clear(samples->size());
		while ( !samples->endOfSamples() ){

			double alpha = samples->getParameters().getValue(0);
			double lambda = samples->getParameters().getValue(1);

			if (measure == NHMEASURE_POPVAR_PDF) 
				msr = DCDFLIBInterface::computeGammaPDF( alpha, lambda, value );
			else if (measure == NHMEASURE_POPVAR_CDF)
				msr = DCDFLIBInterface::computeGammaCDF( alpha, lambda, value );
			else 
				msr = alpha / lambda;

			if( msr >= 0 ) {
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
int NHModelGammaPoisson::computeBoundsEx(int parameter, double &lower, double &higher)
{
	double alpha_i, beta_i, prob, lower_i, higher_i;

	Distribution *DistribLower = new Distribution( samples->size() );
	Distribution *DistribUpper = new Distribution( samples->size() );

	samples->first();
	while ( !samples->endOfSamples() ){
		alpha_i = samples->getParameters().getValue(0);
		beta_i = samples->getParameters().getValue(1);
		prob = samples->getProbability();

		lower_i = DCDFLIBInterface::computeGammaQuantile( alpha_i, beta_i, 0.00001);
		if( !lower_i )
			lower_i = 0.001;
		DistribLower->add( lower_i, prob );

		higher_i = DCDFLIBInterface::computeGammaQuantile( alpha_i, beta_i, 0.99999);
		if( !higher_i )
			higher_i = 0.999;
		DistribUpper->add( higher_i, prob );
		
		samples->next();
	}

	lower = DistribLower->getPercentile( 0.00001 );
	higher = DistribUpper->getPercentile( 0.99999 );

	
//	double start = 0.001;
//	higher = DistribUpper->getPercentile( 0.999 );
//	Distribution *DistribX = new Distribution( samples->size() );
//	StatusPtr status = 0;
//
//	do {
//		lower = DistribLower->getPercentile( start );
//		computeMeasure( 1, lower, NHMEASURE_POPVAR_CDF, *DistribX, status );
//		double m = DistribX->getMean();
//		
//		if( m >= 0.001 )
//			break;
//		start *= 2;
//	}
//	while ( start <= 0.1 );
	

	// Keep lower and upper limits in the float data type range to avoid underflow and
	// overflow in double operations, typically in divisions...
	if( lower < 1e-38 )
		lower = 1e-38;
	if( higher > 1e38 )
		higher = 1e38;

	delete DistribLower;
	delete DistribUpper;
	//delete DistribX;

	this->restrictBoundsEx(lower,higher);

	return 0;
}
*/

/*
int NHModelGammaPoisson::computeBounds(int parameter, double &lower, double &higher)
{
	double alphamin = 1000, betamin = 1000, alphamax = 0, betamax = 0;

	samples->first();
	while ( !samples->endOfSamples() ){
		if( samples->getParameters().getValue(0) < alphamin )
			 alphamin = samples->getParameters().getValue(0);
		if( samples->getParameters().getValue(0) > alphamax )
			 alphamax = samples->getParameters().getValue(0);
		if( samples->getParameters().getValue(1) < betamin )
			 betamin = samples->getParameters().getValue(0);
		if( samples->getParameters().getValue(1) > betamax )
			 betamax = samples->getParameters().getValue(0);

		samples->next();
	}


	lower = DCDFLIBInterface::computeGammaQuantile( alphamin, betamin, 0.01);
	higher = DCDFLIBInterface::computeGammaQuantile( alphamax, betamax, 0.99);

	return 0;
}

*/

bool NHModelGammaPoisson::accept(NHInterpretation & interpretation)
{
//	int pnts = interpretation.getObservationCount();
//	int events = interpretation.getTotalEvents(NHINTERPRETATION_EVENTS_TIME);
//	int demands = interpretation.getTotalExposure();
//	cout << "NHGP: " << pnts << " POINTS; " << events << " EVENTS" << endl;
//	return (pnts >= 2 && events >= 1);
	return true;
}

double NHModelGammaPoisson::computePercentile(const Parameters &params, double percentile) const
{
	double alpha = params.getValue(0);
	double theta = params.getValue(1);
	return DCDFLIBInterface::computeGammaQuantile(alpha,theta,percentile);
}

double NHModelGammaPoisson::computeMean(const Parameters &params) const
{
	double alpha = params.getValue(0);
	double beta = 1 / params.getValue(1);
	return alpha * beta;
}

double NHModelGammaPoisson::computeVariance(const Parameters &params) const
{
	double alpha = params.getValue(0);
	double beta = 1 / params.getValue(1);
	return alpha * beta * beta;
}

double NHModelGammaPoisson::estimatePercentile(double lo, double lo_cdf, double hi, double hi_cdf, double cdf) const
{

	if (lo == 0 || lo_cdf == 0 || hi_cdf == 1) {
		return lo + (lo + hi) / 2;
	}

	// decide whether to allow a Beta fit on the percentiles.
	bool allow_fit = true;
	if (hi / lo < 1.05) allow_fit = false;
	if (hi_cdf <= 0.5 && hi-cdf / lo_cdf < 1.1) allow_fit = false;
	if (hi_cdf >= 0.5 && (1 - lo_cdf) / (1 - hi_cdf) < 1.1) allow_fit = false;

	int retval = 0;
	double x = 0;
	if (allow_fit) {
		// perform the fit on the percentiles.
		double alpha,beta;
		Gamma0595InputMode mode(lo_cdf,hi_cdf);
		retval = mode.convert(lo,hi,alpha,beta);
		x = DCDFLIBInterface::computeGammaQuantile(alpha,1/beta,cdf);
	}

	if (!allow_fit || retval != 0) {
		if (cdf > lo_cdf && cdf < hi_cdf) {
			x = pow(lo * hi,0.5);
		} else if (cdf < lo_cdf) {
			x = lo / 2;
		} else {
			x = hi * 2;
		}
	}

	return x;
}