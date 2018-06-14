// $Header: C:/cvsrepo/ptdev/native/redcas/homogeneous/TabularLogNormalPoissonIntegrator.cpp,v 1.8 2004/02/05 19:25:01 fgroen Exp $
//
// HISTORY:
//
// 7/6/02 (FJG): Modified the density function by multiplying it by x. The purpose
//       of this modification is a correction for the lognormal updating: a tabular
//       distribution updated by a lognormal is NOT a lognormal, but instead of the
//       form Lognormal * x.
// 12/18/03 (FJG): Modified determineBounds to solve singularity exception that occurred 
//       when called from Java.
// TabularLogNormalPoissonIntegrator.cpp: implementation of the TabularLogNormalPoissonIntegrator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TabularLogNormalPoissonIntegrator.h"
#include <math.h>

#include "Function.h"
#include "SimpsonIntegrator.h"

#include <iostream>

class TabLNPFunction : public Function
{
public:
	virtual double evaluate(const Parameters & params) const;
	virtual int getDimension() const {return 1;}
	void setOrder(int n) {this->n = n;}
	TabLNPFunction(double nu, double tau, double k, double t, double lnnorm, int m);
	virtual ~TabLNPFunction() {}
private:
	int m;
	int n;
	double lnnorm;
	double a;
	double b;
	double nu;
	double tau;
};

TabLNPFunction::TabLNPFunction(double nu, double tau, double k, double t, double lnnorm, int m)
{
	this->nu = nu;
	this->tau = tau;
	// 7/6/02 FJG: lognormal correction: increase a by 1.
	this->a = k + 1 + 1;
	this->b = 1 / t;
	this->lnnorm = lnnorm;
	this->m = m;
	this->n = 0;
}

double TabLNPFunction::evaluate(const Parameters & params) const
{
	double x = params.getValue(0);
	if (x == 0) return 0;
	double lnx = log(x);
	double lndensity = - (pow((lnx - nu) / tau,2) / 2) - (x / b) + (a - 2 - m + n) * lnx;
	double lnf = lndensity - lnnorm;
	return exp(lnf);
}

//////////////////////////////////////////////////////////////////////////

class TabLNPLNFunction : public Function
{
public:
	virtual double evaluate(const Parameters & params) const;
	virtual int getDimension() const {return 1;}
	void setOrder(int n) {this->n = n;}
	TabLNPLNFunction(double nu, double tau, double k, double t, double lnnorm, int m);
	virtual ~TabLNPLNFunction() {}
private:
	int m;
	int n;
	double lnnorm;
	double a;
	double b;
	double nu;
	double tau;
};

TabLNPLNFunction::TabLNPLNFunction(double nu, double tau, double k, double t, double lnnorm, int m)
{
	this->nu = nu;
	this->tau = tau;
	// 7/6/02 FJG: lognormal correction: increase a by 1.
	this->a = k + 1 + 1;
	this->b = 1 / t;
	this->lnnorm = lnnorm;
	this->m = m;
	this->n = 0;
}

