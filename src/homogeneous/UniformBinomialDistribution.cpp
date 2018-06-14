// UniformBinomialDistribution.cpp: implementation of the UniformBinomialDistribution class.
//
//////////////////////////////////////////////////////////////////////


#include "UniformBinomialDistribution.h"
#include "../ran/RANLIBInterface.h"

#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UniformBinomialDistribution::UniformBinomialDistribution(double min, double max, double k, double n, bool loguni)
{
	double i0,i1,i2;
	
	this->lo = min;
	this->hi = max;
	this->k = k;
	this->n = n;
	this->m = loguni ? 1 : 0;
	support = new TabularBinomialSupport(k,n,m,lo,hi);
	support->computeIntegrals(i0,i1,i2,lnc);
	this->norm = i0;
	this->mean = i1 / i0;
	this->var = i2 / i0 - mean * mean;
}

UniformBinomialDistribution::~UniformBinomialDistribution()
{
	delete support;
}

double UniformBinomialDistribution::getQuantile(double z) const
{
	double x;
	int retval = support->inverseIntegral(z * norm,x);
	return x;
}

double UniformBinomialDistribution::getCDF(double x) const
{
	double cdf,lnc;
	support->computeIntegral(x,cdf,lnc);
	cdf /= norm;
	return cdf;
}

double UniformBinomialDistribution::getLogPDF(double x) const
{
	double lnx = log(x);
	double lnomx = x > 0.001 ? log(1 - x) : -x*(1+x*(1+2*x*(1+3*x*(1+4*x*(1+5*x/6)/5)/4)/3)/2);
	double lnpdf = (k - m) * lnx + (n - k) * lnomx - log(norm) - lnc;
	return lnpdf;
}

double UniformBinomialDistribution::getSample() const
{
	double z = RANLIBInterface::generateUniformDeviate();
	return getQuantile(z);
}
