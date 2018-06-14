//
// File:      $Id: NHPriorGammaPoisson.cpp 2117 2013-01-21 05:20:35Z Dongfeng Zhu $
// Revision:  $Revision: 2117 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//

#include "stdafx.h"
#include "NHPriorGammaPoisson.h"
#include <math.h>
#include <iostream>
using namespace std;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NHPriorGammaPoisson::NHPriorGammaPoisson(double mn_nu, double mn_tau, double se_nu, double se_tau)
{
	this->mn_nu = mn_nu;
	this->mn_tau = mn_tau;
	this->se_nu = se_nu;
	this->se_tau = se_tau;
//	cout << "created gamma mn/se prior: " << mn_nu << " " << mn_tau << " " << se_nu << " " << se_tau << endl;
}

NHPriorGammaPoisson::NHPriorGammaPoisson()
{
	mn_nu  = -14;
	mn_tau = 6;
	se_nu  = 0;
	se_tau = 3;
}

NHPriorGammaPoisson::~NHPriorGammaPoisson()
{

}

double NHPriorGammaPoisson::getLogPrior(const Parameters &params)
{
	// Sample space: alpha and theta
	// Uniform over mean and standard error.

	// Clear[mn, a, t, se, m0, m1, se0, se1, i0, i1, f]
	// m0 = 0.8; m1 = 1.4;
	// se0 = 0.2; se1 = 2;
	// f[mn_, se_] := 
	//     PDF[LogNormalDistribution[Log[1], 1.4], mn]PDF[
	//         LogNormalDistribution[Log[0.6], 1], se];
	// j[a_, t_] := 0.5/(Sqrt[a](t^2));
	// i0 = Integrate[
	//     Integrate[f[a/t, 1/Sqrt[a]]j[a, t], {t, a/m1, a/m0}], {a, 1/se1^2, 
	//       1/se0^2}]
	// i1 = Integrate[f[m, s], {m, m0, m1}, {s, se0, se1}]

	double a = params.getValue(0);
	double theta = params.getValue(1); // mean = 1 / theta
	double prior = -0.5 * log(a) - 2 * log(theta);

	// lognormal over the mean
	double mn = a / theta;
	prior += -pow((log(mn) - mn_nu) / mn_tau,2) / 2 - log(mn_tau * mn);

	// These settings specify standard error to be lognormally distributed between ~ 0.001 and 1000.
	double se = 1 / sqrt(a);
	prior += -pow((log(se) - se_nu) / se_tau,2) / 2 - log(se_tau * se);

	return prior;
}
