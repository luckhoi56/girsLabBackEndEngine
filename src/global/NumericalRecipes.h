// NumericalRecipes.h: interface for the NumericalRecipes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NUMERICALRECIPES_H__265CFB2D_3E72_11D4_9881_002018557056__INCLUDED_)
#define AFX_NUMERICALRECIPES_H__265CFB2D_3E72_11D4_9881_002018557056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Parameters.h"
#include "Matrix.h"
#include "Function.h"
#include "BetaDistMLEFunction.h"

#define NTAB 32

class NumericalRecipes  
{
public:
	static void testRAN2();
	static void testSobseq();

	NumericalRecipes();
	virtual ~NumericalRecipes();

	static double f1dim(double x, const Parameters &pcom, const Parameters &xicom, const Function &function) 
		{return function.evaluate(pcom + (xicom * x));}
	static double brent(double ax, double bx, double cx, Parameters &pcom, Parameters &xicom, 
		double tol,double &xmin, const Function &function);
	static void mnbrak(double &ax, double &bx, double &cx, double &fa, double &fb, double &fc, 
		Parameters &pcom, Parameters &xicom, const Function &function);
	static void linmin(Parameters & p, Parameters & xi, double & fret, const Function & function);
	static bool powell(Parameters & p, Matrix & xi, double ftol, int & iter, double & fret,
		const Function & function);
	static void testPowell();

//	void sobseq(int *n, double *x);
	void sobseq(int n, Parameters & sample, bool init);

	double ran2(long *idum);
	static double zbrent(Function &F, double x1, double x2, double tol);
	static void broydn(double x[], int n, int *check, BetaDistMLEFunction & F);
	static void fdjac(int n, double x[], double fvec[], double **df, BetaDistMLEFunction & F);
	static double fmin(double x[], int nn, BetaDistMLEFunction & F, double fvec[]);
	static void lnsrch(int n, double xold[], double fold, double g[],  double p[],
				double x[], double *f, double stpmax, int *check, double fvec[],
				BetaDistMLEFunction & F);
	static void qrdcmp(double **a, int n, double *c, double *d, int *sing);
	static void rsolv(double **a, int n, double d[], double b[]);
	static void qrupdt(double **r, double **qt, int n, double u[], double v[]);
	static void rotate(double **r, double **qt, int n, int i, double a, double b);
	static double *vector(int nl,int nh);
	static void free_vector(double *v,int nl,int nh);
	static double **matrix(int nrl,int nrh,int ncl,int nch);
	static void free_matrix(double **m,int nrl,int nrh,int ncl,int nch);

private:
	// sobseq variables
    static double fac;

    static unsigned long mdeg []; 
    static unsigned long ip [];   
	static unsigned long iv [] ;  

    unsigned long in;
	unsigned long * ix;
	unsigned long ** iu;

	// ran2 variables
	long ran2_idum2;
	long ran2_iy;
	long ran2_iv[NTAB];
};

#endif // !defined(AFX_NUMERICALRECIPES_H__265CFB2D_3E72_11D4_9881_002018557056__INCLUDED_)
