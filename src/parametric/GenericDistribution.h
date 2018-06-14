// $Header: C:/cvsrepo/ptdev/native/redcas/parametric/GENERICDISTRIBUTION.h,v 1.2 2001/08/23 16:16:07 fgroen Exp $

// GENERICDISTRIBUTION.h: interface for the GENERICDISTRIBUTION class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GENERICDISTRIBUTION_H__28141A73_81E1_11D4_98C3_002018557056__INCLUDED_)
#define AFX_GENERICDISTRIBUTION_H__28141A73_81E1_11D4_98C3_002018557056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class GenericDistribution 
{
public:
	virtual void fillSampleArray(double * values, int size) const;
	virtual double getSample() const;
	virtual double getMedian() const;
	virtual double getMean() const = 0L;
	virtual double getQuantile(double x) const = 0L;
	virtual double getCDF(double x) const = 0L;
	virtual double getPDF(double x) const;
	virtual double getX(double x) const { return x; };
	virtual double getLogPDF(double x) const = 0L;
	virtual double getVariance() const = 0L;
	virtual void   releaseMemory() = 0L;
	virtual ~GenericDistribution() {};
};

#endif // !defined(AFX_GENERICDISTRIBUTION_H__28141A73_81E1_11D4_98C3_002018557056__INCLUDED_)
