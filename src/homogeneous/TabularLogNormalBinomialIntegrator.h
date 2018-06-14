// TabularLogNormalBinomialIntegrator.h: interface for the TabularLogNormalBinomialIntegrator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABULARLOGNORMALBINOMIALINTEGRATOR_H__11238559_DCFA_4C76_9F43_712EFBECA760__INCLUDED_)
#define AFX_TABULARLOGNORMALBINOMIALINTEGRATOR_H__11238559_DCFA_4C76_9F43_712EFBECA760__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TabularIntegrator.h"

/**
 * Integrator for tabular/uniform distributions updated by a combination of 
 * lognormal and binomial likelihood functions.
 */
class TabularLogNormalBinomialIntegrator  : public TabularIntegrator
{
public:
	TabularLogNormalBinomialIntegrator(double lo, double hi, double nu, double tau, double k, double n, int m, double lnnorm);
	TabularLogNormalBinomialIntegrator(double lo, double hi, double nu, double tau, double k, double n, int m);
	virtual ~TabularLogNormalBinomialIntegrator();
	virtual int computeIntegral(double x, double & i0, double & lnc) const;
	virtual int inverseIntegral(double i0, double & x) const;
	virtual int computeIntegrals(double & i0, double & i1, double & i2, double & lnc) const;
	virtual int computeLogIntegrant(double x, double &p, double &lnc) const;
private:
	double findRestrictedMode(int & location) const;
	double findRestrictedMode() const;
	double findMode() const;
	int inverseSimpsonIntegral(double z, double & x) const;
	bool isLessThanMode(double x) const;

	double lo;			// interval lower bound
	double hi;			// interval upper bound
	double lo_bound;	// lower integration bound.
	double hi_bound;	// higher integration bound.
	double k;			// number of events
	double n;			// number of demands
	double nu;			// mean of underlying normal
	double tau;			// sdev of underlying normal
	double lnnorm;		// log of external normalization factor
	int m;				// 1 if logdensity
	int mode_loc;		// location of mode (left, right, inside)
	double mode;		// location of mode
	double delta;		// log of internal normalization factor
protected:
	void determineBounds(double & lo, double & hi) const;
	double intersectBound(double lo, double hi, double mode, double nu, double tau, double k, double n, int order, double delta) const;
};

#endif // !defined(AFX_TABULARLOGNORMALBINOMIALINTEGRATOR_H__11238559_DCFA_4C76_9F43_712EFBECA760__INCLUDED_)
