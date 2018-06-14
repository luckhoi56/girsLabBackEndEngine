//
// File:      $Id: NHModelGammaPoisson.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//
 

#if !defined(AFX_NHMODELGAMMAPOISSON_H__76F2025B_AC92_4421_9C14_909CFC2C1675__INCLUDED_)
#define AFX_NHMODELGAMMAPOISSON_H__76F2025B_AC92_4421_9C14_909CFC2C1675__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NHModel.h"
#include "NHLikelihoodGammaPoisson.h"
#include "../global/MetropolisGammaGenerator.h"
#include "../global/MetropolisLNGenerator.h"
#include "../global/MetropolisTLNGenerator.h"
#include "NHPriorGammaPoisson.h"
#include "../dcdf/DCDFLIBInterface.h"
#include "NHModel.h"


class NHModelGammaPoisson : public NHModel  
{
public:
	bool accept(NHInterpretation & interpretation);
//	int computeBoundsEx(int parameter, double & lower, double & higher);
	int computeMeasure(int parameter, double value, int measure, Distribution & distribution, StatusPtr status);
	NHPrior * createDefaultPrior();
	NHPrior * createUserPrior(int mode, double nu1, double tau1, double nu2, double tau2);
	MetropolisGenerator * createGenerator();
	NHInterpretation * createInterpretation(int type);
	NHModelGammaPoisson();
	virtual ~NHModelGammaPoisson();
protected:
	double computePercentile(const Parameters & params, double percentile) const;
	double computeVariance(const Parameters & params) const;
	double computeMean(const Parameters & params) const;
	double estimatePercentile(double lo, double lo_cdf, double hi, double hi_cdf, double z) const;

};

#endif // !defined(AFX_NHMODELGAMMAPOISSON_H__76F2025B_AC92_4421_9C14_909CFC2C1675__INCLUDED_)
