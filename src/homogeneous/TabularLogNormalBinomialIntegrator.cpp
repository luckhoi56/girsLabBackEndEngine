// $Header: C:/cvsrepo/ptdev/native/redcas/homogeneous/TabularLogNormalBinomialIntegrator.cpp,v 1.9 2004/02/05 19:25:01 fgroen Exp $
//
// HISTORY:
//
// 7/6/02 (FJG): Modified the density function by multiplying it by x. The purpose
//       of this modification is a correction for the lognormal updating: a tabular
//       distribution updated by a lognormal is NOT a lognormal, but instead of the
//       form Lognormal * x.
// 12/18/03 (FJG): Modified determineBounds to solve singularity exception that occurred 
//       when called from Java.
//
// TabularLogNormalBinomialIntegrator.cpp: implementation of the TabularLogNormalBinomialIntegrator class.
//
//////////////////////////////////////////////////////////////////////


#include "TabularLogNormalBinomialIntegrator.h"
#include "../global/Function.h"
#include "../global/SimpsonIntegrator.h"

#include <math.h>
#include <iostream>

/**
 * Function for use by Simpson Integrator. This function evaluates the
 * integrand of the function, normalized by a factor exp(lnnorm).
 */
class TabLNBFunction : public Function
{
public:
	virtual double evaluate(const Parameters & params) const;
	virtual int getDimension() const {return 1;}
	void setOrder(int o) {this->o = o;}
	TabLNBFunction(double nu, double tau, double k, double n, double lnnorm, int m);
	virtual ~TabLNBFunction() {}
private:
	int o;				// integration order
	double lnnorm;
	double a;
	double b;
	double nu;
	double tau;
};

/**
 * Constructor.
 */
TabLNBFunction::TabLNBFunction(double nu, double tau, double k, double n, double lnnorm, int m)
{
	this->nu = nu;
	this->tau = tau;
	// 7/6/02,FJG: increased a by 1 (lognormal update correction)
	this->a = k + 1 - m + 1;
	this->b = n - k + 1;
	this->lnnorm = lnnorm;
	this->o = 0;
}

/**
 * Returns the integrand normalized by factor lnnorm. This integrand must correspond
 * to exp(TabularLogNormalBinomial::getLogPDF(x)).
 */
