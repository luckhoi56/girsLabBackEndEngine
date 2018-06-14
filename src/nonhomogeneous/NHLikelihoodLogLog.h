//
// File:      $Id: NHLikelihoodLogLog.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//
 

#if !defined(AFX_NHLIKELIHOODLOGLOG_H__D17CCDA4_B962_4F1A_9FE8_05BCD3B14188__INCLUDED_)
#define AFX_NHLIKELIHOODLOGLOG_H__D17CCDA4_B962_4F1A_9FE8_05BCD3B14188__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NHLikelihood.h"
#include "NHModel.h"
#include <math.h>

/**
 * Deprecated. Use NHLikelihoodLogPoisson instead.
 */
class NHLikelihoodLogLog : public NHLikelihood  
{
public:
	void preprocess(const NHObservation & observation);

    double getLogLikelihood(const Parameters & params, NHObservationIterator & iterator, int observations);
	NHLikelihoodLogLog();
	virtual ~NHLikelihoodLogLog();
private:
	double getLogNormalLogNormal(double nu, double tau, double median, double sigma);
	double getLogNormalBinomialLogNormal(double nu, double tau, double k, double n,
										 double median, double sigma);
};

#endif // !defined(AFX_NHLIKELIHOODLOGLOG_H__D17CCDA4_B962_4F1A_9FE8_05BCD3B14188__INCLUDED_)
