// SLATECInterface.h: interface for the SLATECInterface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLATECINTERFACE_H__37ABEAB0_A668_4028_B8BB_C6C8BEE6B173__INCLUDED_)
#define AFX_SLATECINTERFACE_H__37ABEAB0_A668_4028_B8BB_C6C8BEE6B173__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class SLATECInterface  
{
public:
	static double invExponentialIntegralEi(double ei, double x0 = -1);
	static double exponentialIntegralEi(double x);
	SLATECInterface();
	virtual ~SLATECInterface();

};

#endif // !defined(AFX_SLATECINTERFACE_H__37ABEAB0_A668_4028_B8BB_C6C8BEE6B173__INCLUDED_)
