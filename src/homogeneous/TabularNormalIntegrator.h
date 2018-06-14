// TabularNormalIntegrator.h: interface for the TabularNormalIntegrator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABULARNORMALINTEGRATOR_H__C2EF3B8B_AD34_4DA4_88F6_CDCF178387B9__INCLUDED_)
#define AFX_TABULARNORMALINTEGRATOR_H__C2EF3B8B_AD34_4DA4_88F6_CDCF178387B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TabularIntegrator.h"

class TabularNormalIntegrator : public TabularIntegrator  
{
public:
	TabularNormalIntegrator(double nu,double tau,int n,double lo,double hi);
	virtual ~TabularNormalIntegrator();
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
	double loi0; // integral offset
};

#endif // !defined(AFX_TABULARNORMALINTEGRATOR_H__C2EF3B8B_AD34_4DA4_88F6_CDCF178387B9__INCLUDED_)
