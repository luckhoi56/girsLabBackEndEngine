//
// File:      $Id: NHModelLogLog.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//
 

#if !defined(AFX_NHMODELLOGLOG_H__38CBA0C9_5203_4F56_92F5_977775F61669__INCLUDED_)
#define AFX_NHMODELLOGLOG_H__38CBA0C9_5203_4F56_92F5_977775F61669__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NHModel.h"
#include "NHLikelihoodLogLog.h"
#include "NHPriorLogLog.h"
#include "../global/MetropolisGammaGenerator.h"
#include "../global/MetropolisLNGenerator.h"
#include "../global/MetropolisNormalGenerator.h"
#include "../global/MetropolisTLNGenerator.h"
#include "../dcdf/DCDFLIBInterface.h"


class NHModelLogLog : public NHModel  
{
public:

	bool accept(NHInterpretation & interpretation);
//	int computeBoundsEx(int parameter, double & lower, double & higher);
	int computeMeasure(int parameter, double value, int measure, Distribution & distribution, StatusPtr status);
	NHPrior * createDefaultPrior();
	NHPrior * createUserPrior(int mode, double nu1, double tau1, double nu2, double tau2);
	MetropolisGenerator * createGenerator();
	NHInterpretation * createInterpretation(int type);
	NHModelLogLog();
	virtual ~NHModelLogLog();
protected:
	double computePercentile(const Parameters & params, double percentile) const;
	double computeVariance(const Parameters & params) const;
	double computeMean(const Parameters & params) const;
	double estimatePercentile(double lo, double lo_cdf, double hi, double hi_cdf, double z) const;

};

#endif // !defined(AFX_NHMODELLOGLOG_H__38CBA0C9_5203_4F56_92F5_977775F61669__INCLUDED_)
