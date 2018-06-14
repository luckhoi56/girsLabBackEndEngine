// LogNormalPoissonDistribution.cpp: implementation of the LogNormalPoissonDistribution class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogNormalPoissonDistribution.h"
#include "SimpsonIntegrator.h"

#include "GammaDistribution.h"
#include "LogNormalDistribution.h"
#include "Parameters.h"

#include "RANLIBInterface.h"

#include <iostream>
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define min(a, b)  (((a) < (b)) ? (a) : (b)) 
#define max(a, b)  (((a) > (b)) ? (a) : (b)) 

/******** MATHEMATICA SCRIPT TO VERIFY INTEGRAL, MEAN, AND VARIANCE *****
 
dist1 = LogNormalDistribution[-5, 1];
dist2 = GammaDistribution[2, 1/1000];
lb = 8.008*10^(-5);
lb = 0.00008;
ub = 0.015139923156829;
lnfact = -10.032290599103;
func[x_, mom_] := PDF[dist1, x]PDF[dist2, x]*Exp[lnfact]*x^mom;
norm = NIntegrate[func[x, 0], {x, lb, ub}]
mean = NIntegrate[func[x, 1], {x, lb, ub}]/norm
vari = NIntegrate[func[x, 2], {x, lb, ub}]/norm - mean^2

Plot[{PDF[dist1, 10^x], PDF[dist1, 10^x]PDF[dist2, 10^x]/norm}, {x, -6, 0}, 
  PlotRange -> All]

************************************************************************/

LogNormalPoissonDistribution::LogNormalPoissonDistribution(double nu, double tau, double k, double t)
{
	this->nu = nu;
	this->tau = tau;
	this->k = k;
	this->T = t;

	determineBounds();

	LogNormalDistribution * prior = new LogNormalDistribution(nu,tau);
	GammaDistribution * likelihood = new GammaDistribution(1 / T,k + 1);

	// mode is determined by determineBounds()
	double ln_factor = prior->getLogPDF(mode) + likelihood->getLogPDF(mode);

	posterior = new UpdateFunction2(prior,likelihood,-ln_factor);
	ln_posterior = new UpdateFunction(new LogNormalDistribution(nu,tau),new GammaDistribution(1 / T,k + 1));

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

LogNormalPoissonDistribution::~LogNormalPoissonDistribution()
{
	delete posterior;
	delete ln_posterior;
}

double LogNormalPoissonDistribution::getSample() const
{
	double u = RANLIBInterface::generateUniformDeviate();
	return getQuantile(u);
}

double LogNormalPoissonDistribution::getCDF(double x) const
{
	if (x <= lowerBound) return 0;
	if (x >= upperBound) return 1;

	if (integral == 0) return 0;

	SimpsonIntegrator simpson;
	return simpson.simpson145(lowerBound,x,*posterior,0.000001,120)  / integral;;
}

double LogNormalPoissonDistribution::getLogPDF(double x) const
{
	if (integral == 0) return 0;
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
double LogNormalPoissonDistribution::getQuantile(double z) const
{
	if (integral == 0) return 0;

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
 * Computes three characteristics of the distribution:
 * lowerbound and upper bound for integration, plus the location of the
 * mode of the distribution.
 * Characteristics are stored in corresponding fields of the object.
 */
void LogNormalPoissonDistribution::determineBounds()
{
	// First determine approximate mode of the updated distribution
	// According to Mathematica, the mode is found at
	// ProductLog[Exp[nu-2tau^2+k*tau^2]T*tau^2]/(T*tau^2)
	// where ProductLog[z] is the value of w for which z == w*Exp[w]
	double lnz = nu - 2 * tau * tau + k * tau * tau + log(T) + 2 * log(tau);
	double lnwmin,lnwmax;

	if (lnz < 0) {
		lnwmin = lnz - 1;
		lnwmax = lnz;
	} else if (lnz < 1) {
		lnwmin = lnz - 1;
		lnwmax = 0;
	} else {
		lnwmin = 0;
		lnwmax = log(lnz);
	}

	int iterations = 0;
	int max_iterations = 50;

	while (lnwmax - lnwmin > 0.00001 && iterations < max_iterations) {
		double lnw = (lnwmax + lnwmin) / 2;
		double lnwew = lnw + exp(lnw);
		if (lnwew < lnz) {
			lnwmin = lnw;
		} else {
			lnwmax = lnw;
		}
		iterations++;
	}

	double x = exp((lnwmax + lnwmin) / 2 - log(T) - 2 * log(tau));

	mode = x;

	LogNormalDistribution * prior = new LogNormalDistribution(nu,tau);
	GammaDistribution * likelihood = new GammaDistribution(1 / T,k + 1);

    double plox = prior->getQuantile(0.00000001);
	double phix = prior->getQuantile(0.99999999);
	double llox = likelihood->getQuantile(0.00000001);
	double lhix = likelihood->getQuantile(0.99999999);

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

	double p_max = prior->getLogPDF(mode) + likelihood->getLogPDF(mode);

	// descend in positive and negative direction, until value is sufficiently
	// small such that it is not expected to give significant contribution to
	// the integral.
	x = lowerBound;
	while (prior->getLogPDF(x) + likelihood->getLogPDF(x) > p_max - log(10000 * x / mode))
		x /= 1.2;
	lowerBound = x;

	x = upperBound;
	while (prior->getLogPDF(x) + likelihood->getLogPDF(x) > p_max - log(10000 * x / mode))
		x *= 1.2;
	upperBound = x;

	delete prior;
	delete likelihood;
}
