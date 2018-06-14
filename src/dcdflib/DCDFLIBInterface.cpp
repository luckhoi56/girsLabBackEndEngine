// $Header: C:/cvsrepo/ptdev/native/redcas/dcdflib/DCDFLIBInterface.cpp,v 1.9 2003/07/29 16:37:11 fgroen Exp $

// DCDFLIBInterface.cpp: implementation of the DCDFLIBInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "DCDFLIBInterface.h"
#include "cdflib.h"

#include <math.h>
#include "Error.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SyncMutex DCDFLIBInterface::mutex("numerical.library.dcdflib");

/**
 * Returns the gamma cdf. 
 * Scale has lambda interpretation.
 */
double DCDFLIBInterface::computeGammaCDF(double shape, double scale, double x)
{
	int which = 1;
	double p;
	double q;
	int status;
	double bound;

	mutex.obtain();
	cdfgam(&which,&p,&q,&x,&shape,&scale,&status,&bound);
	mutex.release();

	if (status != 0) return 0;
	return p;
}


/**
 * Returns the gamma pdf. 
 * Scale has lambda interpretation.
 */
double DCDFLIBInterface::computeGammaPDF(double shape, double scale, double x)
{
	if (x == 0) return 0;
	if (x < 0) throw INVALID_ARGUMENT;
	return exp( shape * log(scale) - computeLnGamma(shape) + (shape - 1) * log(x) - (x * scale) );
}

double DCDFLIBInterface::computeNormalCDF(double mean, double sd, double x)
{
	int which = 1;
	double p;
	double q;
	int status;
	double bound;
	mutex.obtain();
	cdfnor(&which,&p,&q,&x,&mean,&sd,&status,&bound);
	mutex.release();
	if (status != 0) return 0;
	return p;
}

double DCDFLIBInterface::computeBetaCDF(double alpha, double beta, double x)
{
	int which = 1;
	double p;
	double q;
	double y = 1 - x;
	int status;
	double bound;
	mutex.obtain();
	cdfbet(&which,&p,&q,&x,&y,&alpha,&beta,&status,&bound);
	mutex.release();
	if (status != 0) return 0;
	return p;
}


double DCDFLIBInterface::computeBetaPDF(double alpha, double beta, double x)
{
	if (x < 0.0|| x > 1.0) throw INVALID_ARGUMENT;

	double lnbetafunc = computeLnBeta(alpha,beta);
	double lnx = log(x);
	double lnomx;

	if (lnx < -25)
		lnomx = -x;
	else
		lnomx = log(1 - x);

	return exp( (alpha - 1) * lnx + (beta - 1) * lnomx - lnbetafunc );
}

double DCDFLIBInterface::computeNormalQuantile(double mean, double sd, double p)
{
	int which = 2;
	double q = 1 - p;
	int status;
	double x;
	double bound;
	mutex.obtain();
	cdfnor(&which,&p,&q,&x,&mean,&sd,&status,&bound);
	mutex.release();
	if (status != 0) return 0;
	return x;
}

double DCDFLIBInterface::computeGammaQuantile(double shape, double scale, double p)
{
	int which = 2;
	double q = 1 - p;
	int status;
	double x;
	double bound;
	mutex.obtain();
	cdfgam(&which,&p,&q,&x,&shape,&scale,&status,&bound);
	mutex.release();
	if (status != 0) return 0;
	return x;
}

double DCDFLIBInterface::computeBetaQuantile(double alpha, double beta, double p)
{
	int which = 2;
	double q = 1 - p;
	int status;
	double x;
	double y;
	double bound;
	mutex.obtain();
	cdfbet(&which,&p,&q,&x,&y,&alpha,&beta,&status,&bound);
	mutex.release();
	if (status != 0) return 0;
	return x;
}

double DCDFLIBInterface::computeLnGamma(double x)
{
	double result = 0;
	mutex.obtain();
	result = gamln(&x);
	mutex.release();
	return result;
}

double DCDFLIBInterface::computeDiGamma(double x)
{
	double result = 0;
	mutex.obtain();
	result = psi(&x);
	mutex.release();
	return result;
}

double DCDFLIBInterface::computeLnBeta(double a, double b)
{
	double result = 0;
	double aplusb = a + b;
	mutex.obtain();
	result = gamln(&a) + gamln(&b) - gamln(&aplusb);
	mutex.release();
	return result;
}