double TabLNPLNFunction::evaluate(const Parameters & params) const
{
	double lnx = params.getValue(0);
	double x = exp(lnx);
	double lndensity = - (pow((lnx - nu) / tau,2) / 2) - (x / b) + (a - 2 - m + n + 1) * lnx;
	double lnf = lndensity - lnnorm;
	return exp(lnf);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const int TLNPI_MODE_INSIDE = 1;	// mode lies inside interval
const int TLNPI_MODE_LEFT = 2;		// mode lies left of interval
const int TLNPI_MODE_RIGHT = 3;		// mode lies right of interval

TabularLogNormalPoissonIntegrator::TabularLogNormalPoissonIntegrator(double lo, double hi, double nu, double tau, double k, double t, int m, double lnnorm)
{

	this->hi = hi;
	this->lo = lo;
	this->nu = nu;
	this->tau = tau;
	this->k = k;
	this->t = t;
	this->m = m;
	this->lnnorm = lnnorm;

	this->mode = findRestrictedMode(this->mode_loc);
	lo_bound = lo;
	hi_bound = hi;
	determineBounds(lo_bound,hi_bound);
	double lnnorm_tmp,lnc;
	computeLogIntegrant(mode,lnnorm_tmp,lnc);
	this->delta = lnnorm_tmp;
}


TabularLogNormalPoissonIntegrator::TabularLogNormalPoissonIntegrator(double lo, double hi, double nu, double tau, double k, double t, int m)
{
	this->hi = hi;
	this->lo = lo;
	this->nu = nu;
	this->tau = tau;
	this->k = k;
	this->t = t;
	this->m = m;
	this->lnnorm = 0;

	this->mode = findRestrictedMode(this->mode_loc);
	lo_bound = lo;
	hi_bound = hi;
	determineBounds(lo_bound,hi_bound);
	double lnnorm_tmp,lnc;
	computeLogIntegrant(mode,lnnorm_tmp,lnc);
	this->lnnorm = lnnorm_tmp;
	this->delta = 0;
}


TabularLogNormalPoissonIntegrator::~TabularLogNormalPoissonIntegrator()
{

}

/**
 * Computes the integral at x. See also inverseIntegral().
 */
int TabularLogNormalPoissonIntegrator::computeIntegral(double x, double & i0, double & lnc) const
{
	if (x < lo_bound) {
		i0 = 0;
		lnc = lnnorm;
		return 0;
	} else if (x > hi_bound) {
		x = hi_bound;
	}

	SimpsonIntegrator simpson;
	TabLNPFunction f(nu,tau,k,t,lnnorm + delta,m);
	f.setOrder(0);
	i0 = simpson.simpson145(lo,x,f,0.00001,100);
	i0 *= exp(delta);
	lnc = lnnorm;
	return 0;
}

/**
 * Finds the value of x for the integral equals i0.
 * See also computeIntegral().
 */
int TabularLogNormalPoissonIntegrator::inverseIntegral(double i0, double & x) const
{
	return inverseSimpsonIntegral(i0,x);
}

/**
 * Computes the 0th, first and second order integral, used to compute integral, mean and variance.
 * Returns 0 if the computations were successful.
 */
int TabularLogNormalPoissonIntegrator::computeIntegrals(double & i0, double & i1, double & i2, double & lnc) const
{
	SimpsonIntegrator simpson;
	TabLNPFunction f(nu,tau,k,t,lnnorm + delta,m);
	double expdelta = exp(delta);
	f.setOrder(0);
	i0 = simpson.simpson145(lo_bound,hi_bound,f,0.00001,100) * expdelta;
	f.setOrder(1);
	i1 = simpson.simpson145(lo_bound,hi_bound,f,0.00001,100) * expdelta;
	f.setOrder(2);
	i2 = simpson.simpson145(lo_bound,hi_bound,f,0.00001,100) * expdelta;
	lnc = lnnorm;
	return 0;
}

/**
 * Returns the natural log of the integrand at x.
 */
int TabularLogNormalPoissonIntegrator::computeLogIntegrant(double x, double &p, double &lnc) const
{
	double lnx = log(x);
	// 7/6/02 FJG lognormal correction
	double a = k + 1 + 1;
	double b = 1 / t;
	double lndensity = - (pow((lnx - nu) / tau,2) / 2) - (x / b) + (a - 2 - m) * lnx;
	p = lndensity - lnnorm;
	lnc = lnnorm;
	return 0;
}

/**
 * Find the mode of the density function, without regards of the 
 * lower and higher bounds.
 */
double TabularLogNormalPoissonIntegrator::findMode() const
{
	// 7/6/02 FJG lognormal correction
	double a = k + 1 + 1;
	double b = 1 / t;
	double tausq = pow(tau,2);
	double z = tausq * exp(nu - (2 + m - a) * tausq) / b;
	double productlog = computeProductLog(z);
	return b * productlog / tausq;
}

/**
 * Find the mode of the density function, taking into account the
 * lower and higher bounds.
 */
double TabularLogNormalPoissonIntegrator::findRestrictedMode() const
{
	int location;
	return findRestrictedMode(location);
}

double TabularLogNormalPoissonIntegrator::findRestrictedMode(int & location) const
{
	double x;
	
	if (isLessThanMode(hi)) {
		x = hi;
		location = TLNPI_MODE_RIGHT;
	} else if (!isLessThanMode(lo)) {
		x = lo;
		location = TLNPI_MODE_LEFT;
	} else {
		x = findMode();
		location = TLNPI_MODE_INSIDE;
	}
	return x;
}

/**
 * Returns the productlog of z. The productlog is defined by Mathematica
 * as the solution of w*exp(w) = z. This function is only capable of 
 * returning results for z >= 0.
 */
double TabularLogNormalPoissonIntegrator::computeProductLog(double z)
{
	const int max_iterations = 50;

	if (z <= 0) return 0;

	double lnz = log(z);
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

	double lnw;
	while (lnwmax - lnwmin > 0.00001 && iterations < max_iterations) {
		lnw = (lnwmax + lnwmin) / 2;
		double lnwew = lnw + exp(lnw);
		if (lnwew < lnz) {
			lnwmin = lnw;
		} else {
			lnwmax = lnw;
		}
		iterations++;
	}
	return exp(lnw);
}

/**
 * Finds the value of x for which the simpson integrator returns z.
 */
int TabularLogNormalPoissonIntegrator::inverseSimpsonIntegral(double z, double & x) const
{
	double lnc;

	double hival = hi;
	double hicdf = 0; // cdf's are not real cdf's but unnormalized.
	computeIntegral(hival,hicdf,lnc);

	double loval = lo;
	double locdf = 0;

	double mdval = (loval == 0) ? hival / 10 : sqrt(hival * loval);
	double mdcdf = 0;
	double lncdf = 0;
	computeIntegral(mdval,mdcdf,lnc);

	int iterations = 0;
	const int max_iterations = 100;

	while ((loval == 0 || hival / loval > 1.000001) && iterations < max_iterations)
	{
		if (z > mdcdf) {
			loval = mdval;
			locdf = mdcdf;
		} else {
			hival = mdval;
			hicdf = mdcdf;
		}

		mdval = (loval == 0) ? hival / 10 : sqrt(hival * loval);
		computeIntegral(mdval,mdcdf,lnc);
	}

	x = mdval;
	return (iterations < max_iterations) ? 0 : -1;
}

/**
 * Returns true is less than the location of the mode.
 */
bool TabularLogNormalPoissonIntegrator::isLessThanMode(double x) const
{
	if (x == 0) return true;

	// 7/6/02 FJG lognormal correction
	double a = k + 1 + 1;
	double b = 1 / t;
	return (-b * (nu + (-2 + a - m) * pow(tau,2)) + pow(tau,2) * x + b * log(x)) < 0;
}

/**
 * Determines a set of integration bounds, based on the bounds provided by the user.
 * The integration bounds may be tighter than the original settings, in order to 
 * perform the integration only over the significant portion of the likelihood function.
 */
void TabularLogNormalPoissonIntegrator::determineBounds(double &lob, double &hib) const
{
	lob = lo;
	hib = hi;

	const double delta = 1e-25;
	const double ln_delta = log(delta);

	if (lob == 0) 
		lob = exp(nu - 12 * tau) * (k + 1) / (t * 1000);
	if (hib > exp(nu + 12 * tau) * (k + 1) / (t * 0.001))
		hib = exp(nu + 12 * tau) * (k + 1) / (t * 0.001);
		// adjust only if mode lies within the interval
	if (mode > lo && mode < hi) {
		double lnmodeval,lnloval,lnhival,lnc;
		computeLogIntegrant(mode,lnmodeval,lnc);
		computeLogIntegrant(hib,lnhival,lnc);
		computeLogIntegrant(lob,lnloval,lnc);
			// adjust hi only if f(hi) is sufficiently small compared to f(mode).
		if (lnhival < lnmodeval - log(hi / mode) + ln_delta) {
			hib = intersectBound(mode,hib,mode,nu,tau,k,t,0,delta);
			computeLogIntegrant(hib,lnhival,lnc);
		}
			// adjust lo only if f(lo) is sufficiently small compared to f(mode).
		if (lo == 0 || lnloval < lnmodeval - log(lo / mode) + ln_delta) {
			lob = intersectBound(lob,mode,mode,nu,tau,k,t,0,delta);
			computeLogIntegrant(lob,lnloval,lnc);
		}
	}
}

/**
 * Returns the value of x for which the (unnormalized) posterior intersects with
 * the line ln(x/mode) + ln(f(mode)) - lndelta. This function can be used to 
 * determine the integration interval for the posterior. The intersection point
 * is found by means of an bisection search.
 */
double TabularLogNormalPoissonIntegrator::intersectBound(double lo, double hi, double mode, double nu, double tau, double k, double t, int order, double delta) const
{
	//
	// intersection of the likelihood with the line ln(x/mode) + ln(f(mode)) - lndelta
	// intersection is located at x where (c1 lnx^2 + c2 Log[1 - x] + c3 lnx + c4) == 0
	//
	// for normal integral, order should be 0.
	// for mean type integral, order should be 1.
	// for variance type integral, order should be 2.
	//

	double lnlo = log(lo);
	double lnhi = log(hi);
	double lndelta = log(delta);
	double lnmode = log(mode);

	bool inc = hi <= mode; // flag: likelihood is increasing function on the interval

	// 7/6/02 lognormal correction
	double a = k + 1 + 1;
	double b = 1 / t;

	double c1 = -pow(tau,-2) / 2;
	double c2 = -t;
	double c3 = a - 2 + nu * pow(tau,-2);
	double c4 = - c1 * pow(lnmode,2) - c2 * mode - c3 * lnmode  - lndelta;

	int iterations = 0;
	int max_iterations = 100;
	double epsilon = log(1.01);

	double loval = c1 * pow(lnlo,2) + c2 * lo + c3 * lnlo + c4;
	double hival = c1 * pow(lnhi,2) + c2 * hi + c3 * lnhi + c4;
	double lnx;

	while (lnhi - lnlo > epsilon) {
		
		lnx = (lnlo + lnhi) / 2;

		double x = exp(lnx);
		double y = c1 * pow(lnx,2) + c2 * x + c3 * lnx + c4;

		if ((inc && y < 0) || (!inc && y > 0)) {
			// select higher bracket
			lnlo = lnx;
			loval = y;
		} else {
			// select lower bracket
			lnhi = lnx;
			hival = y;
		}

		iterations++;
	}
	return exp(lnx);
}

