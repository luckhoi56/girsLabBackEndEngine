// HookeMinFinder.h: interface for the HookeMinFinder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOOKEMINFINDER_H__7FEB697C_B02D_4119_A2B1_C71A4C901AF3__INCLUDED_)
#define AFX_HOOKEMINFINDER_H__7FEB697C_B02D_4119_A2B1_C71A4C901AF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Function.h"

class HookeMinFinder  
{
public:
	int hooke(Function & f, Parameters & p, bool findmax, bool posonly);
	HookeMinFinder();
	virtual ~HookeMinFinder();

protected:
	double f(double * x, int n);
	double best_nearby(double * delta, double * point, double prevbest, int nvars);
	int hooke(int nvars, double * startpt, double * endpt, double rho,
	  double epsilon, int itermax);
	bool findMax;
	bool posonly;
	Function * function;
	int funevals;
};

#endif // !defined(AFX_HOOKEMINFINDER_H__7FEB697C_B02D_4119_A2B1_C71A4C901AF3__INCLUDED_)
