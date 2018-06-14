// $Header: C:/cvsrepo/ptdev/native/redcas/parametric/LogNormalDistribution.h,v 1.3 2001/08/10 14:28:23 fgroen Exp $

// LogNormalDistribution.h: interface for the LogNormalDistribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGNORMALDISTRIBUTION_H__332F1AF4_827E_11D4_98C4_002018557056__INCLUDED_)
#define AFX_LOGNORMALDISTRIBUTION_H__332F1AF4_827E_11D4_98C4_002018557056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParametricDistribution.h"

/**
 * Parameters nu and tau have the interpretation of mean and sd of
 * underlying Normal distribution.
 */
class LogNormalDistribution : public ParametricDistribution  
{
public:
	LogNormalDistribution(double nu, double tau);
	virtual ~LogNormalDistribution() {};
	virtual double getSample() const;
	virtual double getQuantile(double x) const;
	virtual double getMean() const;
	virtual double getMedian() const;
	virtual double getVariance() const;
	virtual double getCDF(double x) const;
	virtual double getLogPDF(double x) const;
	virtual void   releaseMemory() {};
	virtual double getX(double x) { return x; };
	static bool checkParameters(double nu, double tau);

	void operator= (const LogNormalDistribution & lsd) {this->nu = lsd.nu; this->tau = lsd.tau;}

private:
	double nu;
	double tau;
};

#endif // !defined(AFX_LOGNORMALDISTRIBUTION_H__332F1AF4_827E_11D4_98C4_002018557056__INCLUDED_)
