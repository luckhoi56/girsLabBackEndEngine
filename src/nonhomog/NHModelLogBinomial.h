//
// File:      $Id: NHModelLogBinomial.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//
 

#pragma once
#include "nhmodel.h"

#include "NHLikelihoodLogBinomial.h"
#include "MetropolisGammaGenerator.h"
#include "MetropolisLNGenerator.h"
#include "MetropolisNormalGenerator.h"
#include "MetropolisTLNGenerator.h"
#include "MetropolisBetaGenerator.h"
#include "NHPriorLogPoisson.h"
#include "DCDFLIBInterface.h"

class NHModelLogBinomial :
	public NHModel
{
public:
	NHModelLogBinomial(void);
	~NHModelLogBinomial(void);
	bool accept(NHInterpretation & interpretation);
//	int computeBoundsEx(int parameter, double & lower, double & higher);
	int computeMeasure(int parameter, double value, int measure, Distribution & distribution, StatusPtr status);
	NHPrior * createDefaultPrior();
	NHPrior * createUserPrior(int mode, double nu1, double tau1, double nu2, double tau2);
	MetropolisGenerator * createGenerator();
	NHInterpretation * createInterpretation(int type);

protected:
	double computePercentile(const Parameters & params, double percentile) const;
	double computeVariance(const Parameters & params) const;
	double computeMean(const Parameters & params) const;
	double estimatePercentile(double lo, double lo_cdf, double hi, double hi_cdf, double z) const;
};
