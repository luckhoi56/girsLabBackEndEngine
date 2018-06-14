// $Header: C:/cvsrepo/ptdev/native/redcas/parametric/TrLogNormalDistribution.h,v 1.2 2001/07/24 21:31:43 fgroen Exp $

// TrLogNormalDistribution.h: interface for the TrLogNormalDistribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRLOGNORMALDISTRIBUTION_H__332F1AF5_827E_11D4_98C4_002018557056__INCLUDED_)
#define AFX_TRLOGNORMALDISTRIBUTION_H__332F1AF5_827E_11D4_98C4_002018557056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParametricDistribution.h"

/**
 * Lognormal distribution truncated to the interval [0,1]
 *
 */

class TrLogNormalDistribution : public ParametricDistribution  
{
public:
	TrLogNormalDistribution(double nu, double tau);
	virtual ~TrLogNormalDistribution() {}
	virtual double getSample() const;
	virtual double getQuantile(double x) const;
	virtual double getMean() const;
	virtual double getCDF(double x) const;
	virtual double getLogPDF(double x) const;
	virtual double getVariance() const;
	virtual void   releaseMemory() {};
	virtual double getX(double x) { return x; };
	static bool checkParameters(double nu, double tau);
private:
	double nu;
	double tau;
	double iota; // equal to CDF[LogNormal[tau,nu],1.0]
};

#endif // !defined(AFX_TRLOGNORMALDISTRIBUTION_H__332F1AF5_827E_11D4_98C4_002018557056__INCLUDED_)
