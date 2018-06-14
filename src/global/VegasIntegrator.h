// VegasIntegrator.h: interface for the VegasIntegrator class.
//
//////////////////////////////////////////////////////////////////////

#include "Function.h"


#if !defined(AFX_VEGASINTEGRATOR_H__ABA9433B_E5BB_46AE_955E_E2F1C246FD8B__INCLUDED_)
#define AFX_VEGASINTEGRATOR_H__ABA9433B_E5BB_46AE_955E_E2F1C246FD8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class VegasIntegrator  
{
public:
	void vegas(double regn[], int ndim, Function &F, int init, unsigned long ncall,
				int itmx, int nprn, double *tgral, double *sd, double *chi2a, long idum);
	VegasIntegrator();
	virtual ~VegasIntegrator();
private:
	double ran2(long *idum);
	void rebin(double rc, int nd, double r[], double xin[], double xi[]);

};

#endif // !defined(AFX_VEGASINTEGRATOR_H__ABA9433B_E5BB_46AE_955E_E2F1C246FD8B__INCLUDED_)
