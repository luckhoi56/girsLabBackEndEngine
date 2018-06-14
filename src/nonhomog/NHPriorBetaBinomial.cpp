//
// File:      $Id: NHPriorBetaBinomial.cpp 2117 2013-01-21 05:20:35Z Dongfeng Zhu $
// Revision:  $Revision: 2117 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//

#include "stdafx.h"
#include "NHPriorBetaBinomial.h"

#include <math.h>
#include <iostream>
using namespace std;

NHPriorBetaBinomial::NHPriorBetaBinomial()
{
	mn_nu  = -14;
	mn_tau = 6;
	se_nu  = 0;
	se_tau = 3;
}

NHPriorBetaBinomial::NHPriorBetaBinomial(double mn_nu, double mn_tau, double se_nu, double se_tau)
{
	this->mn_nu = mn_nu;
	this->mn_tau = mn_tau;
	this->se_nu = se_nu;
	this->se_tau = se_tau;
//	cout << "created beta mn/se prior: " << mn_nu << " " << mn_tau << " " << se_nu << " " << se_tau << endl;
}

NHPriorBetaBinomial::~NHPriorBetaBinomial()
{

}

double NHPriorBetaBinomial::getLogPrior(const Parameters &params)
{

	//                1
	// prior  ---------------- results in a uniform joint density over log(mean) and log(mean/sdev)
	//         2 a (1 + a + b)
    //
	// The prior was obtained through a density transformation from a uniform 
	// density over log(mean) and log(sdev/mean) to a density over a and b.
	//
	// Note that some bias towards lower values of mean may be introduced by this
	// prior for higher values of the mean. This is due to the fact that the value
	// of the standard deviation has a maximum which depends on the mean.

	//	double a = params.getValue(0);
	//	double b = params.getValue(1);
	//	double prior = -log(2 * a * (1 + a + b));

    // The prior
	// 
	//                            b
	//   p[a,b] = -----------------------------------
	//            2 Sqrt[a b] (1 + a + b)^1.5 (a + b)
    // 
	// results in a uniform distribution over the mean and standard error (sdev/mean).
	// This can be demonstrated by showing that the script
	//
	//   a := mn (-mn + mn^2 + sd^2) / sd^2
	//   b := (mn - 2 mn^2 + mn^3 - sd^2 + mn sd^2) / sd^2
    //   Integrate[1 / p[a,b], {mn,mn0 + 0.001}, {sd,sd0,sd0 + 0.001}]
	// 
	// gives the same result for every mn0 and sd0 (as long as the integration bounds
	// are valid, and the intervals on mn and sd are small enough, such that the standard
	// error is approximately constant.

	double a = params.getValue(0);
	double b = params.getValue(1);

	double prior = log(b / (a + b)) - 0.5 * log(4 * a * b) - 1.5 * log(1 + a + b);

	// A term is added to obtain a lognormally distributed prior over the mean and standard error.

	// These settings specify mean to be lognormally distributed between ~ 1e-12 and 1
//	const double mn_nu  = -14;
//	const double mn_tau = 6;
	double mn = a / (a + b);
	double prior1 = -pow((log(mn) - mn_nu) / mn_tau,2) / 2 - log(mn_tau * mn);

	// These settings specify standard error to be lognormally distributed between ~ 0.001 and 1000.
//	const double se_nu  = 0;
//	const double se_tau = 3;
	double se = sqrt(b / a / (1 + a + b));
	double prior2 = -pow((log(se) - se_nu) / se_tau,2) / 2 - log(se_tau * se);

	double prior_tot = prior + prior1 + prior2;

	return prior_tot;
}