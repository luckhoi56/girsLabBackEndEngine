// UniformPoissonDistribution.h: interface for the UniformPoissonDistribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNIFORMPOISSONDISTRIBUTION_H__BC3A6A20_3BF8_4E71_B4C6_F3E4B9CEEC48__INCLUDED_)
#define AFX_UNIFORMPOISSONDISTRIBUTION_H__BC3A6A20_3BF8_4E71_B4C6_F3E4B9CEEC48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TabularPoissonSupport.h"
#include "../parametric/ParametricDistribution.h"

class UniformPoissonDistribution : public ParametricDistribution   
{
public:
	UniformPoissonDistribution(double min, double max, double k, double t, bool loguni);
	virtual ~UniformPoissonDistribution();
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
	double t;	// exposure time
	int n;   // 
	double hi;  // upper bound
	double lo;  // lower bound

	double norm;
	double mean;
	double var;

	TabularPoissonSupport * support;
};

#endif // !defined(AFX_UNIFORMPOISSONDISTRIBUTION_H__BC3A6A20_3BF8_4E71_B4C6_F3E4B9CEEC48__INCLUDED_)
