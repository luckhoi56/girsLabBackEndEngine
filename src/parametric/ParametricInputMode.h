// ParametricInputMode.h: interface for the ParametricInputMode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARAMETRICINPUTMODE_H__8F716DD8_4CE9_4A04_8524_EF14B35B7BDE__INCLUDED_)
#define AFX_PARAMETRICINPUTMODE_H__8F716DD8_4CE9_4A04_8524_EF14B35B7BDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ParametricInputMode  
{
public:
	virtual int convert(double i1, double i2, double & o2, double & o3) = 0;
	ParametricInputMode();
	virtual ~ParametricInputMode();

};

#endif // !defined(AFX_PARAMETRICINPUTMODE_H__8F716DD8_4CE9_4A04_8524_EF14B35B7BDE__INCLUDED_)
