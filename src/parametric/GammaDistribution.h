// $Header: C:/cvsrepo/ptdev/native/redcas/parametric/GammaDistribution.h,v 1.2 2001/05/04 21:27:36 fgroen Exp $

// GammaDistribution.h: interface for the GammaDistribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMMADISTRIBUTION_H__28141A75_81E1_11D4_98C3_002018557056__INCLUDED_)
#define AFX_GAMMADISTRIBUTION_H__28141A75_81E1_11D4_98C3_002018557056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * Class implementing the two parameter Gamma distribution.
 *
 * The Gamma distribution has can be given the interpretation
 * of modeling the time to arrival of the n-th event, assuming
 * a poisson distribution of events through time. Within that 
 * interpretation, the parameters in this class have the 
 * following interpretation:
 *
 *  alpha: the number of events
 *  beta: mean time to arrival between events.
 *
 * Consequently, the mean is computed as beta * alpha.
 */
#include "ParametricDistribution.h"

class GammaDistribution : public ParametricDistribution  
{
public:
	GammaDistribution(double beta, double alpha);
	virtual ~GammaDistribution() {}
	virtual double getSample() const;
	virtual double getVariance() const;
	virtual double getQuantile(double x) const;
	virtual double getMean() const;
	virtual double getCDF(double x) const;
	virtual double getLogPDF(double x) const;
	static bool checkParameters(double beta, double alpha);
	virtual void   releaseMemory() {};
	virtual double getX(double x) { return x; };
	static void testGammaDeviates();
private:
	static double gammaln(double x);
	double beta;
	double alpha;
	double ln_gamma;
};

#endif // !defined(AFX_GAMMADISTRIBUTION_H__28141A75_81E1_11D4_98C3_002018557056__INCLUDED_)
