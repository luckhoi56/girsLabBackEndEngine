// TabularIntegrator.h: interface for the TabularIntegrator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABULARINTEGRATOR_H__C1CD3B8D_91D2_44FC_ABC4_6DE2A5EC128D__INCLUDED_)
#define AFX_TABULARINTEGRATOR_H__C1CD3B8D_91D2_44FC_ABC4_6DE2A5EC128D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TabularIntegrator  
{
public:
	TabularIntegrator();
	virtual ~TabularIntegrator();
	virtual int computeIntegral(double x, double & i0, double & lnc) const = 0;
	virtual int inverseIntegral(double i0, double & x) const = 0;
	virtual int computeIntegrals(double & i0, double & i1, double & i2, double & lnc) const = 0;
	virtual int computeLogIntegrant(double x, double &p, double &lnc) const = 0;
	virtual double getLnNorm();
};

#endif // !defined(AFX_TABULARINTEGRATOR_H__C1CD3B8D_91D2_44FC_ABC4_6DE2A5EC128D__INCLUDED_)
