// TabularPoissonSupport.cpp: implementation of the TabularPoissonSupport class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TabularPoissonSupport.h"
#include "DCDFLIBInterface.h"
#include "SLATECInterface.h"
#include "GammaDistribution.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TabularPoissonSupport::TabularPoissonSupport(double k, double t, int n, double lo, double hi)
{
	this->k = k;
	this->t = t;
	this->n = n;
	this->lo = lo;
	this->hi = hi;

	double a = 1 + k - n;
	lnc = (a > 0) ? DCDFLIBInterface::computeLnGamma(a) : 0;
	loi0 = 0;
	this->lo = 0;
	double loi0_tmp = 0;
	computeIntegral(lo,loi0_tmp,lnc);
	this->loi0 = loi0_tmp;
	this->lo = lo;
}

TabularPoissonSupport::TabularPoissonSupport(double k, double t, int n, double lo, double hi, double gammaln)
{
	this->k = k;
	this->t = t;
	this->n = n;

	this->hi = hi;

	double a = 1 + k - n;
	lnc = gammaln;
	loi0 = 0;
	this->lo = 0;
	double loi0_tmp = 0;
	computeIntegral(lo,loi0_tmp,lnc);
	this->loi0 = loi0_tmp;
	this->lo = lo;
}

TabularPoissonSupport::~TabularPoissonSupport()
{

}

/**
 * Returns the integrals over x = [lo,hi] for following integrands:
 * i0 -> Exp[-x * t - lnc] * (x * t)^k * x^-n
 * i1 -> x * Exp[-x * t - lnc] * (x * t)^k * x^-n
 * i2 -> x^2 * Exp[-x * t - lnc] * (x * t)^k * x^-n
 * where lnc is computed as gammaln(1 + k - n)
 * These values can be used to compute the mean and variance for the following 
 * updated distributions:
 *  - uniform-poisson
 *  - loguniform-poisson
 *  - tabular-poisson
 *  - logtabular-poisson
 * Parameter must be either 0 or 1.
 * TEST NOTE: implementation is suspect. Check against Java implementation
 * and revalidate. Particularly second case...
 */
int TabularPoissonSupport::computeIntegrals(double &i0, double &i1, double &i2, double & lnc) const
{
	double a = 1 + k - n;
	
	if (k == 0 && n == 1) { 
		lnc = 0;
		i0 = SLATECInterface::exponentialIntegralEi(-hi * t) - loi0;
		i1 = (-exp(-hi * t) + exp(-lo * t)) / t;
		i2 = (exp(-hi * t) * (-1 - hi * t) - exp(-lo * t) * (-1 - lo * t)) / (t * t);
	} else {
		double b = 1 / t;
		double bn1 = pow(b,1 - n);
		lnc = this->lnc;
		//double i0x = bn1 * (DCDFLIBInterface::computeGammaCDF(a,t,hi)) - loi0;
		i0 = bn1 * DCDFLIBInterface::computeDeltaGammaCDF(a,t,lo,hi);
		//double i1x = bn1 * a * b * (DCDFLIBInterface::computeGammaCDF(a + 1,t,hi) - DCDFLIBInterface::computeGammaCDF(a + 1,t,lo));
		i1 = bn1 * a * b * DCDFLIBInterface::computeDeltaGammaCDF(a+1,t,lo,hi);
		//double i2x = bn1 * a * (a + 1) * b * b * (DCDFLIBInterface::computeGammaCDF(a + 2,t,hi) - DCDFLIBInterface::computeGammaCDF(a + 2,t,lo));
		i2 = bn1 * a * (a + 1) * b * b * DCDFLIBInterface::computeDeltaGammaCDF(a + 2,t,lo,hi);
	}
	return 0;
}

/**
 * Same as i0 in computeIntegrals
 * TEST STATUS: This function has been checked for correctness against Mathematica.
 * No large scale stability tests have been performed, but initial indications are
 * good.
 */
int TabularPoissonSupport::computeIntegral(double x, double &i0, double &lnc) const
{
	double a = 1 + k - n;
	
	if (k == 0 && n == 1) { 
		lnc = 0;
		i0 = SLATECInterface::exponentialIntegralEi(-x * t) - loi0;
	} else {
		double b = 1 / t;
		double bn1 = pow(b,1 - n);
		lnc = this->lnc;
		//double i0x = bn1 * (DCDFLIBInterface::computeGammaCDF(a,t,x)) - loi0;
		i0 = bn1 * DCDFLIBInterface::computeDeltaGammaCDF(a,t,lo,x);
	}
	return 0;
}

int TabularPoissonSupport::computeLogIntegrant(double x, double &i, double &lnc) const
{
	double a = 1 + k - n;
	
	if (k == 0 && n == 1) { 
		lnc = 0;
		i = -x * t - log(x);
	} else {
		lnc = this->lnc; // DCDFLIBInterface::computeLnGamma(a);
		double b = 1 / t;
		double gamma_ln_pdfx = GammaDistribution(1/t,a).getLogPDF(x);
		double gamma_ln_pdf = -a * log(b) - lnc + (a - 1) * log(x) - (x * t);
		i = (1 - n) * log(b) + gamma_ln_pdf;
	}
	return 0;
}

/**
 * Returns the value of x for which the integral over [lo,x'] of
 *  Exp[-x * t - lnc] * (x * t)^k * x^-n equals i0
 * where lnc = gammaln(1 + k - n)
 */
int TabularPoissonSupport::inverseIntegral(double i0, double &x) const
{
	double a = 1 + k - n;
	if (k == 0 && n == 1) {
		double y = i0 + loi0;
		x = -SLATECInterface::invExponentialIntegralEi(y,-hi * t) / t;
		if (x > hi) x = hi;
	} else {
		double b = 1 / t;
		double bn1 = pow(b,1 - n);
 		double z = (i0 + loi0) / bn1;
		x = DCDFLIBInterface::computeGammaQuantile(a,t,z);
	}
	return 0;
}


