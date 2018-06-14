//
// File:      $Id: NHPriorLogPoisson.cpp 2117 2013-01-21 05:20:35Z Dongfeng Zhu $
// Revision:  $Revision: 2117 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//


#include "NHPriorLogPoisson.h"
#include <math.h>
#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NHPriorLogPoisson::NHPriorLogPoisson()
{
	ef_nu  = log(33.3);
	ef_tau = 1.5;
	md_nu  = -14;
	md_tau = 6;
}

NHPriorLogPoisson::~NHPriorLogPoisson()
{

}

NHPriorLogPoisson::NHPriorLogPoisson(double md_nu, double md_tau, double ef_nu, double ef_tau)
{
	this->ef_nu = ef_nu;
	this->ef_tau = ef_tau;
	this->md_nu = md_nu;
	this->md_tau = md_tau;
}

double NHPriorLogPoisson::getLogPrior(const Parameters &params)
{
	double x = params.getValue(0); // median of the distribution
	double tau = params.getValue(1); // sigma of underlying normal

	// Following prior results in a uniform distribution over the median (x) and
	// error factor of the distribution.
	double prior = 1.6448536269514722 * tau;

	// lognormal over the median (between ~1e-12 and 0)
	double md = x;
	prior += -pow((log(md) - md_nu) / md_tau,2) / 2 - log(md_tau * md);

	// These settings specify standard error to be lognormally distributed between ~ 1 and 1000
	double lnef = 1.6448536269514722 * tau;
	double ef = exp(lnef);
	prior += -pow((lnef - ef_nu) / ef_tau,2) / 2 - log(ef_tau * ef);
	return prior;
}
