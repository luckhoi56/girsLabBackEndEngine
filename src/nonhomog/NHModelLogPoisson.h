//
// File:      $Id: NHModelLogPoisson.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//
 

#if !defined(AFX_NHMODELLOGPOISSON_H__404D7C54_20CC_46E4_A5C1_C4D87BB23D56__INCLUDED_)
#define AFX_NHMODELLOGPOISSON_H__404D7C54_20CC_46E4_A5C1_C4D87BB23D56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NHModel.h"
#include "NHLikelihoodLogPoisson.h"
#include "MetropolisGammaGenerator.h"
#include "MetropolisLNGenerator.h"
#include "MetropolisNormalGenerator.h"
#include "MetropolisTLNGenerator.h"
#include "MetropolisBetaGenerator.h"
#include "NHPriorLogPoisson.h"
#include "DCDFLIBInterface.h"


class NHModelLogPoisson : public NHModel  
{
public:
	bool accept(NHInterpretation & interpretation);
//	int computeBoundsEx(int parameter, double & lower, double & higher);
	int computeMeasure(int parameter, double value, int measure, Distribution & distribution, StatusPtr status);
	NHPrior * createDefaultPrior();
	NHPrior * createUserPrior(int mode, double nu1, double tau1, double nu2, double tau2);
	MetropolisGenerator * createGenerator();
	NHInterpretation * createInterpretation(int type);
	NHModelLogPoisson();
	virtual ~NHModelLogPoisson();
protected:
	double computePercentile(const Parameters & params, double percentile) const;
	double computeVariance(const Parameters & params) const;
	double computeMean(const Parameters & params) const;
	double estimatePercentile(double lo, double lo_cdf, double hi, double hi_cdf, double z) const;

};

#endif // !defined(AFX_NHMODELLOGPOISSON_H__404D7C54_20CC_46E4_A5C1_C4D87BB23D56__INCLUDED_)
