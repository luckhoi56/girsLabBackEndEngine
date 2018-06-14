// $Header: C:/cvsrepo/ptdev/native/redcas/parametric/LogNormalDistribution.cpp,v 1.2 2001/07/24 21:31:42 fgroen Exp $

// LogNormalDistribution.cpp: implementation of the LogNormalDistribution class.
//
//////////////////////////////////////////////////////////////////////

#include "LogNormalDistribution.h"
#include "NormalDistribution.h"
#include "error.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LogNormalDistribution::LogNormalDistribution(double nu, double tau)
{
	if (checkParameters(nu,tau)) {
		this->nu = nu;
		this->tau = tau;
	} else throw INVALID_ARGUMENT;
}

double LogNormalDistribution::getSample() const
{
	return exp(NormalDistribution(nu,tau).getSample());
}

double LogNormalDistribution::getQuantile(double x) const
{
	if (x < 0 || x >= 1) throw INVALID_ARGUMENT;
	if (x == 0) return 0;
	return exp(NormalDistribution(nu,tau).getQuantile(x));
}

double LogNormalDistribution::getMean() const
{
	return exp(nu + (tau * tau / 2));
}

double LogNormalDistribution::getMedian() const 
{
	return exp(nu);
}

double LogNormalDistribution::getVariance() const
{
	return exp(2 * nu) * (exp(2 * tau * tau) - exp(tau * tau));
}

double LogNormalDistribution::getCDF(double x) const
{
	if (x <= 0) throw INVALID_ARGUMENT;
	return NormalDistribution(nu,tau).getCDF(log(x));
}

double LogNormalDistribution::getLogPDF(double x) const
{
	if (x <= 0) throw INVALID_ARGUMENT;
	double z = (log(x) - nu) / tau;
	return (-z * z / 2) + log(0.3989422804 / (tau * x));
}

bool LogNormalDistribution::checkParameters(double nu, double tau) 
{
	return tau > 0;
}