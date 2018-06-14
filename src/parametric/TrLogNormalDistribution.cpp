// $Header: C:/cvsrepo/ptdev/native/redcas/parametric/TrLogNormalDistribution.cpp,v 1.3 2002/02/20 18:31:25 fgroen Exp $

// TrLogNormalDistribution.cpp: implementation of the TrLogNormalDistribution class.
//
//////////////////////////////////////////////////////////////////////

#include "TrLogNormalDistribution.h"
#include "LogNormalDistribution.h"
#include "NormalDistribution.h"
#include "../global/Error.h"
#include <stdlib.h>
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TrLogNormalDistribution::TrLogNormalDistribution(double nu, double tau)
{
	if (checkParameters(nu,tau)) {
		this->nu = nu;
		this->tau = tau;
		iota = LogNormalDistribution(nu,tau).getCDF(1.0);
	} else throw INVALID_ARGUMENT;
}

double TrLogNormalDistribution::getSample() const
{
	double result = 0;

	if (iota > 0.2) {
		while (result <= 0 || result >= 1) 
			result = LogNormalDistribution(nu,tau).getSample();
	} else {
		double u = rand() * 1.0 / RAND_MAX;
		result = getQuantile(u);
	}

	return result;
}

double TrLogNormalDistribution::getQuantile(double x) const
{
	return LogNormalDistribution(nu,tau).getQuantile(x * iota);
}

double TrLogNormalDistribution::getVariance() const
{
	double x1 = exp(2*(nu+tau*tau));
	double x2 = NormalDistribution(0,1).getCDF(-(nu+2*tau*tau)/tau);
	double x3 = LogNormalDistribution(nu,tau).getCDF(1);
	double x4 = getMean();
	return x1 * x2 / x3 - (x4 * x4);
}

double TrLogNormalDistribution::getMean() const
{

// Mean of the truncated lognormal distribution is equal to:
//
//    mean = 1/2 Exp[nu + tau^2 / 2] (1 - Erf[(nu + tau^2) / (Sqrt[2] tau)] / CDF[LogNormalDistribution[nu,tau],1]
//
// where Erf[x] is the integral of the gaussian distribution
//
//    Erf[y] = 2 / Sqrt[Pi] Integral[Exp[-x^2],{x,0,y}]
//
// This result was obtained using Mathematica 4 by solving and reducing the expression 
// 
//    mean = Integrate[Exp[-((Log[x]-nu)/tau)^2/2]/(tau Sqrt[2Pi]),{x,0,1}] 
//
// which is the integral of the lognormal distribution multiplied by parameter x
// and dividing it by the term CDF[LogNormalDistribution[nu,tau],1]. The latter is 
// done to account for the fact that truncation takes place over the interval [0,1]
//
// In turn, the mean can also be written as
//
//    mean = Exp[nu + tau^2 / 2] CDF[NormalDistribution[0,1],-(nu + tau^2) / tau] / CDF[LogNormalDistribution[nu,tau],1]

//
// Note: Erf[x/Sqrt[2]] = 1 - 2*CDF[NormalDistribution[0,1],-x]
//


	return exp(nu + tau * tau / 2) * NormalDistribution(0,1).getCDF(-(nu + tau * tau)/tau) / iota;
}

double TrLogNormalDistribution::getCDF(double x) const
{
	return LogNormalDistribution(nu,tau).getCDF(x) / iota;
}

double TrLogNormalDistribution::getLogPDF(double x) const
{
	return LogNormalDistribution(nu,tau).getLogPDF(x) - log(iota);
}

bool TrLogNormalDistribution::checkParameters(double nu, double tau) 
{
	return tau > 0;
}
