// UniformBinomialDistribution.h: interface for the UniformBinomialDistribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNIFORMBINOMIALDISTRIBUTION_H__7A48CD2F_871A_43A5_9ECB_BFD2BDE12643__INCLUDED_)
#define AFX_UNIFORMBINOMIALDISTRIBUTION_H__7A48CD2F_871A_43A5_9ECB_BFD2BDE12643__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TabularBinomialSupport.h"
#include "ParametricDistribution.h"

class UniformBinomialDistribution  : public ParametricDistribution  
{
public:
	UniformBinomialDistribution(double min, double max, double k, double n, bool loguni);
	virtual ~UniformBinomialDistribution();
	virtual void   releaseMemory() {};
	double getSample() const;
	double getMedian() const {return getQuantile(0.5);}
	double getMean() const {return mean;}
	double getQuantile(double z) const;
	double getCDF(double x) const;
	double getLogPDF(double x) const;
	double getVariance() const {return var;}
	double getUpperBound() {return hi;}
	double getLowerBound() {return lo;}
private:
	double k;	// number of events
	double n;	// exposure time
	int m;   // 
	double hi;  // upper bound
	double lo;  // lower bound

	double norm;
	double mean;
	double var;
	double lnc;

	TabularBinomialSupport * support;
};

#endif // !defined(AFX_UNIFORMBINOMIALDISTRIBUTION_H__7A48CD2F_871A_43A5_9ECB_BFD2BDE12643__INCLUDED_)
