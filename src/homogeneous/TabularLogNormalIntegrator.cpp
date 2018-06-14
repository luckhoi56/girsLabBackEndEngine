// $Header: C:/cvsrepo/ptdev/native/redcas/homogeneous/TabularLogNormalIntegrator.cpp,v 1.5 2002/07/08 00:23:05 fgroen Exp $
//
// HISTORY:
//
// 7/6/02 (FJG): Modified the density function by multiplying it by x. The purpose
//       of this modification is a correction for the lognormal updating: a tabular
//       distribution updated by a lognormal is NOT a lognormal, but instead of the
//       form Lognormal * x.
//
// TabularLogNormalIntegrator.cpp: implementation of the TabularLogNormalIntegrator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TabularLogNormalIntegrator.h"
#include <math.h>
#include "DCDFLIBInterface.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TabularLogNormalIntegrator::TabularLogNormalIntegrator(double nu,double tau,int n,double lo,double hi)
{
	this->nu = nu;
	this->tau = tau;
	this->n = n;
	this->hi = hi;
	this->lo = lo;
}

TabularLogNormalIntegrator::~TabularLogNormalIntegrator()
{

}

/**
 * Returns the integral at x.
 */
int TabularLogNormalIntegrator::computeIntegral(double x, double & i0, double & lnc) const
{

	if (x == lo) return 0;

	lnc = 0;

	// different routines are used to compute the integral depending on:
	//  - whether the tabular density is linear (uniform) or logarithmic (exponential) density
	//  - whether the interval is in the lower or upper tail of the distribution:
	//    avoid subtraction of large numbers, at the cost of extra erf(x) computation
	//  - whether the lower bound is 0 or greater than 0:
	//    log(lo) can not be computed, but erf(lo) is known to be -1

	double lnhi = (x > 0) ? log(x) : 0;
	double lnlo = (lo > 0) ? log(lo) : 0;
	double tausq = pow(tau,2);
	const double twosqrt = sqrt((double)2);

	// 7/6/02 FJG: add x to integrand. Lognormal correction
	if (n == 1) { // log-tabular
		double hi_x0 = (-nu + lnhi) / twosqrt / tau;
		double lo_x0 = (-nu + lnlo) / twosqrt / tau;
		if (lo > 0) {
			i0 = DCDFLIBInterface::computeDeltaErf(lo_x0,hi_x0) / 2;
		} else {
			i0 = DCDFLIBInterface::computeErfc(-hi_x0) / 2;
		}
	} else { // tabular
		// compute integral of x * LN(x|nu,tau)
		double lo_x1 = (nu + tausq - lnhi) / twosqrt / tau;
		if (lo > 0) {
			double hi_x1 = (nu + tausq - lnlo) / twosqrt / tau;
			i0 = -DCDFLIBInterface::computeDeltaErf(lo_x1,hi_x1);
		} else {
			i0 = -DCDFLIBInterface::computeErfc(lo_x1);
		}
		i0 *= -exp(nu + tausq / 2) / 2;
	}

	return 0;
}

int TabularLogNormalIntegrator::computeLogIntegrant(double x, double & i, double & lnc) const
{
	lnc = 0;
	const double lnnorm = 0.12078223763524522235;
	// 7/6/02 FJG: add x to integrand. Lognormal correction
	if (n == 1) {
		i= log(DCDFLIBInterface::computeLognormalPDF(nu,tau,x));
	} else { // n == 0
		i= log(DCDFLIBInterface::computeLognormalPDF(nu,tau,x) * x);
	}
	return 0;
}

/**
 * Returns the value of x at which the the integral reaches the specified value i0
 */
