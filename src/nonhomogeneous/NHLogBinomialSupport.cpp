//
// File:      $Id: NHLogBinomialSupport.cpp 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//

#include "NHLogBinomialSupport.h"
#include "../global/SimpsonIntegrator.h"
#include "../dcdf/DCDFLIBInterface.h"

#include <math.h>

NHLogBinomialSupport::NHLogBinomialSupport(void)
{
}

NHLogBinomialSupport::~NHLogBinomialSupport(void)
{
}

/**
 * Returns the natural log of the integral of the expression
 * x^(a-1) * (1-x)^(b-1) * Exp[-(1/2)*((Log[x]-nu)/tau)^2]
 * for x between 0 and 1.
 */
double NHLogBinomialSupport::integrate(double nu, double tau, double a, double b,bool & fail)
{
	double lo_x,hi_x,mode_x;
	double integral,ln_integral;

	this->nu = nu;
	this->tau = tau;
	this->alpha = a;
	this->beta = b;
	this->ln_norm = 0;

	determineBounds(lo_x,hi_x,mode_x);

	if (lo_x > 0.99999) {
		fail = true;
	} else if (lo_x < hi_x) {
		SimpsonIntegrator simpson;

		// determine normalization factor used during integration
		this->ln_norm = evaluate_ln(mode_x);

		if (hi_x < 0.99999) {
			integral = simpson.simpson145(lo_x,hi_x,*this,0.0001,80);
		} else {
			// special case: pdf to infinity for x -> 1:
			// In this case, the routines assumes a constant 
			// contribution of the exponential term over the
			// interval 0.99999 - 1. This reduces the integral
			// to the incomplete Beta function.
			integral = simpson.simpson145(lo_x,0.99999,*this,0.0001,80);
			integral += exp(-0.5 * pow(nu / tau , 2) + DCDFLIBInterface::computeLnBeta(beta,alpha) - ln_norm) *
				DCDFLIBInterface::computeBetaCDF(beta,alpha,0.00001);

			//double tst1 = simpson.simpson145(0.9999,0.99999,*this,0.0001,80);
			//double tst2 = exp(-0.5 * pow(nu / tau , 2) + DCDFLIBInterface::computeLnBeta(beta,alpha) - ln_norm) *
			//	(-DCDFLIBInterface::computeBetaCDF(beta,alpha,0.00001) + DCDFLIBInterface::computeBetaCDF(beta,alpha,0.0001));
			//tst1 -= tst2;
		}

		ln_integral = log(integral);
		ln_integral += ln_norm;
	} else {
		fail = true;
	}

	return ln_integral;
}

/**
 * Determines integration bounds as well as the mode of the Beta/Lognormal
 * product. Bounds are found by means of an iterative procedure, starting
 * from the mode. Works only in interval mode 1e-50 < x < 1.
 */
void NHLogBinomialSupport::determineBounds(double &lowerBound, double &upperBound, 
										  double & mode)
{
	double x;

	lowerBound = 0;
	upperBound = 0;
	mode = findMode();

	double p_max = evaluate_ln(mode);
	double x_max = mode;

	// descend in positive and negative direction, until value is sufficiently
	// small such that it is not expected to give significant contribution to
	// the integral.
	x = x_max / 1.2;
	while (x >= 1e-50 && evaluate_ln(x) > p_max - log(100000 * x / x_max))
		x /= 1.2;
	lowerBound = x;

	x = x_max * 1.2;
	while (x < 1 && evaluate_ln(x) > p_max - log(10000 * x / x_max))
		x *= 1.2;
	upperBound = x;

	if (upperBound > 1) upperBound = 1;
}

/**
 * Finds the mode of the expression.
 * Mode is located by means of a bracket search, which
 * is preceded by a simple condition based iteration, 
 * in order to set up the initial bracket.
 */
double NHLogBinomialSupport::findMode() const 
{
 // (2 - a - b) x + a - 1 - (1 - x) (-nu + Log[x]) / tau ^ 2 == 0
 // provides condition. function is strictly monotonically decreasing.
 // Mode location is found by means of a bracket search.

	double x,lnx;

	double tau_sq = pow(tau,2);

	// establish bracket
	double i = 0;
	lnx = nu + i * tau;
	x = exp(lnx);
	while (((2 - alpha - beta) * x + alpha - 1 - (1 - x) * (-nu + log(x)) / tau_sq) > 0 && x < 1) {
		i++;
		lnx = nu + i * tau;
		x = exp(lnx);
	}
	while (((2 - alpha - beta) * x + alpha - 1 - (1 - x) * (-nu + log(x)) / tau_sq) < 0 || x > 1) {
		i--;
		lnx = nu + i * tau;
		x = exp(lnx);
	}

	// bracket search
	double lox = exp(nu + i * tau);
	double hix = exp(nu + (i + 1) * tau);
	if (hix > 1) hix = 1;
	while (hix / lox > 1.001) {
		x = sqrt(hix * lox);
		if (((2 - alpha - beta) * x + alpha - 1 - (1 - x) * (-nu + log(x)) / tau_sq) > 0) {
			lox = x;
		} else {
			hix = x;
		}
	}
	return x;
}

/**
 * Evaluates the function.
 * Pass-through call to evaluate(double).
 * x must be greater than 0 and smaller than (1 - 1e-10).
 */
double NHLogBinomialSupport::evaluate(const Parameters & params) const
{
	double x = params.getValue(0);
	return exp(evaluate_ln(x));
}

double NHLogBinomialSupport::evaluate(double x) const
{
	return exp(evaluate_ln(x));
}

/**
 * Evaluates the function at x.
 * The function is x^(a-1) * (1-x)^(b-1) * Exp[-(1/2)*((Log[x]-nu)/tau)^2].
 * x must be greater than 0 and smaller than (1 - 1e-10).
 */
double NHLogBinomialSupport::evaluate_ln(double x) const
{
	double ln_x = log(x);
	double ln_omx = log(1 - x);
	double ln_y = -0.5 * pow((ln_x - nu) / tau , 2) + (alpha - 1) * ln_x + (beta - 1) * ln_omx - ln_norm;
	return ln_y;
}
