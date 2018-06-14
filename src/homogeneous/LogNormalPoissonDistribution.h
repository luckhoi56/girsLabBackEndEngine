// LogNormalPoissonDistribution.h: interface for the LogNormalPoissonDistribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGNORMALPOISSONDISTRIBUTION_H__1927951E_8818_4088_972D_135152A9E138__INCLUDED_)
#define AFX_LOGNORMALPOISSONDISTRIBUTION_H__1927951E_8818_4088_972D_135152A9E138__INCLUDED_

#include "SimpleHomogeneous.h"
#include "../parametric/ParametricDistribution.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class LogNormalPoissonDistribution : public ParametricDistribution  
{
public:
	LogNormalPoissonDistribution(double nu, double tau, double k, double t);
	virtual ~LogNormalPoissonDistribution();

	virtual double getSample() const;
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

	double nu;
	double tau;
	double k;
	double T;

	UpdateFunction * ln_posterior;
	UpdateFunction2 * posterior;

	double integral;		// integral
	double ln_integral;		// natural log of integral
	double lowerBound;		// lower bound of integration interval
	double upperBound;		// upper bound of integration interval
	double mode;			// mode of the distribution

	double mean;
	double var;
};

#endif // !defined(AFX_LOGNORMALPOISSONDISTRIBUTION_H__1927951E_8818_4088_972D_135152A9E138__INCLUDED_)
