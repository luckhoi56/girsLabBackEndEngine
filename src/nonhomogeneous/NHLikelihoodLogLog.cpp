//
// File:      $Id: NHLikelihoodLogLog.cpp 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//


#include "NHLikelihoodLogLog.h"
#include "../homogeneous/TabularLogNormalBinomialIntegrator.h"
#include "../homogeneous/LogNormalBinomialDistribution.h"
#include "../dcdf/DCDFLIBInterface.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define LOG_ZERO -1e-300

NHLikelihoodLogLog::NHLikelihoodLogLog()
{

}

NHLikelihoodLogLog::~NHLikelihoodLogLog()
{

}

/// Deprecated. Use NHLikelihoodLogBinomial or NHLikelihoodLogPoisson instead.
double NHLikelihoodLogLog::getLogLikelihood(const Parameters &params, NHObservationIterator &it, int observations)
{
	double nu = log(params.getValue(0));
 	double tau  = params.getValue(1);
	double like = 0;
	double k, n;
	double median, sigma;
	double like_at_i;
	
	double tauLow = 0.1;
	double tauHigh = 5;
	
	//if(tau > tauLow && tau < tauHigh) {
		for (int i = 0 ; i < observations && like != LOG_ZERO ; i++) {
			k		= (*it).events;
			n		= (*it).exposure;
			median	= (*it).median > 0 ? log((*it).median) : (*it).median;
			sigma	= (*it).sigma;
			
			if(!k && !n)
				like_at_i = getLogNormalLogNormal(nu, tau, median, sigma);
			else
				like_at_i = getLogNormalBinomialLogNormal(nu, tau, k, n, median, sigma);
			
			if (like_at_i != LOG_ZERO) {
				like += like_at_i;
			} else {
				like = LOG_ZERO;
			}
							
			it++;
 		}

		return like;
	//}
	//else
		//return -1e200;
}

///Log Likelihood for LogNormal-Poisson-LogNormal model.
double NHLikelihoodLogLog::getLogNormalBinomialLogNormal(double nu, double tau, double k,
														double n, double median,
														double sigma)
{
	double c = DCDFLIBInterface::computeLognormalCDF( nu, tau, 1.0);
	double temp1 = log(c * (n+1) * sigma * tau * sqrt(pow(sigma,-2)+pow(tau,-2)));
	double temp2 = median*median+nu*nu+2*median*(-nu+sigma*sigma+tau*tau);
	double temp3 = 2*(sigma*sigma+tau*tau);
    double like_at_i = -temp2/temp3 - temp1;

	double newmedian = median/(sigma*sigma) + nu/(tau*tau);
	newmedian /= (pow(sigma,-2) + pow(tau,-2));
	double newsigma = sqrt(1.0/(pow(sigma,-2) + pow(tau,-2)));

//	LogNormalBinomialDistribution f(newmedian, newsigma, k, n);
//	double hi = f.getUpperBound();

	double io = 0;
	double lnc = 0;
	TabularLogNormalBinomialIntegrator I(0, 1, newmedian, newsigma, k+1, n-k+1, 0,0);
	I.computeIntegral(1, io, lnc);
	
	if (io != 0) 
	  like_at_i += log(io);
	else
	  like_at_i = LOG_ZERO;
	
	return like_at_i;
}

//Log Likelihood for LogNormal-LogNormal model.
double NHLikelihoodLogLog::getLogNormalLogNormal(double nu, double tau, double median,
												 double sigma)
{
	double temp1 = log(sigma * tau * sqrt(pow(sigma,-2)+pow(tau,-2)));
	double temp2 = median*median+nu*nu+2*median*(-nu+sigma*sigma+tau*tau);
	double temp3 = 2*(sigma*sigma+tau*tau);

	return -temp2/temp3 - temp1;
}

/*
double NHLikelihoodLogLog::getLogLikelihood(const Parameters &params, NHObservationIterator &it, int observations)
{
	double alpha = params.getValue(0); //Popvar mean -> Theta
 	double beta  = params.getValue(1); //Popvar Std -> Omega
 	double like = 0;
	double Lambda, Std;
	double temp1, temp2, temp3;

	double alphaLow = 1e-15;
	double alphaHigh = 10;
	double betaLow = 0.1;
	double betaHigh = 5;

//	if(this->modeParams) {
//		alphaLow = modeParams->getValue(0) * 1e-1;
//		alphaHigh = modeParams->getValue(0) * 1e2;
//		betaLow = modeParams->getValue(1) * 1e-2;
//		betaHigh = modeParams->getValue(1) * 1e2;
//	}

	//if( alpha > alphaLow && alpha < alphaHigh && beta > betaLow && beta < betaHigh ) {
	if(beta > betaLow && beta < betaHigh) {
		for (int i = 0 ; i < observations ; i++) {

			Lambda = (*it).median; //Mean
			Std = (*it).sigma; //Std

			temp1 = beta * beta + Std * Std;
			temp2 = log( 1 / sqrt( temp1 ) );
			temp3 = log( Lambda ) - log( alpha );
			
			like += temp2 - 0.5 * ( pow( temp3, 2 ) / temp1 );
			
			it++;
 		}

		return like;
	}
	else
		return -1e200;
}
*/

void NHLikelihoodLogLog::preprocess(const NHObservation &observation)
{

}
