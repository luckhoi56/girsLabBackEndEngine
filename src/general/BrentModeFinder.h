// BrentModeFinder.h: interface for the BrentModeFinder class.
//
//////////////////////////////////////////////////////////////////////

#include "Function.h"

#if !defined(AFX_BRENTMODEFINDER_H__70AF2AC1_8856_42A5_9EB8_60312151175E__INCLUDED_)
#define AFX_BRENTMODEFINDER_H__70AF2AC1_8856_42A5_9EB8_60312151175E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class BrentModeFinder  
{
public:
	void mnbrak(double *ax, double *bx, double *cx, double *fa, double *fb, double *fc,
				Function &F);
	double brent(double ax, double bx, double cx, Function &F, double tol,
							  double *xmin);
	BrentModeFinder();
	virtual ~BrentModeFinder();

};

#endif // !defined(AFX_BRENTMODEFINDER_H__70AF2AC1_8856_42A5_9EB8_60312151175E__INCLUDED_)
