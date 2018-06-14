// SLATECInterface.cpp: implementation of the SLATECInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "SLATECInterface.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Provides access to SLATEC numerical library routines.
// While SLATEC is published in the form of FORTRAN, source has
// been converted with f2c.

extern "C" double dei_(double x);

SLATECInterface::SLATECInterface()
{

}

SLATECInterface::~SLATECInterface()
{

}

double SLATECInterface::exponentialIntegralEi(double x)
{
	return dei_(x);
}

/**
 * Computes the inverse exponential integral Ei value x, such that
 * Ei(x) = ei. Note that this implementation allows negative values 
 * of x only. Parameter ei must therefore also be smaller than zero.
 * 
 * On this range, Ei(x) is strictly negative, and monotonically 
 * decreasing (at a rate exp(x) / x ).
 * 
 * This is not a very efficient implementation using a simple geometric
 * bracket search.
 */
double SLATECInterface::invExponentialIntegralEi(double ei, double x0)
{
	const double epsilon = 0.000001;
	const int maxiter = 30;

	double x = x0;
	double hi = x0;
	double lo = 0;

	double f = dei_(hi);

	while (f > ei) {
		lo = hi;
		hi /= 2;
		f = dei_(hi);
	}

	if (lo == 0) {
		// previous while loop never entered: f = dei(x0)
		lo = x0;
		while (f < ei) {
			hi = lo;
			lo *= 2;
			f = dei_(lo);
		}
	}

	int iter = 0;
	while (fabs(1 - f/ei) > epsilon && iter < maxiter) {
		x = -pow(hi * lo , 0.5);
		f = dei_(x);
		if (f > ei) lo = x;
		if (f < ei) hi = x;
		iter++;
	}

	return x;
}
