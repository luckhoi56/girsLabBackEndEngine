// $Header: C:/cvsrepo/ptdev/native/redcas/parametric/ParametricDistribution.h,v 1.2 2001/08/23 16:16:07 fgroen Exp $

// ParametricDistribution.h: interface for the ParametricDistribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARAMETRICDISTRIBUTION_H__28141A73_81E1_11D4_98C3_002018557056__INCLUDED_)
#define AFX_PARAMETRICDISTRIBUTION_H__28141A73_81E1_11D4_98C3_002018557056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GenericDistribution.h"

class ParametricDistribution : public GenericDistribution 
{
public:
	virtual void fillSampleArray(double * values, int size) const;
	virtual double getSample() const;
	virtual double getMedian() const;
	virtual double getMean() const = 0L;
	virtual double getQuantile(double x) const = 0L;
	virtual double getCDF(double x) const = 0L;
	virtual double getPDF(double x) const;
	virtual double getLogPDF(double x) const = 0L;
	virtual double getX(double x) { return x; };
	virtual double getVariance() const = 0L;
	virtual ~ParametricDistribution() {};
};

#endif // !defined(AFX_PARAMETRICDISTRIBUTION_H__28141A73_81E1_11D4_98C3_002018557056__INCLUDED_)
