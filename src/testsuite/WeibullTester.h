// WeibullTester.h: interface for the WeibullTester class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WEIBULLTESTER_H__C14FD241_AD9E_4ADA_8EBD_12AA42E9558B__INCLUDED_)
#define AFX_WEIBULLTESTER_H__C14FD241_AD9E_4ADA_8EBD_12AA42E9558B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "redcas_numerical_NativeWeibull.h"

class WeibullTester  
{
public:
	void executeSimpleI();
	WeibullTester();
	virtual ~WeibullTester();
private:
    int handle;


};

#endif // !defined(AFX_WEIBULLTESTER_H__C14FD241_AD9E_4ADA_8EBD_12AA42E9558B__INCLUDED_)