double DCDFLIBInterface::computeLognormalPDF(double nu, double tau, double x)
{
	if (x <= 0) throw INVALID_ARGUMENT;
	double z = (log(x) - nu) / tau;
	return exp((-z * z / 2) + log(0.3989422804 / (tau * x)));
}

double DCDFLIBInterface::computeLognormalCDF(double nu, double tau, double x)
{
	if (x <= 0) throw INVALID_ARGUMENT;
	return computeNormalCDF(nu, tau, log(x));
}

double DCDFLIBInterface::computeLognormalQuantile(double nu, double tau, double p)
{
	if (p < 0 || p >= 1) throw INVALID_ARGUMENT;
	if (p == 0) return 0;
	return exp(computeNormalQuantile(nu, tau, p));
}

/*
double DCDFLIBInterface::computeLogGammaPDF(double shape, double scale, double x)
{
	if (x == 0) return 0;
	if (x < 0) throw INVALID_ARGUMENT;
	scale = 1 /scale;
	return -shape * log(scale) - computeLnGamma(shape) + (shape - 1) * log(x) - (x / scale);
}

double DCDFLIBInterface::computeLogLognormalPDF(double nu, double tau, double x)
{
	if (x <= 0) throw INVALID_ARGUMENT;
	double z = (log(x) - nu) / tau;
	return (-z * z / 2) + log(0.3989422804 / (tau * x));
}
*/

/**
 * Computes error function.
 */
double DCDFLIBInterface::computeErf(double x)
{
	double result = 0;
	mutex.obtain();
	result = erf1(&x);
	mutex.release();
	return result;
}

/**
 * Computes complement error function.
 */
double DCDFLIBInterface::computeErfc(double x)
{
	double result = 0;
	int ind = 0;
	mutex.obtain();
	result = erfc1(&ind,&x);
	mutex.release();
	return result;
}

/**
 * Generalized incomplete beta function.
 * This function returns the difference between two incomplete Beta functions.
 * The difference is computed as (bet1 - bet0) or ((1-bet0) - (1-bet1)), depending
 * on whether bet0 and bet1 are small (<0.5) or large. This gives good results in 
 * good results for the tails of the distribution, though not necessarily when lo and
 * hi are very close together.
 */
double DCDFLIBInterface::computeDeltaBetaCDF(double a, double b, double lo, double hi)
{
	double lox = lo;
	double loy = 1 - lo;
	double hix = hi;
	double hiy = 1 - hi;
	double locum,loccum,hicum,hiccum;

	cumbet(&lox,&loy,&a,&b,&locum,&loccum);
	cumbet(&hix,&hiy,&a,&b,&hicum,&hiccum);

	if (locum <= 0.5)
		return hicum - locum;
	else
		return loccum - hiccum;
}

/**
 * Generalized incomplete Gamma function. 
 * Scale has lambda interpretation.
 */
double DCDFLIBInterface::computeDeltaGammaCDF(double shape, double scale, double lo, double hi)
{
	double lox = lo * scale;
	double hix = hi * scale;
	double locum,loccum,hicum,hiccum;

	cumgam(&lox,&shape,&locum,&loccum);
	cumgam(&hix,&shape,&hicum,&hiccum);

	if (locum <= 0.5)
		return hicum - locum;
	else
		return loccum - hiccum;
}

/**
 * Computes the difference between error functions.
 */
double DCDFLIBInterface::computeDeltaErf(double lo, double hi)
{
	double erfc_max, erfc_min, x_min, x_max;
	if (hi == lo) return 0;
	int ind = 0;

	if (lo > 0) {
		x_min = hi;
		x_max = lo;
	} else {
		x_max = -hi;
		x_min = -lo;
	}

	mutex.obtain();
	erfc_max = erfc1(&ind,&x_max);
	erfc_min = erfc1(&ind,&x_min);
	mutex.release();
	double delta_erf = erfc_max - erfc_min;
	return delta_erf;
}

/**
 * Computes inverse error function.
 */
double DCDFLIBInterface::computeInverseErf(double z)
{
	return computeNormalQuantile(0,1,(1 + z) / 2) / sqrt((double)2);
}
