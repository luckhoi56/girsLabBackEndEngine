// SimpsonIntegrator.cpp: implementation of the SimpsonIntegrator class.
//
//////////////////////////////////////////////////////////////////////


#include "SimpsonIntegrator.h"
#include <math.h>

/**
 * Implementation of Simpson's Rule Integrator, CACM Algorithm 103
 *
 * INPUTS:
 *  a -> double : lower limit
 *  b -> double : upper limit
 *  f -> Function : integrand
 *  I -> double : integral
 *  i -> double : 
 *  eps -> double : overall tolerance
 *  N -> integer : 
 *
 * COMMENT FROM ORIGINAL ALGORITHM DESCRIPTION
 * This procedure integrates the function f(x) using a modified Simpson's Rule
 * quadrature formula. The quadrature is performed over j subintervals of [a,b]
 * forming the total area I. Convergence in each interval of length (b-a)/2^n is
 * indicated when the relative difference between successive three-point and 
 * five-point area approximations 
 *   A_{3,j} = (b-a)(g0 + 4g2 + g4) / (3*2^(n+2))
 *	 A_{3,j} = (b-a)(g0 + 4g2 + g4) / (3*2^(n+2))
 * is less than or equal to an appropriate portion of the over-all tolerance eps
 * (i.e., |(A_{5,j} - A_{3,j}) / A_{5,j}| <= eps/2^n with n <= N). SIMPSON will
 * reduce the size of each interval until this condition is satisfied.
 *
 * Complete integration over [a,b] is indicated by i = b. A value a <= i < b 
 * indicates that the integration was terminated, leaving I the true area under
 * f in [a,i]. Further integration over [i,b] will necessitate either the assignment
 * of a larger N, a larger eps, or an integral substitution reducing the slope of the
 * integrand in that interval. It is recommended that this procedure be used between
 * known integrand maxima and minima.
 */
void SimpsonIntegrator::simpson103(double a, double b, Function &f, double &I, double &i, double eps, int N)
{
	int m;
	int n;
	double d;
	double h;
	double g[5];
	double A[3];
	double **S = new double *[N + 1];
	for (int i1 = 1; i1 <= N ; i1++) S[i1] = new double [4];

	I = 0;
	i = 0;
	m = 0;
	n = 0;

	g[0] = f.evaluate(Parameters(a));
	g[2] = f.evaluate(Parameters((a + b) / 2));
	g[4] = f.evaluate(Parameters(b));

	A[0] = (b - a) * (g[0] + 4 * g[2] + g[4]) / 2;

AA:	d = pow((double)2,(double)n);
	h = (b - a) / (4 * d);
	g[1] = f.evaluate(Parameters(a + h * (4 * m + 1)));
	g[3] = f.evaluate(Parameters(a + h * (4 * m + 3)));
	A[1] = h * (g[0] + 4 * g[1] + g[2]);
	A[2] = h * (g[2] + 4 * g[3] + g[4]);

	if (fabs(((A[1] + A[2]) - A[0]) / (A[1] + A[2])) > eps / d) {
		m *= 2;
		n += 1;
		if (n > N) goto CC;
		A[0] = A[1];
		S[n][1] = A[2];
		S[n][2] = g[3];
		S[n][3] = g[4];
		g[4] = g[2];
		g[2] = g[1];
		goto AA;
	} else {
		I += (A[1] + A[2]) / 3;
		m += 1;
		i = a + m * (b - a) / d;

BB:		if (m == 2 * (m / 2)) {
			m /= 2;
			n -= 1;
			goto BB;
		}
		
		if (m != 1 || n != 0) {
			A[0] = S[n][1];
			g[0] = g[4];
			g[2] = S[n][2];
			g[4] = S[n][3];
			goto AA;
		}
	}
CC:
	for (int i2 = 1; i2 <= N ; i2++) delete [] S[i2];
}

/**
 * Implementation of Adaptive Simpson's Rule Integrator, CACM Algorithm 145
 *
 * INPUTS:
 *  a -> double : lower limit
 *  b -> double : upper limit
 *  f -> Function : integrand
 *  eps -> double : overall tolerance
 *  levels -> integer : recursion depth 
 *
 * COMMENT FROM ORIGINAL ALGORITHM DESCRIPTION
 * [simpson145] will numerically approximate the integral of the function F
 * between the limits a and b by the application of a modified Simpson's rule. 
 * Although eps is a measure of the relative error of the result, the actual 
 * error may be very much larger (e.g. whenever the answer is small because
 * a positive area cancelled a negative area). The procedure attempts to 
 * minimize the number of function evaluations by using small subdivisions of
 * the interval only where required for the given tolerance.
 *
 * NOTE:
 * Recursion depth has been added as a parameter. The documentation recommends 20.
 */
double SimpsonIntegrator::simpson145(double a, double b, Function &F, double eps, int levels)
{
	int level = 1;
	return simpson145r(F,a,b - a,F.evaluate(a),4 * F.evaluate((a+b)/2),F.evaluate(b),1,1,eps,level,levels);
}

double SimpsonIntegrator::simpson145(double a, double b, Function &F, double eps)
{
	int level = 1;
	double absarea = 1;
	return simpson145r(F,a,b - a,F.evaluate(a),4 * F.evaluate((a+b)/2),F.evaluate(b),absarea,1,eps,level,20);
}

/**
 *	Recursive part of algorithm 145.
 */
double SimpsonIntegrator::simpson145r(Function &F, double a, double da, double Fa, double Fm, double Fb, double absarea, double est, double eps, int level, int levels)
{
	double dx, x1, x2, est1, est2, est3, F1, F2, F3, F4, sum;
	double simpson;
	
	dx = da / 3;
	x1 = a + dx;
	x2 = x1 + dx;
	F1 = 4 * F.evaluate(a + dx / 2);
	F2 = F.evaluate(x1);
	F3 = F.evaluate(x2);
	F4 = 4 * F.evaluate(a + 2.5 * dx);
	est1 = (Fa + F1 + F2) * dx / 6;
	est2 = (F2 + Fm + F3) * dx / 6;
	est3 = (F3 + F4 + Fb) * dx / 6;
	absarea = absarea - fabs(est) + fabs(est1) + fabs(est2) + fabs(est3);
	sum = est1 + est2 + est3;
	level += 1;

	if ((fabs(est - sum) <= eps * absarea && est != 1) || level >= levels) {
		simpson = sum;
	} else {
		simpson = simpson145r(F,a,dx,Fa,F1,F2,absarea,est1,eps/1.7,level,levels) +
			simpson145r(F,x1,dx,F2,Fm,F3,absarea,est2,eps/1.7,level,levels) +
			simpson145r(F,x2,dx,F3,F4,Fb,absarea,est3,eps/1.7,level,levels);
		level -= 1;
	}

	return simpson;
}