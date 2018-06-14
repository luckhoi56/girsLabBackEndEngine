//
// File:      $Id: NHLogPoissonSupport.cpp 2117 2013-01-21 05:20:35Z Dongfeng Zhu $
// Revision:  $Revision: 2117 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//


#include "NHLogPoissonSupport.h"
#include "../homogeneous/SimpleHomogeneous.h"
#include "../global/SimpsonIntegrator.h"

#include <math.h>
#include <iostream>
#include <time.h>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NHLogPoissonSupport::NHLogPoissonSupport()
{

}

NHLogPoissonSupport::~NHLogPoissonSupport()
{

}

/**
 * Returns the natural log of the integral of the expression
 * x^(a-1) * Exp[-x*b] * Exp[-(1/2)*((Log[x]-nu)/tau)^2]
 * for x between 0 and infinity.
 */
double NHLogPoissonSupport::integrate(double nu, double tau, double a, double b,bool & fail)
{
	double lo_x,hi_x,mode_x;
	double integral;
	double ln_integral = 0;

	this->nu = nu;
	this->tau = tau;
	this->alpha = a;
	this->beta = b;
	this->ln_norm = 0;

	long t1 = clock();
	determineBounds(lo_x,hi_x,mode_x);
	long t2 = clock();
	bounds_cycles += t2 - t1;

	long t3 = clock();
	if (lo_x < hi_x) {
		SimpsonIntegrator simpson;
		this->ln_norm = evaluate_ln(mode_x);
		integral = simpson.simpson145(lo_x,hi_x,*this,0.001,80);
		ln_integral = log(integral);
	} else {
		fail = true;
	}
	long t4 = clock();

	integr_cycles += t4 - t3;
	
	ln_integral += ln_norm;

	return ln_integral;
}

/**
 * Determines integration bounds as well as the mode of the Gamma/Lognormal
 * product. Bounds are found by means of an iterative procedure, starting
 * from the mode. Works only in interval mode > 1e-50.
 */
void NHLogPoissonSupport::determineBounds(double &lowerBound, double &upperBound, 
										  double & mode)
{

	lowerBound = 0;
	upperBound = 0;
	mode = 0;

	// First determine approximate mode of the updated distribution
	// According to Mathematica, the mode is found at
	// ProductLog[Exp[nu-tau^2+k*tau^2]T*tau^2]/(T*tau^2)
	// ProductLog[Exp[nu-tau^2+a*tau^2]b*tau^2]/(b*tau^2)
	// where ProductLog[z] is the value of w for which z == w*Exp[w]
	double lnz = nu + (alpha - 1) * tau * tau + log(beta) + 2 * log(tau);
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
	int max_iterations = 50;

	while (lnwmax - lnwmin > 0.00001 && iterations < max_iterations) {
		double lnw = (lnwmax + lnwmin) / 2;
		double lnwew = lnw + exp(lnw);
		if (lnwew < lnz) {
			lnwmin = lnw;
		} else {
			lnwmax = lnw;
		}
		iterations++;
	}

	// if mode < 1e-50 -> abort
	if ((lnwmax + lnwmin) / 2 - log(beta) - 2 * log(tau) < -115) {
		return;
	}

	double x = exp((lnwmax + lnwmin) / 2 - log(beta) - 2 * log(tau));

	mode = x;

	double p_max = evaluate_ln(x);
	double x_max = x;

	// descend in positive and negative direction, until value is sufficiently
	// small such that it is not expected to give significant contribution to
	// the integral.
	x = x_max / 1.3;
	while (evaluate_ln(x) > p_max - log(100000 * x / x_max) && x >= 1e-50)
		x /= 1.3;
	lowerBound = x;

	x = x_max * 1.3;
	while (evaluate_ln(x) > p_max - log(10000 * x / x_max))
		x *= 1.3;
	upperBound = x;

}

/**
 * Evaluates the function.
 * Pass-through call to evaluate(double).
 */
double NHLogPoissonSupport::evaluate(const Parameters & params) const
{
	double x = params.getValue(0);
	return exp(evaluate_ln(x));
}

double NHLogPoissonSupport::evaluate(double x) const
{
	return exp(evaluate_ln(x));
}

/**
 * Evaluates the function at x.
 * The function is x^(a-1) * Exp[-x*b] * Exp[-(1/2)*((Log[x]-nu)/tau)^2].
 */
double NHLogPoissonSupport::evaluate_ln(double x) const
{
	double ln_x = log(x);
	double ln_y = -0.5 * pow((ln_x - nu) / tau , 2) + (alpha - 1) * ln_x - x * beta - ln_norm;
	return ln_y;
}
