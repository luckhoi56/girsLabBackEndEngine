//
// File:      $Id: NHPriorLogNormalMedianSigma.cpp 2117 2013-01-21 05:20:35Z Dongfeng Zhu $
// Revision:  $Revision: 2117 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//

#include "stdafx.h"
#include "NHPriorLogNormalMedianSigma.h"
#include <math.h>
#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NHPriorLogNormalMedianSigma::NHPriorLogNormalMedianSigma()
{

}

NHPriorLogNormalMedianSigma::~NHPriorLogNormalMedianSigma()
{
	si_nu  = log((double)1);
	si_tau = 1.5;
	md_nu  = -14;
	md_tau = 6;
}

NHPriorLogNormalMedianSigma::NHPriorLogNormalMedianSigma(double md_nu, double md_tau, double ef_nu, double ef_tau)
{
	this->si_nu = ef_nu;
	this->si_tau = ef_tau;
	this->md_nu = md_nu;
	this->md_tau = md_tau;
//	cout << "created lognormal med/sigma prior: " << md_nu << " " << md_tau << " " << si_nu << " " << si_tau << endl;
}

double NHPriorLogNormalMedianSigma::getLogPrior(const Parameters &params)
{
	double x = params.getValue(0); // median of the distribution
	double sigma = params.getValue(1); // sigma of underlying normal

	double prior = 0;

	// lognormal over the median
	double md = x;
	prior += -pow((log(md) - md_nu) / md_tau,2) / 2 - log(md_tau * md);

	// These settings specify standard error to be lognormally distributed between ~ 1 and 1000
	double lnsi = log(sigma);
	prior += -pow((lnsi - si_nu) / si_tau,2) / 2 - log(si_tau * sigma);

	return prior;
}
