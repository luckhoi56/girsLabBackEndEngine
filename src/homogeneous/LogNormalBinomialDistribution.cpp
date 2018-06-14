// LogNormalBinomialDistribution.cpp: implementation of the LogNormalBinomialDistribution class.
//
//////////////////////////////////////////////////////////////////////


#include "LogNormalBinomialDistribution.h"
#include "../global/SimpsonIntegrator.h"
#include "../parametric/BetaDistribution.h"
#include "../parametric/LogNormalDistribution.h"
#include "../global/Parameters.h"
#include "../ran/RANLIBInterface.h"

#include <iostream>
#include <math.h>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define min(a, b)  (((a) < (b)) ? (a) : (b)) 
#define max(a, b)  (((a) > (b)) ? (a) : (b)) 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
 * Constructs a LogNormalBinomial distribution object. This object
 * behaves as the result of a distribution that is the result of a
 * lognormal-binomial updating procedure.
 */
LogNormalBinomialDistribution::LogNormalBinomialDistribution(double nu, double tau, double k, double n)
{

#ifdef _DEBUG
//	cout << "UELB: " << nu << " " << tau << " " << k << " " << n << endl;
#endif
	this->nu = nu;
	this->tau = tau;
	this->k = k;
	this->N = n;

	determineBounds();

	LogNormalDistribution * prior = new LogNormalDistribution(nu,tau);
	BetaDistribution * likelihood = new BetaDistribution(k + 1,n - k + 1);

	double ln_factor = prior->getLogPDF(mode) + likelihood->getLogPDF(mode);
	posterior = new UpdateFunction2(prior,likelihood,-ln_factor);

	ln_posterior = new UpdateFunction(new LogNormalDistribution(nu,tau),
		new BetaDistribution(k + 1,n - k + 1));

	SimpsonIntegrator simpson;
	integral = simpson.simpson145(lowerBound,upperBound,*posterior,0.000001,120);

	if (integral > 0) {
		ln_integral = log(integral) + ln_factor;

		posterior->setMoment(1);
		double mn_temp = simpson.simpson145(lowerBound,upperBound,*posterior,0.000001,120);
		mean = mn_temp / integral;

		posterior->setMoment(2);
		double vr_temp = simpson.simpson145(lowerBound,upperBound,*posterior,0.000001,120);
		var = vr_temp / integral -  mean * mean;
	}

	posterior->setMoment(0);
}

LogNormalBinomialDistribution::~LogNormalBinomialDistribution()
{
	delete posterior;
	delete ln_posterior;
}

/**
 * Returns the cdf at x
 */
double LogNormalBinomialDistribution::getCDF(double x) const
{
	if (x <= lowerBound) return 0;
	if (x >= upperBound) return 1;

	SimpsonIntegrator simpson;
	return simpson.simpson145(lowerBound,x,*posterior,0.000001,120) / integral;
}


double LogNormalBinomialDistribution::getSample() const
{
	double u = RANLIBInterface::generateUniformDeviate();
	return getQuantile(u);
}

/**
 * Returns the natural log of the pdf at x.
 */
double LogNormalBinomialDistribution::getLogPDF(double x) const
{
	return ln_posterior->evaluate(Parameters(x)) - ln_integral;
}

/**
 * Returns the quantile for the lognormal binomial distribution.
 *
 * Quantiles are obtained using a simple bisection search
 * in the interval lowerbound - upperbound. Function returns 
 * whatever value is obtained after at most 50 bisection
 * iterations, or when result is accurate to 6 digits.
 */
double LogNormalBinomialDistribution::getQuantile(double z) const
{
	double hival = upperBound;
	double hicdf = 1;

	double loval = lowerBound;
	double locdf = 0;

	double mdval = (upperBound + lowerBound) / 2;
	double mdcdf = getCDF(mdval);

	int iterations = 0;
	int max_iterations = 100;

	while ((loval == 0 || hival / loval > 1.000001) && iterations < max_iterations)
	{
		if (z > mdcdf) {
			loval = mdval;
			locdf = mdcdf;
		} else {
			hival = mdval;
			hicdf = mdcdf;
		}

		mdval = (hival + loval) / 2;
		mdcdf = getCDF(mdval);
	}

	return mdval;
}


/**
 * Determines credible integration interval for this distribution.
 * In addition establishes approximate location of the mode.
 */
void LogNormalBinomialDistribution::determineBounds()
{
	LogNormalDistribution prior(nu,tau);
	BetaDistribution likelihood(k + 1,N - k + 1);

	double x;

	double lo = min(prior.getQuantile(0.0001),likelihood.getQuantile(0.0001));
	while (!delta(prior,likelihood,lo)) 
		lo /= 2;

	double hi = min(max(prior.getQuantile(0.9999),likelihood.getQuantile(0.9999)),1);

	while (delta(prior,likelihood,hi) && hi < 1) 
		hi = min(hi*2,1);

	while (hi / lo > 1.1) {
		x = exp(log(lo*hi)/2);
		if (delta(prior,likelihood,x)) {
			lo = x;
		} else {
			hi = x;
		}
	}

	mode = x;

    double plox = prior.getQuantile(0.000001);
	double phix = prior.getQuantile(0.999999);
	double llox = likelihood.getQuantile(0.000001);
	double lhix = likelihood.getQuantile(0.999999);

	if (plox <= llox && phix >= lhix) {
		lowerBound = llox;
		upperBound = lhix;
	} else if (plox > llox && phix < lhix) {
		lowerBound = plox;
		upperBound = phix;
	} else {
		lowerBound = (llox < plox) ? llox : plox;
		upperBound = (lhix > phix) ? lhix : phix;
	}

	upperBound = min(upperBound,1);
	lowerBound = min(lowerBound,0.9);

	double p_max = prior.getLogPDF(mode) + likelihood.getLogPDF(mode);

	// descend in positive and negative direction, until value is sufficiently
	// small such that it is not expected to give significant contribution to
	// the integral.
	x = lowerBound;
	while (prior.getLogPDF(x) + likelihood.getLogPDF(x) > p_max - log(10000 * x / mode))
		x /= 2;
	lowerBound = x;

	x = upperBound;
	while (x <1  && prior.getLogPDF(x) + likelihood.getLogPDF(x) > p_max - log(10000 * x / mode))
		x = min(2*x,1);
	upperBound = x;
}

bool LogNormalBinomialDistribution::delta(LogNormalDistribution & prior, BetaDistribution & like, double x)
{
	double y0 = prior.getLogPDF(x/1.0001) + like.getLogPDF(x/1.0001);
	double y1 = prior.getLogPDF(x) + like.getLogPDF(x);
	return y1 > y0;
}
