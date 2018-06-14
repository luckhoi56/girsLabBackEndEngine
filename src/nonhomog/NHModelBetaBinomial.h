//
// File:      $Id: NHModelBetaBinomial.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//
 

#if !defined(AFX_NHMODELBETABINOMIAL_H__7BE90277_7AEB_4870_B21F_52155AA53843__INCLUDED_)
#define AFX_NHMODELBETABINOMIAL_H__7BE90277_7AEB_4870_B21F_52155AA53843__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NHModel.h"
#include "NHLikelihoodBetaBinomial.h"
#include "MetropolisLNGenerator.h"
#include "NHPriorBetaBinomial.h"
#include "DCDFLIBInterface.h"


class NHModelBetaBinomial : public NHModel  
{
public:
	bool accept(NHInterpretation & interpretation);
//	int computeBoundsEx(int parameter, double & lower, double & higher);
	int computeMeasure(int parameter, double value, int measure, Distribution & distribution, StatusPtr status);
	NHPrior * createDefaultPrior();
	NHPrior * createUserPrior(int mode, double nu1, double tau1, double nu2, double tau2);
	MetropolisGenerator * createGenerator();
	NHInterpretation * createInterpretation(int type);
	NHModelBetaBinomial();
	virtual ~NHModelBetaBinomial();
protected:
	double computePercentile(const Parameters & params, double percentile) const;
	double computeVariance(const Parameters & params) const;
	double computeMean(const Parameters & params) const;
	double estimatePercentile(double lo, double lo_cdf, double hi, double hi_cdf, double z) const;
};

#endif // !defined(AFX_NHMODELBETABINOMIAL_H__7BE90277_7AEB_4870_B21F_52155AA53843__INCLUDED_)
