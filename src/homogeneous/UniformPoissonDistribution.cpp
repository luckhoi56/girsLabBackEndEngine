// UniformPoissonDistribution.cpp: implementation of the UniformPoissonDistribution class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UniformPoissonDistribution.h"
#include "TabularPoissonSupport.h"
#include "RANLIBInterface.h"
#include <math.h>

#include <iostream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UniformPoissonDistribution::UniformPoissonDistribution(double min, double max, double k, double t, bool loguni)
{
	double i0,i1,i2,lnc;
	
	this->lo = min;
	this->hi = max;
	this->k = k;
	this->t = t;
	this->n = loguni ? 1 : 0;
	support = new TabularPoissonSupport(k,t,n,lo,hi);
	support->computeIntegrals(i0,i1,i2,lnc);
	this->norm = i0;
	this->mean = i1 / i0;
	this->var = i2 / i0 - mean * mean;
}

UniformPoissonDistribution::~UniformPoissonDistribution()
{
	delete support;
}

double UniformPoissonDistribution::getQuantile(double z) const
{
	double x;
	int retval = support->inverseIntegral(z * norm,x);
	return x;
}

double UniformPoissonDistribution::getCDF(double x) const
{
	double cdf,lnc;
	support->computeIntegral(x,cdf,lnc);
	cdf /= norm;
	return cdf;
}

double UniformPoissonDistribution::getLogPDF(double x) const
{
	double lnpdfx = -x * t + k * log(x * t) - n * log(x) - log(norm);
	double lnc, lnpdf;
	support->computeLogIntegrant(x,lnpdf,lnc);
	lnpdf -= log(norm);
	return lnpdf;
}

double UniformPoissonDistribution::getSample() const
{
	double z = RANLIBInterface::generateUniformDeviate();
	return getQuantile(z);
}
