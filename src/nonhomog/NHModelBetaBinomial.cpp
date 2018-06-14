//
// File:      $Id: NHModelBetaBinomial.cpp 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//

#include "stdafx.h"
#include "NHModelBetaBinomial.h"
#include "NHBetaSampleGenerator.h"
#include "Beta0595InputMode.h"
#include "BetaDistribution.h"
#include <math.h>
#include <iostream>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NHModelBetaBinomial::NHModelBetaBinomial()
{
	 
}

NHModelBetaBinomial::~NHModelBetaBinomial()
{

}

NHInterpretation * NHModelBetaBinomial::createInterpretation(int type)
{
	NHLikelihoodBetaBinomial *likelihood = new NHLikelihoodBetaBinomial();
	return new NHInterpretation( likelihood );
}

MetropolisGenerator * NHModelBetaBinomial::createGenerator()
{
	return new NHBetaSampleGenerator();
}

NHPrior * NHModelBetaBinomial::createDefaultPrior()
{
	return new NHPriorBetaBinomial();

}

NHPrior * NHModelBetaBinomial::createUserPrior(int mode, double nu1, double tau1, double nu2, double tau2)
{
	return new NHPriorBetaBinomial(nu1,tau1,nu2,tau2);
}

int NHModelBetaBinomial::computeMeasure(int parameter, double value, int measure,
										Distribution &distribution, StatusPtr status)
{
	double prob,msr;

	if( measure == NHMEASURE_POPVAR_PDF || measure == NHMEASURE_POPVAR_CDF || measure == NHMEASURE_POPVAR_MEAN) {
		samples->first();
		distribution.clear(samples->size());
		while ( !samples->endOfSamples() ){

			double alpha = samples->getParameters().getValue(0);
			double beta = samples->getParameters().getValue(1);

			if (measure == NHMEASURE_POPVAR_PDF) 
				msr = DCDFLIBInterface::computeBetaPDF( alpha,beta, value );
			else if (measure == NHMEASURE_POPVAR_CDF)
				msr = DCDFLIBInterface::computeBetaCDF( alpha,beta, value );
			else 
				msr = alpha / (alpha + beta);

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
int NHModelBetaBinomial::computeBoundsEx(int parameter, double &lower, double &higher)
{
	double alpha_i, beta_i, prob, lower_i, higher_i;

	Distribution *DistribLower = new Distribution( samples->size() );
	Distribution *DistribUpper = new Distribution( samples->size() );

	samples->first();
	while ( !samples->endOfSamples() ){
		alpha_i = samples->getParameters().getValue(0);
		beta_i = samples->getParameters().getValue(1);
		prob = samples->getProbability();

		lower_i = DCDFLIBInterface::computeBetaQuantile( alpha_i, beta_i, 0.01);
		if( !lower_i )
			lower_i = 0.001;
		DistribLower->add( lower_i, prob );

		higher_i = DCDFLIBInterface::computeBetaQuantile( alpha_i, beta_i, 0.999);
		if( !higher_i )
			higher_i = 0.999;
		DistribUpper->add( higher_i, prob );
		
		samples->next();
	}

	lower = DistribLower->getPercentile( 0.01 );
	higher = DistribUpper->getPercentile( 0.9999 );

	// Keep lower and upper limits in the float data type range to avoid underflow and
	// overflow in double operations, typically in divisions...
	if( lower < 1e-38 )
		lower = 1e-38;
	if( higher >= 1 )
		higher = 0.9999;
	
	delete DistribLower;
	delete DistribUpper;

	this->restrictBoundsEx(lower,higher);

	return 0;
}
*/

bool NHModelBetaBinomial::accept(NHInterpretation & interpretation)
{
//	int pnts = interpretation.getObservationCount();
//	int events = interpretation.getTotalEvents(NHINTERPRETATION_EVENTS_DEMANDS);
//	int demands = interpretation.getTotalExposure();
////	cout << "NHBB: " << pnts << " POINTS; " << events << " EVENTS" << endl;
//	return (pnts >= 2 && events >= 1);
	return true; // accept all data sets.
}

double NHModelBetaBinomial::computePercentile(const Parameters &params, double percentile) const
{
	double alpha = params.getValue(0);
	double beta = params.getValue(1);
	return DCDFLIBInterface::computeBetaQuantile(alpha,beta,percentile);
}

double NHModelBetaBinomial::computeMean(const Parameters &params) const
{
	double alpha = params.getValue(0);
	double beta = params.getValue(1);
	return alpha / (alpha + beta);
}

double NHModelBetaBinomial::computeVariance(const Parameters &params) const
{
	double alpha = params.getValue(0);
	double beta = params.getValue(1);
	return (alpha * beta) / ((alpha + beta) * (alpha + beta) * (alpha + beta + 1));	
}

/**
 * Returns an approximated value for the percentile, given the two positions on 
 * a Beta distribution. If conditions are favorable, a Beta fit will be performed,
 * and a quantile estimated from that Beta. Else, a generic value (geometric average
 * is returned.
 */
double NHModelBetaBinomial::estimatePercentile(double lo, double lo_cdf, double hi, double hi_cdf, double cdf) const
{

	if (lo == 0 || hi == 1 || lo_cdf == 0 || hi_cdf == 1) {
		return lo + (lo + hi) / 2;
	}

	// decide whether to allow a Beta fit on the percentiles.
	bool allow_fit = true;
	if (hi <= 0.5 && hi / lo < 1.05) allow_fit = false;
	if (hi >= 0.5 && (1 - lo) / (1 - hi) < 1.05) allow_fit = false;
	if (hi_cdf <= 0.5 && hi-cdf / lo_cdf < 1.1) allow_fit = false;
	if (hi_cdf >= 0.5 && (1 - lo_cdf) / (1 - hi_cdf) < 1.1) allow_fit = false;

	int retval = 0;
	double x = 0;
	if (allow_fit) {
		// perform the fit on the percentiles.
		double alpha,beta;
		Beta0595InputMode mode(lo_cdf,hi_cdf);
		retval = mode.convert(lo,hi,alpha,beta);
		x = DCDFLIBInterface::computeBetaQuantile(alpha,beta,cdf);
	}

	if (!allow_fit || retval != 0) {
		if (cdf > lo_cdf && cdf < hi_cdf) {
			x = pow(lo * hi,0.5);
		} else if (cdf < lo_cdf) {
			x = lo / 2;
		} else {
			x = (1 + hi) / 2;
		}
	}

	return x;
}