int TabularLogNormalIntegrator::inverseIntegral(double i0, double & x) const
{
	double loi0,y;
	double lnlo = (lo > 0) ? log(lo) : 0;
	double tausq = pow(tau,2);
	const double twosqrt = sqrt((double)2);

	// 7/6/02 FJG: Lognormal correction.
	if (n == 1) {
		if (lo > 0) {
			y = (-nu + lnlo) / twosqrt / tau;
			loi0 = DCDFLIBInterface::computeErf(y) / 2;
		} else loi0 = -0.5;
		double inverf =
			DCDFLIBInterface::computeInverseErf(2 * (i0 + loi0));
		double lnx = nu + sqrt((double)2) * tau * inverf;
		x = exp(lnx);
	} else {
		double erflo = 0;
		if (lo > 0) {
			y = (nu + tausq - lnlo) / twosqrt / tau;
			erflo = DCDFLIBInterface::computeErf(y);
		} else erflo = 1;

		double erfhi = erflo - 2 * i0 / exp(nu + tausq / 2);
		double inverf = DCDFLIBInterface::computeInverseErf(erfhi);
		double lnx = nu + tausq - twosqrt * tau * inverf;
		x = exp(lnx);
	}
	return 0;
}

/**
 * Returns the integrals of:
 *  i0 : x^1-n * PDF[LogNormalDistribution]
 *  i1 : x^2-n * PDF[LogNormalDistribution]
 *  i2 : x^3-n * PDF[LogNormalDistribution]
 * n is 1 for logdensity, and 0 for lindensity
 *
 * Extra x term is part of the lognormal correction (7/6/02)
 */
int TabularLogNormalIntegrator::computeIntegrals(double & i0, double & i1, double & i2, double & lnc) const
{
	lnc = 0;

	// different routines are used to compute the integral depending on:
	//  - whether the tabular density is linear (uniform) or logarithmic (exponential) density
	//  - whether the interval is in the lower or upper tail of the distribution:
	//    avoid subtraction of large numbers, at the cost of extra erf(x) computation
	//  - whether the lower bound is 0 or greater than 0:
	//    log(lo) can not be computed, but erf(lo) is known to be -1	

	double im1_temp, i0_temp, i1_temp, i2_temp;

	double lnhi = (hi > 0) ? log(hi) : 0;
	double lnlo = (lo > 0) ? log(lo) : 0;
	double tausq = pow(tau,2);
	const double twosqrt = sqrt((double)2);


	if (n == 1) {
		// integral of lognormal distribution
		double hi_xm1 = (-nu + lnhi) / twosqrt / tau;
		if (lo > 0) {
			double lo_xm1 = (-nu + lnlo) / twosqrt / tau;
			im1_temp = DCDFLIBInterface::computeDeltaErf(lo_xm1,hi_xm1);
		} else {
			im1_temp = DCDFLIBInterface::computeErfc(-hi_xm1);
		}
		im1_temp /= 2;
	}

	// integral of lognormal distribution * x
	double lo_x0 = (nu + tausq - lnhi) / twosqrt / tau;
	if (lo > 0) {
		double hi_x0 = (nu + tausq - lnlo) / twosqrt / tau;
		i0_temp = -DCDFLIBInterface::computeDeltaErf(lo_x0,hi_x0);
	} else {
		i0_temp = -DCDFLIBInterface::computeErfc(lo_x0);
	}
	i0_temp *= -exp(nu + tausq / 2) / 2;

	// integral of lognormal distribution * x^2
	double lo_x1 = (nu + 2 * tausq - lnhi) / twosqrt / tau;
	if (lo > 0) {
		double hi_x1 = (nu + 2 * tausq - lnlo) / twosqrt / tau;
		i1_temp = -DCDFLIBInterface::computeDeltaErf(lo_x1,hi_x1);
	} else {
		i1_temp = -DCDFLIBInterface::computeErfc(lo_x1);
	}
	i1_temp *= -exp(2 * nu + 2 * tausq) / 2;

	if (n == 0) {
		// integral of lognormal distribution * x^2
		double lo_x2 = (nu + 3 * tausq - lnhi) / twosqrt / tau;
		if (lo > 0) {
			double hi_x2 = (nu + 3 * tausq - lnlo) / twosqrt / tau;
			i2_temp = -DCDFLIBInterface::computeDeltaErf(lo_x2,hi_x2);
		} else {
			i2_temp = -DCDFLIBInterface::computeErfc(lo_x2);
		}
		i2_temp *= -exp(3 * nu + 4.5 * tausq) / 2;
	}

	if (n == 1) {
		i0 = im1_temp;
		i1 = i0_temp;
		i2 = i1_temp;
	} else {
		i0 = i0_temp;
		i1 = i1_temp;
		i2 = i2_temp;
	}

	return 0;
}
