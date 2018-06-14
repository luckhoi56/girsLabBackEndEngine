// TabularLogNormalIntegrator.h: interface for the TabularLogNormalIntegrator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABULARLOGNORMALINTEGRATOR_H__61EDB2CD_2507_4788_84F4_7EAE805E8A69__INCLUDED_)
#define AFX_TABULARLOGNORMALINTEGRATOR_H__61EDB2CD_2507_4788_84F4_7EAE805E8A69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TabularIntegrator.h"

/**
 * Integrator class that supports distributions representing a tabular (uniform) 
 * distribution updated by a lognormal distribution.
 */
class TabularLogNormalIntegrator : public TabularIntegrator  
{
public:
	TabularLogNormalIntegrator(double nu,double tau,int n,double lo,double hi);
	virtual ~TabularLogNormalIntegrator();
	int computeIntegral(double x, double & i0, double & lnc) const;
	int inverseIntegral(double i0, double & x) const;
	int computeIntegrals(double & i0, double & i1, double & i2, double & lnc) const;
	int computeLogIntegrant(double x, double &p, double &lnc) const;
private:
	double nu;	// number of events
	double tau;	// exposure time
	int n;   // 
	double hi;  // upper bound
	double lo;  // lower bound

	double lnc;
//	double loi0; // integral offset
};

#endif // !defined(AFX_TABULARLOGNORMALINTEGRATOR_H__61EDB2CD_2507_4788_84F4_7EAE805E8A69__INCLUDED_)
