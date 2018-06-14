// $Header: C:/cvsrepo/ptdev/native/redcas/parametric/BetaDistribution.h,v 1.2 2001/07/24 21:31:42 fgroen Exp $

// BetaDistribution.h: interface for the BetaDistribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BETADISTRIBUTION_H__28141A74_81E1_11D4_98C3_002018557056__INCLUDED_)
#define AFX_BETADISTRIBUTION_H__28141A74_81E1_11D4_98C3_002018557056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParametricDistribution.h"

class BetaDistribution : public ParametricDistribution  
{
public:
	static bool checkParameters(double alpha, double beta);
	virtual double getSample() const;
	virtual double getMean() const;
	virtual double getQuantile(double x) const;
	virtual double getCDF(double x) const;
	virtual double getLogPDF(double x) const;
	virtual double getVariance() const;
	virtual void   releaseMemory() {};
	BetaDistribution(double alpha, double beta);
	virtual ~BetaDistribution() {};
private:
	double beta;
	double alpha;
	double lnbetafunc;
};

#endif // !defined(AFX_BETADISTRIBUTION_H__28141A74_81E1_11D4_98C3_002018557056__INCLUDED_)