double TabLNBFunction::evaluate(const Parameters & params) const
{
	double x = params.getValue(0);

	if (x == 0 && a + o >= 2) return 0;
	if (x == 1 && b >= 1) return 0;

	if (b < 1 && x > 0.99999999) {
		const double eps = 0.00000001;
		double y = 1 - x;
		x = 1 - y * y / eps / 2 - eps / 2;
	} if (a + o < 2 && x < 1e-100) {
		const double eps = 1e-100;
		x = x * x / eps / 2 + eps / 2;
	}

	double lnx = log(x);
	double lnomx = x > 0.001 ? log(1 - x) : -x*(1+x*(1+2*x*(1+3*x*(1+4*x*(1+5*x/6)/5)/4)/3)/2);
	double lndensity = (a + o - 2) * lnx + (b - 1) * lnomx - (pow((lnx - nu) / tau,2) / 2);
	double lnf = lndensity - lnnorm;
	return exp(lnf);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Function that is negative left of mode, positive right of mode.

const int TLNBI_MODE_INSIDE = 1;	// mode lies inside interval
const int TLNBI_MODE_LEFT = 2;		// mode lies left of interval
const int TLNBI_MODE_RIGHT = 3;		// mode lies right of interval

/**
 * Constructor.
 */
TabularLogNormalBinomialIntegrator::TabularLogNormalBinomialIntegrator(double lo, double hi, double nu, double tau, double k, double n, int m, double lnnorm)
{
	this->hi = hi;
	this->lo = lo;
	this->nu = nu;
	this->tau = tau;
	this->k = k;
	this->n = n;
	this->m = m;
	this->lnnorm = lnnorm;

	this->mode = findRestrictedMode(this->mode_loc);
	lo_bound = lo;
	hi_bound = hi;
	determineBounds(lo_bound,hi_bound);

	double lnnorm_tmp,lnc;
	computeLogIntegrant(this->mode,lnnorm_tmp,lnc);
	this->delta = lnnorm_tmp;
}

/**
 * Constructor.
 */
TabularLogNormalBinomialIntegrator::TabularLogNormalBinomialIntegrator(double lo, double hi, double nu, double tau, double k, double n, int m)
{
	this->hi = hi;
	this->lo = lo;
	this->nu = nu;
	this->tau = tau;
	this->k = k;
	this->n = n;
	this->m = m;
	this->lnnorm = 0;

	this->mode = findRestrictedMode(this->mode_loc);

	lo_bound = lo;
	hi_bound = hi;
	determineBounds(lo_bound,hi_bound);

	double lnnorm_tmp,lnc;
	computeLogIntegrant(this->mode,lnnorm_tmp,lnc);
	this->lnnorm = lnnorm_tmp;
	this->delta = 0;
}

TabularLogNormalBinomialIntegrator::~TabularLogNormalBinomialIntegrator()
{

}

/**
 * Computes the integral at x. Integral is returned through i0 parameter. lnc is the
 * normalization factor that is applied. See also inverseIntegral().
 */
int TabularLogNormalBinomialIntegrator::computeIntegral(double x, double & i0, double & lnc) const
{
	if (x < lo_bound) {
		i0 = 0;
		lnc = lnnorm;
		return 0;
	} else if (x > hi_bound) {
		x = hi_bound;
	}

	SimpsonIntegrator simpson;
	TabLNBFunction f(nu,tau,k,n,lnnorm + delta,m);
	f.setOrder(0);
	i0 = simpson.simpson145(lo,x,f,0.000001,120);
	//double i0x = simpson.simpson145(lo,x,f,0.000001,120);
	i0 *= exp(delta);
	lnc = lnnorm;
	return 0;
}

/**
 * Computes the value of x for which the integral equals x. Inverse of 
 * computeIntegral().
 */
int TabularLogNormalBinomialIntegrator::inverseIntegral(double i0, double & x) const
{
	return inverseSimpsonIntegral(i0,x);
}

/**
 * Computes the zero-th, first, and second order integrals.
 */
int TabularLogNormalBinomialIntegrator::computeIntegrals(double & i0, double & i1, double & i2, double & lnc) const
{
	TabLNBFunction f(nu,tau,k,n,lnnorm + delta,m);
	SimpsonIntegrator simpson;

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
 * Computes the integrand at x.
 */
int TabularLogNormalBinomialIntegrator::computeLogIntegrant(double x, double &p, double &lnc) const
{
	lnc = lnnorm;

//	if (x == 1) cout << "REQUEST FOR TABLOGBIN::PDF(1)" << endl;

	if (x <= 0 || x >= 1) {
		p = -3000 - lnc;
//		if (x == 1) cout << "RETURNING: " << p << endl;
		return 0;
	}

	// 7/6/02: increased a by 1 (lognormal correction)
	double a = k + 1 - m + 1;
	double b = n - k + 1;

	// avoid x == 0 and x == 1 when pdf goes to infinity...
	if (b < 1 && x > 0.99999999) {
		const double eps = 0.00000001;
		double y = 1 - x;
		x = 1 - y * y / eps / 2 - eps / 2;
	} else if (a < 2 && x < 1e-100) {
		const double eps = 1e-100;
		x = x * x / eps / 2 + eps / 2;
	}

	double lnx = log(x);

	double lnomx = x > 0.001 ? log(1 - x) : -x*(1+x*(1+2*x*(1+3*x*(1+4*x*(1+5*x/6)/5)/4)/3)/2);
	double lndensity = (a - 2) * lnx + (b - 1) * lnomx - (pow((lnx - nu) / tau,2) / 2);
	p = lndensity - lnnorm;
	lnc = lnnorm;
	return 0;
}

/**
 * Returns the location of the extreme in the interval.
 */
double TabularLogNormalBinomialIntegrator::findRestrictedMode() const
{
	int location;
	return findRestrictedMode(location);
}

/**
 * Returns the location of the extreme in the interval. Also indicates whether
 * the mode is located to the left, right, or within the interval.
 */
double TabularLogNormalBinomialIntegrator::findRestrictedMode(int & location) const
{
	double x;
	
	if (isLessThanMode(hi)) {
		x = hi;
		location = TLNBI_MODE_RIGHT;
	} else if (!isLessThanMode(lo)) {
		x = lo;
		location = TLNBI_MODE_LEFT;
	} else {
		x = findMode();
		location = TLNBI_MODE_INSIDE;
	}
	return x;
}

/**
 * Returns the location of the extreme of the density, regardless of whether it is 
 * inside or outside the interval.
 */
double TabularLogNormalBinomialIntegrator::findMode() const 
{
 // (nu (-1 + x) + tau^2 (2 + alpha (-1 + x) + (-3 + beta) x) - (-1 + x) Log[x]) == 0
 // provides condition. function is strictly monotonically increasing.
 // Mode location is found by means of a bracket search.

	double x,lnx;
//	double a = k + 1;
//	double b = n - k + 1;

	// establish bracket
	double i = 0;
	lnx = nu + i * tau;
	x = exp(lnx);
	while (isLessThanMode(x) && x < 1) {
		i++;
		lnx = nu + i * tau;
		x = exp(lnx);
	}
	while (!isLessThanMode(x) || x > 1) {
		i--;
		lnx = nu + i * tau;
		x = exp(lnx);
	}

	// bracket search
	double lox = exp(nu + i * tau);
	double hix = exp(nu + (i + 1) * tau);
	if (hix > 1) hix = 1;
	while (hix / lox > 1.00001) {
		x = sqrt(hix * lox);
		if (isLessThanMode(x)) {
			lox = x;
		} else {
			hix = x;
		}
	}
	return x;
}

/**
 * Returns the value of x for which the integral equals z. Returns -1 if the 
 * search was stopped only after the meximum number of iterations was reached.
 */
int TabularLogNormalBinomialIntegrator::inverseSimpsonIntegral(double z, double & x) const
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
 * Returns true if x is to the left of the mode.
 */
bool TabularLogNormalBinomialIntegrator::isLessThanMode(double x) const
{
// (nu (-1 + x) + tau^2 (2 + alpha (-1 + x) + (-3 + beta) x) - (-1 + x) Log[x]) == 0
	if (x == 0) return true;
	double a = k + 1 - m;
	double b = n - k + 1;
	return (nu * (x - 1) + pow(tau,2) * (2 + a * (x - 1) + (b - 3) * x) - (x - 1) * log(x)) < 0;
}

/**
 * Determines a set of integration bounds, based on the bounds provided by the user.
 * The integration bounds may be tighter than the original settings, in order to 
 * perform the integration only over the significant portion of the likelihood function.
 */
void TabularLogNormalBinomialIntegrator::determineBounds(double &lob, double &hib) const
{
	lob = lo;
	hib = hi;

	const double delta = 1e-25;
	const double ln_delta = log(delta);

	if (lob == 0) 
		lob = exp(nu - 12 * tau) * (k + 1) / (n * 1000);

	if (hib == 1)
		hib = 0.99999999999;

	// adjust only if mode lies within the interval
	if (mode > lo && mode < hi) {

		double lnmodeval,lnloval,lnhival,lnc;
		computeLogIntegrant(mode,lnmodeval,lnc);
		computeLogIntegrant(hib,lnhival,lnc);
		computeLogIntegrant(lob,lnloval,lnc);

		// adjust hi only if f(hi) is sufficiently small compared to f(mode).
		if (lnhival < lnmodeval - log(hi / mode) + ln_delta) {
			hib = intersectBound(mode,hib,mode,nu,tau,k,n,-1 + m,delta);
		}

		// adjust lo only if f(lo) is sufficiently small compared to f(mode).
		if (lo == 0 || lnloval < lnmodeval - log(lo / mode) + ln_delta) {
			lob = intersectBound(lob,mode,mode,nu,tau,k,n,-1 + m,delta);
		}
	}

//	cout << "Bounds set to " << lob << " and " << hib << endl;
}

/**
 * Returns the approximate intersection of the likelihood function with the line
 * ln(x/mode) + ln(f(mode)) - lndelta.
 */
double TabularLogNormalBinomialIntegrator::intersectBound(double lo, double hi, double mode, double nu, double tau, double k, double n, int order, double delta) const
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

	// 7/6/02: increased a by 1 (lognormal correction)
	double a = k + 1 - m + 1;
	double b = n - k + 1;

	double c1 = -pow(tau,-2) / 2;
	double c2 = b - 1;
	double c3 = a - 2 + nu * pow(tau,-2);
	double c4 = - c1 * pow(lnmode,2) - c2 * log(1 - mode) - c3 * lnmode  - lndelta;

	int iterations = 0;
	int max_iterations = 100;
	double epsilon = log(1.01);

	double loval = c1 * pow(lnlo,2) + c2 * log(1 - exp(lnlo)) + c3 * lnlo + c4;
	double hival = c1 * pow(lnhi,2) + c2 * log(1 - exp(lnhi)) + c3 * lnhi + c4;
	double lnx;

	while (lnhi - lnlo > epsilon) {
		
/*
		if (inc) {
			// linear interpolation.
			lnx = lnlo + (lnhi - lnlo) * (-loval) / (hival - loval);
		} else {
			// linear interpolation on the inverse.
			double lnhi_inv = 1 / lnhi;
			double lnlo_inv = 1 / lnlo;
			double lnx_inv = lnhi_inv + (lnlo_inv - lnhi_inv) * (-hival) / (loval - hival);
			lnx = 1 / lnx_inv;
		}
*/

		lnx = (lnlo + lnhi) / 2;

		double x = exp(lnx);
		double y = c1 * pow(lnx,2) + c2 * log(1 - x) + c3 * lnx + c4;

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
