// $Header: C:/cvsrepo/ptdev/native/redcas/parametric/NormalDistribution.h,v 1.2 2001/07/24 21:31:43 fgroen Exp $

// NormalDistribution.h: interface for the NormalDistribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NORMALDISTRIBUTION_H__332F1AF3_827E_11D4_98C4_002018557056__INCLUDED_)
#define AFX_NORMALDISTRIBUTION_H__332F1AF3_827E_11D4_98C4_002018557056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParametricDistribution.h"

class NormalDistribution : public ParametricDistribution 
{
public:
	NormalDistribution(double mu, double sigma);
	virtual ~NormalDistribution() {};
	virtual double getSample() const;
	virtual double getQuantile(double x) const;
	virtual double getMean() const;
	virtual double getMedian() const {return getMean();}
	virtual double getCDF(double x) const;
	virtual double getLogPDF(double x) const;
	virtual double getVariance() const;
	virtual void   releaseMemory() {};
	virtual double getX(double x) { return x; };
	static bool checkParameters(double mu, double sigma);
private:
	double mu;
	double sigma;
};

#endif // !defined(AFX_NORMALDISTRIBUTION_H__332F1AF3_827E_11D4_98C4_002018557056__INCLUDED_)
