// RomberIntegrator.h: interface for the RomberIntegrator class.
//
//////////////////////////////////////////////////////////////////////

#include "Function.h"

#if !defined(AFX_ROMBERINTEGRATOR_H__93A05187_5F75_4B01_B283_A077D65B9D64__INCLUDED_)
#define AFX_ROMBERINTEGRATOR_H__93A05187_5F75_4B01_B283_A077D65B9D64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class RomberIntegrator  
{
public:
	static double Integrate(Function &F, double a, double b);
	RomberIntegrator();
	virtual ~RomberIntegrator();

private:
	static double qromb(Function &F, double a, double b);
	static void polint(double xa[], double ya[], int n, double x, double *y, double *dy);
	static double trapzd(Function &F, double a, double b, int n);
	static double *vector(int nl,int nh);
	static void free_vector(double *v,int nl,int nh);
};

#endif // !defined(AFX_ROMBERINTEGRATOR_H__93A05187_5F75_4B01_B283_A077D65B9D64__INCLUDED_)
