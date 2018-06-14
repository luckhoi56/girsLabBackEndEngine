// TabularNormalIntegrator.cpp: implementation of the TabularNormalIntegrator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TabularNormalIntegrator.h"
#include <math.h>
#include "DCDFLIBInterface.h"
#include "NormalDistribution.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TabularNormalIntegrator::TabularNormalIntegrator(double nu,double tau,int n,double lo,double hi)
{
	this->nu = nu;
	this->tau = tau;
	this->n = n;
	this->lo = lo;
	this->hi = hi;
	this->loi0 = 0;

	if (lo < nu)
		this->lo = lo - 8 * tau;
	else
		this->lo = nu - 8 * tau;

	double loi0_tmp = 0;
	computeIntegral(lo,loi0_tmp,lnc);
	loi0 = loi0_tmp;
	this->lo = lo;
}

TabularNormalIntegrator::~TabularNormalIntegrator()
{

}


/**
 * Returns the integral at x.
 */
int TabularNormalIntegrator::computeIntegral(double x, double & i0, double & lnc) const
{
	lnc = 0;

	if (n == 1) {
		
	} else { // n == 0
		//double i0x = DCDFLIBInterface::computeErf((x - nu) / sqrt(2) / tau) / 2;
		//i0x -= loi0;
		const double twosqrt = sqrt((double)2);
		double deltaErf = DCDFLIBInterface::computeDeltaErf((lo - nu) / twosqrt / tau,(x - nu) / twosqrt / tau);
		i0 = deltaErf / 2;
	}

	return 0;
}


int TabularNormalIntegrator::computeLogIntegrant(double x, double & i, double & lnc) const
{
	lnc = 0;
	const double lnnorm = 0.12078223763524522235;

	if (n == 1) {
		return -1;
	} else { // n == 0
		double pdf = NormalDistribution(nu,tau).getPDF(x);
		i = log(pdf);
	}

	return 0;
}



/**
 * Returns the value of x at which the the integral reaches the specified value i0
 */
int TabularNormalIntegrator::inverseIntegral(double i0, double & x) const
{
	if (n == 1) {
		// NOT IMPLEMENTED
	} else { // n == 0
		double inverf = 
			DCDFLIBInterface::computeInverseErf(2 * (i0 + loi0) - 1);
		x = nu + sqrt((double)2) * tau * inverf;
	}
	return 0;
}

/**
 * Returns the integrals of:
 *  i0 : x^-n * PDF[NormalDistribution]
 *  i1 : x^1-n * PDF[NormalDistribution]
 *  i2 : x^2-n * PDF[NormalDistribution]
 * n is 1 for logdensity, and 0 for lindensity
 *
 * NOTE: logdensity mode is currently NOT implemented.
 */
int TabularNormalIntegrator::computeIntegrals(double & i0, double & i1, double & i2, double & lnc) const
{
	lnc = 0;
	
	double i0_temp, i1_temp, i2_temp;

	double lnhi = log(hi);
	double lnlo = log(lo);
	double tausq = pow(tau,2);
	const double twosqrt = sqrt((double)2);
	const double twopisqrt = sqrt((double)2 * 3.141592653589793);

	double deltaErf = DCDFLIBInterface::computeDeltaErf((lo - nu) / twosqrt / tau,(hi - nu) / twosqrt / tau);

	// integral of normal distribution
//	i0_temp = DCDFLIBInterface::computeErf((hi - nu) / twosqrt / tau)  - 
//		      DCDFLIBInterface::computeErf((lo - nu) / twosqrt / tau);
	i0_temp = deltaErf;
	i0_temp *= 1. / 2;

	// integral of normal distribution * x
	i1_temp = -tau * exp(-pow((hi-nu)/tau,2)/2) / twopisqrt + 
			   tau * exp(-pow((lo-nu)/tau,2)/2) / twopisqrt + 
//			  nu * DCDFLIBInterface::computeErf((hi - nu) / twosqrt / tau) / 2 -
//			  nu * DCDFLIBInterface::computeErf((lo - nu) / twosqrt / tau) / 2;
			nu * deltaErf / 2;

	if (n == 1) {
		// NOT IMPLEMENTED
		i0 = 0;
		i1 = 0;
		i2 = 0;
	} else { // n == 0
		i2_temp = exp((-lo*lo + 2*lo*nu - nu*nu)/tausq/2) * (lo + nu) * tau / twopisqrt 
			      - exp((-hi*hi + 2*hi*nu - nu*nu)/tausq/2) * (hi + nu) * tau / twopisqrt
				  // - (nu * nu + tausq) * DCDFLIBInterface::computeErf((lo - nu) / twosqrt / tau) / 2
				  // + (nu * nu + tausq) * DCDFLIBInterface::computeErf((hi - nu) / twosqrt / tau) / 2;
				  + (nu * nu + tausq) * deltaErf / 2;

		i0 = i0_temp;
		i1 = i1_temp;
		i2 = i2_temp;
	}
	return 0;
}
