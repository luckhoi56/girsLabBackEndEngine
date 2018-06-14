// $Header: C:/cvsrepo/ptdev/native/redcas/homogeneous/TabularLogNormalPoissonIntegrator.h,v 1.4 2002/07/08 00:23:05 fgroen Exp $
//
// HISTORY:
//
// 7/6/02 (FJG): Modified the density function by multiplying it by x. The purpose
//       of this modification is a correction for the lognormal updating: a tabular
//       distribution updated by a lognormal is NOT a lognormal, but instead of the
//       form Lognormal * x.
//
// TabularLogNormalPoissonIntegrator.h: interface for the TabularLogNormalPoissonIntegrator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABULARLOGNORMALPOISSONINTEGRATOR_H__69703A59_C292_4F11_B792_2897D3BE8CB5__INCLUDED_)
#define AFX_TABULARLOGNORMALPOISSONINTEGRATOR_H__69703A59_C292_4F11_B792_2897D3BE8CB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TabularIntegrator.h"

class TabularLogNormalPoissonIntegrator : public TabularIntegrator  
{
public:
	TabularLogNormalPoissonIntegrator(double lo, double hi, double nu, double tau, double k, double t, int m, double lnnorm);
	TabularLogNormalPoissonIntegrator(double lo, double hi, double nu, double tau, double k, double t, int m);
	virtual ~TabularLogNormalPoissonIntegrator();
	virtual int computeIntegral(double x, double & i0, double & lnc) const;
	virtual int inverseIntegral(double i0, double & x) const;
	virtual int computeIntegrals(double & i0, double & i1, double & i2, double & lnc) const;
	virtual int computeLogIntegrant(double x, double &p, double &lnc) const;
private:
	static double computeProductLog(double x);
	double findRestrictedMode(int & location) const;
	double findRestrictedMode() const;
	double findMode() const;
	int inverseSimpsonIntegral(double z, double & x) const;
	bool isLessThanMode(double x) const;
	void determineBounds(double &lob, double &hib) const;
	double intersectBound(double lo, double hi, double mode, double nu, double tau, double k, double t, int order, double delta) const;

	double lo;			// interval lower bound
	double hi;			// interval upper bound

	double lo_bound;
	double hi_bound;

	double k;			// number of events
	double t;			// total time
	double nu;			// mean of underlying normal
	double tau;			// sdev of underlying normal
	double lnnorm;		// log of external normalization factor
	int m;				// 1 if logdensity
	int mode_loc;		// location of mode (left, right, inside)
	double mode;		// location of mode
	double delta;		// log of internal normalization factor
};

#endif // !defined(AFX_TABULARLOGNORMALPOISSONINTEGRATOR_H__69703A59_C292_4F11_B792_2897D3BE8CB5__INCLUDED_)