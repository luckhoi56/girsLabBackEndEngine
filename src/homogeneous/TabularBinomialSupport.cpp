// TabularBinomialSupport.cpp: implementation of the TabularBinomialSupport class.
//
//////////////////////////////////////////////////////////////////////


#include "TabularBinomialSupport.h"
#include "../parametric/BetaDistribution.h"
#include "../global/SimpsonIntegrator.h"
#include "../global/Function.h"
#include "../dcdf/DCDFLIBInterface.h"

#include <math.h>
#include <iostream>

class KIsZeroFunction : public Function
{
public:
	virtual double evaluate(const Parameters & params) const;
	virtual int getDimension() const {return 1;}
	void setN(double n) {this->n = n;}
	KIsZeroFunction(double n) {this->n = n;}
	KIsZeroFunction() {}
	virtual ~KIsZeroFunction() {}
private:
	double n;
};

/**
 * Evaluates the function (1 - x)^n / x, where x is the first
 * parameter in p. This function is used for the special case
 * where 
 */
double KIsZeroFunction::evaluate(const Parameters & p) const
{
	const double eps = 1e-200;

	double x = p.getValue(0);
	if (x < 0 || x >= 1) return 0;
	
	// For very small values, alter value of x to x', so that division 
	// by 0 is avoided. eps should be chosen such that it is far below
	// the region in which accurate computation is required. Choice
	// of function is a smooth transition into a constant value of 
	// x' = eps/2 at x = 0.

	if (x < eps)
		x = x * x / eps / 2 + eps / 2;
	
	double lnx = log(x);
	double lnomx = lnomx = x > 0.001 ? log(1 - x) : -x*(1+x*(1+2*x*(1+3*x*(1+4*x*(1+5*x/6)/5)/4)/3)/2);
	double retval = exp(n * lnomx - lnx);
	return retval;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/** 
 * Integrator suppor for Binomial (Beta) - Tabular products. This class contains
 * the routines to do integrals and reverse integrals for this combination over a 
 * specified interval.
 *
 * Note that in the parameterization, k represents the number of events, n the
 * number of trials, and m is a special parameter that is 1 whenever the tabular
 * prior has a exponentially distributed density.
 *
 * Implementation Note: 
 *
 * The algorithms recognize three cases:
 *  - k == 0, n is integer, logdensity : uses a series approximation to compute i0.
 *  - k == 0, n is not integer, logdensity : uses a simpson integration to compute i0.
 *  - all other cases : BetaCDF is used to compute i0.
 *
 * For k == 0, the algorithm will switch to n is integer when n is close to an integer,
 * see computeSimpsonIntegral().
 *
 * 'mean' and 'variance' related measures (i1 and i2) can be computed through the
 * evaluation of (special) functions for which algorithms are readily available.
 */
TabularBinomialSupport::TabularBinomialSupport(double k, double n, int m, double lo, double hi)
{

	this->k = k;
	this->n = n;
	this->m = m;
	this->lo = lo;
	this->hi = hi;
	this->integer_n = false;

	if (m == 1 && k == 0) {

//		cout << "LOGTABBIN0 : n = " << n << " -> " ;

		// check whether n is close enough to integer such that
		// it can be approximated.
		int floor_n = (int)n;
		int ceil_n = floor_n + 1;
		if (fabs(floor_n - n) < 1e-4 * n) {
			this->n = floor_n;
			integer_n = true;
		} else if (fabs(ceil_n - n) < 1e-3 * n) {
			this->n = ceil_n;
			integer_n = true;
		} else {
			integer_n = false;
		}

//		cout << "n' = " << n << endl;

	}

	// following assignment verified OK.
	lnc = (m == 0 || k > 0) ? DCDFLIBInterface::computeLnGamma(k-m+1) + 
		  DCDFLIBInterface::computeLnGamma(n-k+1) - 
		  DCDFLIBInterface::computeLnGamma(n-m+2) : 0;

	// strange procedure necessary because computeIntegral refers to i0
	loi0 = 0;
	this->lo = 0;
	double loi0_temp;
	computeIntegral(lo,loi0_temp,lnc);
	loi0 = loi0_temp;
	this->lo = lo;
}

TabularBinomialSupport::TabularBinomialSupport(double k, double n, int m, double lo, double hi, double betaln)
{
	this->k = k;
	this->n = n;
	this->m = m;
	this->lo = lo;
	this->hi = hi;
	this->integer_n = false;

	if (m == 1 && k == 0) {

//		cout << "LOGTABBIN0 : n = " << n << " -> " ;

		// check whether n is close enough to integer such that
		// it can be approximated.
		int floor_n = (int)n;
		int ceil_n = floor_n + 1;
		if (fabs(floor_n - n) < 1e-4 * n) {
			this->n = floor_n;
			integer_n = true;
		} else if (fabs(ceil_n - n) < 1e-3 * n) {
			this->n = ceil_n;
			integer_n = true;
		} else {
			integer_n = false;
		}

//		cout << "n' = " << n << endl;

	}

	// following assignment verified OK.
	lnc = betaln;

	// strange procedure necessary because computeIntegral refers to i0
	loi0 = 0;
	this->lo = 0;
	double loi0_temp;
	computeIntegral(lo,loi0_temp,lnc);
	loi0 = loi0_temp;
	this->lo = lo;
}

TabularBinomialSupport::~TabularBinomialSupport()
{

}

/**
 * Computes the integral.
 * Note that in special cases where k is 0, m is 1, and q / b > 20, the code may
 * return -3 as an indication that an approximate value is returned (the value 
 * at q = n / 20. This is typically a good approximation, since the integral will
 * generally be close to its end value (at q = 1).
 */
int TabularBinomialSupport::computeIntegral(double x, double & i0, double & lnc) const
{
	int retval = 0;
	lnc = this->lnc;
	if (k == 0 && m == 1) { // special case : (1 - x)^n / x, lnc should be zero

		if (integer_n) {
			retval = computeSeriesIntegral((int)n,x,i0);
			i0 -= loi0;
		} else if (lo > 0) {
			retval = computeSimpsonIntegral(n,x,i0);
		} else {
			i0 = 0;
		}
		
	} else {
		//	i0 = DCDFLIBInterface::computeBetaCDF(k-m+1,n-k+1,x) - loi0;
		i0 = DCDFLIBInterface::computeDeltaBetaCDF(k-m+1,n-k+1,lo,x);
	}
	return retval;
}


int TabularBinomialSupport::computeLogIntegrant(double x, double & i, double & lnc) const
{
	int retval = 0;
	lnc = this->lnc;
	if (k == 0 && m == 1) { 
		// special case : (1 - x)^n / x, lnc should be zero
		double lnomx = x > 0.001 ? log(1 - x) : -x*(1+x*(1+2*x*(1+3*x*(1+4*x*(1+5*x/6)/5)/4)/3)/2);
		i = n * lnomx - log(x);
	} else {
		if (x == 0 || x == 1) {
			i = -3000 - lnc;
		} else {
			double lnx = log(x);
			double lnomx = lnomx = x > 0.001 ? log(1 - x) : -x*(1+x*(1+2*x*(1+3*x*(1+4*x*(1+5*x/6)/5)/4)/3)/2);
			i =  (k - m) * lnx + (n - k) * lnomx - this->lnc;
		}
	}
	return retval;
}

int TabularBinomialSupport::inverseIntegral(double i0, double & x) const
{
	double y = i0 + loi0;
	int retval = 0;

	if (k == 0 && m == 1) {
		if (integer_n) {
			retval = inverseSeriesIntegral((int)n,y,x);
		} else {
			retval = inverseSimpsonIntegral(n,y,x);
		}
	} else {
		x = DCDFLIBInterface::computeBetaQuantile(k-m+1,n-k+1,y);
	}
	return retval;
}

/**
 * Returns integrals over the interval lo-hi for the following equations
 * i0 : e^(-lnc) * x^(k-m) * (1-x)^(n-k)
 * i1 : e^(-lnc) * x^(k-m+1) * (1-x)^(n-k)
 * i2 : e^(-lnc) * x^(k-m+2) * (1-x)^(n-k)
 *
 * These equations correspond to x^i * Be(x|k-m+1,n-k+1), where Be(x|.) 
 * the Beta PDF. Parameter lnc = ln(Beta(k+1-m,n-k+1)), or 0 in the special case
 * where k = 0 and m = 1. Parameter m can be used to simulate a loguniform prior 
 * by setting its value to 1. For uniform prior set m to 0.
 *
 * This function requires that n is greater than 0.
 * 
 * See also the note about return values for computeIntegral(...)
 */ 
int TabularBinomialSupport::computeIntegrals(double & i0, double & i1, double & i2, double & lnc) const
{

	int retval = computeIntegral(hi,i0,lnc);

	if (k == 0 && m == 1) { // special case : (1 - x)^n / x, lnc should be zero

		// NOTE : there is a special case when hi == 1. ln_omhi is 'approximated'
		// but not used.

		double ln_omhi = (hi < 0.001 || hi == 1) ? -hi*(1+hi*(1+2*hi*(1+3*hi*(1+4*hi*(1+5*hi/6)/5)/4)/3)/2) : log(1 - hi); // ln(1-hi)
		double ln_omlo = (lo < 0.001) ? -lo*(1+lo*(1+2*lo*(1+3*lo*(1+4*lo*(1+5*lo/6)/5)/4)/3)/2) : log(1 - lo); // ln(1-lo)

//		cout << "TBS: LN 1-LO " << lo << " -> " << ln_omlo << endl;
//		cout << "TBS: LN 1-HI " << lo << " -> " << ln_omhi << endl;

		// Following closed-form results were obtained using Mathematica.
		i1  = exp((n + 1) * ln_omlo);
		i1 -= (hi < 1) ? exp((n + 1) * ln_omhi) : 0;
		i1 /= n + 1;

		i2 = (hi < 1) ? exp((n + 1) * ln_omhi) * (-1 - hi - hi * n) : 0; 
		i2 -= exp((n + 1) * ln_omlo) * (-1 - lo - lo * n);
		i2 /= ((1 + n) * (2 + n));
	} else {
		// following two assignments have been verified using Mathematica
//		i1 = (DCDFLIBInterface::computeBetaCDF(k-m+2,n-k+1,hi) - DCDFLIBInterface::computeBetaCDF(k-m+2,n-k+1,lo)) * 
		i1 = DCDFLIBInterface::computeDeltaBetaCDF(k-m+2,n-k+1,lo,hi) *
			 (k - m + 1) / (n - m + 2);
//		i2 = (DCDFLIBInterface::computeBetaCDF(k-m+3,n-k+1,hi) - DCDFLIBInterface::computeBetaCDF(k-m+3,n-k+1,lo)) * 
		i2 = DCDFLIBInterface::computeDeltaBetaCDF(k-m+3,n-k+1,lo,hi) *
			 (k - m + 1) * (k - m + 2) / (n - m + 2.) / (n - m + 3.) ;
	}

//	cout << "TBS returning integrals" << endl;
	return retval;
}

/**
 * Computes Integrate[(1-x)^b/x,{x,0,q}] + c, where c equals log(0)
 * Computation takes place using a series expansion.
 * This method returns 1 when q > 20 / b and b > 50. Furthermore, in that case it 
 * will return the value for q = 20 / b, which is usually a good approximation of
 * the integral for q = 1.
 *
 * These constants depend on the accuracy of the floating point operations.
 *
 * @param b number of demands
 * @param q integration interval upper bound
 * @param p integral value
 */
int TabularBinomialSupport::computeSeriesIntegral(int b, double q, double & p) const
{

//	cout << "COMPUTING LUB SERIES INTEGRAL AT " << q << " FOR N = " << b << endl;

	// The value is compute as the integral over the series expansion:
	// Integrate[-Sum[Binomial[n, k](-x)^(k - 1), {k, 1, n}], {x, 0, q}] + Log[q];

	// Script for generating table of reference values:
    // xxx = Integrate[-Sum[Binomial[1, k](-x)^(k - 1), {k, 1, 1}], {x, 0, 10^-13}] + Log[10^-13];
    // qarr = Table[10^i, {i, -7, 0}];
    // narr = {1, 10, 100, 1000, 10000, 100000, 1000000};
    // yzzz := NIntegrate[(1/x)(1 - x)^n, {x, 10^-13, q}, MaxRecursion -> 13] + xxx;
    // ztable = Table[q = qarr[[qi]]; n = narr[[ni]]; N[yzzz], {ni, 1, 7}, {qi, 1, 8}];
    // TableForm[ztable]

	int retval = 0;
	int kmax;

	double qb_prod = q * b;

	// Experimentally, we see that if q*b > 10, the 
	// integral value will be the same as for q = 10 / b
	// The replacement prevents us from running into instabilities
	// in the summation.
	if (qb_prod > 10) {
		q = 10. / b;
		qb_prod = 10;
//		cout << "REVISING Q TO " << q << endl;
	}

	// select the appropriate number of steps in the iteration
	if (qb_prod < 0.1)
		kmax = 10;
	else if (qb_prod < 1)
		kmax = 100;
	else 
		kmax = 1000;

	if (kmax > b) kmax = b;

	// compute expansion: z = a1*q(1 - a2*q(1 - a3*q(...)))
	double lnq = log(q);
	int k = kmax;
	double z = 0;
	while (k > 0) { 
		double alpha = (k == 1) ? -b : (b - k + 1) * (k - 1) / ((double)(k * k));
		if (z > 0.001 || z < -0.001 || z == 0) {
			z += 1;
			z *= -alpha * q;
		} else { // small z: use taylor series
			int alpha_sign = (alpha < 0) ? -1 : 1;
			double lnalpha = log(alpha_sign * alpha);
			double lnz = lnalpha + lnq + z*(1-z*(1-2*z*(1-3*z*(1-4*z*(1-5*z)/5)/4)/3)/2);
			z = -exp(lnz) * alpha_sign;
		}
		k--;
	}
	p = -z + log(q);
	return retval;
}

/**
 * Inverse function for computeSeriesIntegral.
 */
int TabularBinomialSupport::inverseSeriesIntegral(int n, double z, double & x) const
{
	double hival = hi;
	if (hival * n > 10) hival = 10. / n;

	double hicdf = 0; // cdf's are not real cdf's but unnormalized.
	computeSeriesIntegral(n,hival,hicdf);


	double loval = lo;
	double locdf = 0;

	double mdval = sqrt(hival * loval);
	double mdcdf;
	computeSeriesIntegral(n,mdval,mdcdf);

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

		mdval = sqrt(hival * loval);
		computeSeriesIntegral(n,mdval,mdcdf);
	}

	x = mdval;
	return (iterations < max_iterations) ? 0 : -1;
}

int TabularBinomialSupport::computeSimpsonIntegral(double n, double x, double & p) const
{
	if (x <= lo) {
		p = 0;
		return 0;
	}

	KIsZeroFunction func(n);
	SimpsonIntegrator simpson;
	p = simpson.simpson145(lo,x,func,0.00001,100);
	return 0;
}

int TabularBinomialSupport::inverseSimpsonIntegral(double n, double z, double & x) const
{
	if (z < 0) {
		x = lo;
		return 0;
	}

	double hival = hi;
	if (hival * n > 10) hival = 10. / n;

	double hicdf = 0; // cdf's are not real cdf's but unnormalized.
	computeSimpsonIntegral(n,hival,hicdf);

	double loval = lo;
	double locdf = 0;

	double mdval = sqrt(hival * loval);
	double mdcdf;
	computeSimpsonIntegral(n,mdval,mdcdf);

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

		mdval = sqrt(hival * loval);
		computeSimpsonIntegral(n,mdval,mdcdf);
	}

	x = mdval;
	return (iterations < max_iterations) ? 0 : -1;
}
