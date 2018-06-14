// TabularPoissonSupport.h: interface for the TabularPoissonSupport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABULARPOISSONSUPPORT_H__0E890C5B_C255_4FD7_B973_1079E4B74A40__INCLUDED_)
#define AFX_TABULARPOISSONSUPPORT_H__0E890C5B_C255_4FD7_B973_1079E4B74A40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TabularIntegrator.h"

/**
 * Support class for tabular-poisson type distributions, including 
 * uniform-poisson, loguniform-poisson. Support functions consist of
 * the computation of various integrals required to determine mean, sdev,
 * as well as a inverse integral function.
 */

class TabularPoissonSupport : public TabularIntegrator
{
public:
	int computeIntegral(double x, double & i0, double & lnc) const;
	int inverseIntegral(double i0, double & x) const;
	int computeIntegrals(double & i0, double & i1, double & i2, double & lnc) const;
	int computeLogIntegrant(double x, double &p, double &lnc) const;
	TabularPoissonSupport(double k, double t, int m, double lo, double hi);
	TabularPoissonSupport(double k, double t, int m, double lo, double hi, double gammaln);

	virtual ~TabularPoissonSupport();
private:
	double k;	// number of events
	double t;	// exposure time
	int n;   // 
	double hi;  // upper bound
	double lo;  // lower bound

	double lnc;
	double loi0; // integral offset

};

#endif // !defined(AFX_TABULARPOISSONSUPPORT_H__0E890C5B_C255_4FD7_B973_1079E4B74A40__INCLUDED_)
