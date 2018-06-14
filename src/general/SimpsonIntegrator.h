// SimpsonIntegrator.h: interface for the SimpsonIntegrator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMPSONINTEGRATOR_H__51965800_E2BD_4E54_9397_A3F412387AB4__INCLUDED_)
#define AFX_SIMPSONINTEGRATOR_H__51965800_E2BD_4E54_9397_A3F412387AB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Function.h"

//
//
//  The correctness of this implementation is in doubt. Needs to be tested / verified.
//
//

class SimpsonIntegrator  
{
public:
	virtual ~SimpsonIntegrator() {};
	// CACM Algorithm 103: Simpson's Rule Integrator
	static void simpson103(double a, double b, Function &f, double &I, double &i, double eps, int N);
	// CACM Algorithm 145: Adaptive Numerical Integration by Simpson's Rule
	static double simpson145(double a, double b, Function &F, double eps, int levels);
	static double simpson145(double a, double b, Function &F, double eps);
	static double simpson145r(Function &F, double a, double da, double Fa, double Fm, double Fb, double absarea, double est, double eps, int level, int levels);

};

#endif // !defined(AFX_SIMPSONINTEGRATOR_H__51965800_E2BD_4E54_9397_A3F412387AB4__INCLUDED_)
