// LogNormalBinomialDistribution.h: interface for the LogNormalBinomialDistribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGNORMALBINOMIALDISTRIBUTION_H__F2DE67B3_95AE_471E_963B_EB92F25E2E3B__INCLUDED_)
#define AFX_LOGNORMALBINOMIALDISTRIBUTION_H__F2DE67B3_95AE_471E_963B_EB92F25E2E3B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParametricDistribution.h"
#include "SimpleHomogeneous.h"
#include "BetaDistribution.h"
#include "LogNormalDistribution.h"

class LogNormalBinomialDistribution : public ParametricDistribution  
{
public:
	LogNormalBinomialDistribution(double nu, double tau, double failures, double demands);
	virtual ~LogNormalBinomialDistribution();
	virtual double getSample() const ;
	virtual double getMedian() const {return getQuantile(0.5);}
	virtual double getMean() const {return mean;}
	virtual double getQuantile(double x) const ;
	virtual double getCDF(double x) const;
	virtual double getLogPDF(double x) const;
	virtual double getVariance() const {return var;}
	virtual void   releaseMemory() {};
	double getUpperBound() {return upperBound;}
	double getLowerBound() {return lowerBound;}
private:
	void determineBounds();
	bool delta(LogNormalDistribution & prior, BetaDistribution & like, double x);
	double nu;
	double tau;
	double k;
	double N;

	UpdateFunction * ln_posterior;
	UpdateFunction2 * posterior;

	double integral;
	double ln_integral;
	double lowerBound;
	double upperBound;

	double mean;
	double var;
	double mode;	// approximate location of mode.
};

#endif // !defined(AFX_LOGNORMALBINOMIALDISTRIBUTION_H__F2DE67B3_95AE_471E_963B_EB92F25E2E3B__INCLUDED_)
