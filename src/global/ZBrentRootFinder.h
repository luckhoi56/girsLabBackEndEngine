// ZBrentRootFinder.h: interface for the ZBrentRootFinder class.
//
//////////////////////////////////////////////////////////////////////

#include "Function.h"
#include "RomberIntegrator.h"

#define ITMAXX 100
#define EPSS 3.0e-8
#define SIGN(a,b) ((b) > 0.0 ? fabs(a) : -fabs(a))


#if !defined(AFX_ZBRENTROOTFINDER_H__471B250E_3E0A_453B_A24A_3ED4414150FD__INCLUDED_)
#define AFX_ZBRENTROOTFINDER_H__471B250E_3E0A_453B_A24A_3ED4414150FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ZBrentRootFinder  
{
public:
	static double FindRoot( Function &F, double x1, double x2, double tol, double Ran );
	ZBrentRootFinder();
	virtual ~ZBrentRootFinder();
	//RomberIntegrator *integrator;
private:
	static double zbrent( Function &F, double x1, double x2, double tol, double Ran );

};

#endif // !defined(AFX_ZBRENTROOTFINDER_H__471B250E_3E0A_453B_A24A_3ED4414150FD__INCLUDED_)
