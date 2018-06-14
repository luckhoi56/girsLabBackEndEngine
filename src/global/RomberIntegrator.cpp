// RomberIntegrator.cpp: implementation of the RomberIntegrator class.
//
//////////////////////////////////////////////////////////////////////


#include "RomberIntegrator.h"
#include <math.h>
#include <stdlib.h>


#define EPS 1.0e-6
#define JMAX 20
#define JMAXP (JMAX+1)
#define K 5

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

RomberIntegrator::RomberIntegrator()
{

}

RomberIntegrator::~RomberIntegrator()
{

}

double RomberIntegrator::Integrate(Function &F, double a, double b)
{
	return qromb( F, a, b);
}


/*Here EPS is the fractional accuracy desired, as determined by the extrapolation error estimate;
JMAX limits the total number of steps; K is the number of points used in the extrapolation.*/

double RomberIntegrator::qromb(Function &F, double a, double b)
//Returns the integral of the function func from a to b. Integration is performed by Romberg's
//method of order 2K, where, e.g., K=2 is Simpson's rule.
{
	int j;
	//void polint(double xa[], double ya[], int n, double x, double *y, double *dy);	
	//double trapzd(double (*func)(double), double a, double b, int n);
	//void nrerror(char error_text[]);
	double ss,dss;
	double s[JMAXP],h[JMAXP+1];// These store the successive trapezoidal approxi-
								//mations and their relative stepsizes. int j;
	h[1]=1.0;
	for (j=1;j<=JMAX;j++) {
		s[j]=trapzd(F,a,b,j);
		if (j >= K) {
			polint(&h[j-K],&s[j-K],K,0.0,&ss,&dss);
			if (fabs(dss) <= EPS*fabs(ss)) return ss;
		}
		h[j+1]=0.25*h[j];
//This is a key step: The factor is 0.25 even though the stepsize is decreased by only
//0.5. This makes the extrapolation a polynomial in h 2 as allowed by equation (4.2.1),
//not just a polynomial in h.
	}
	//nrerror("Too many steps in routine qromb");
	return 0.0;// Never get here.
}


void RomberIntegrator::polint(double xa[], double ya[], int n, double x, double *y, double *dy)
//Given arrays xa[1..n] and ya[1..n], and given a value x, this routine returns a value y, and
//an error estimate dy. IfP(x)is the polynomial of degree N - 1 such that P(xai) =ya i ; i=
//1;: : :;n, then the returned value y = P(x).
{
	int i,m,ns=1;
	double den,dif,dift,ho,hp,w;
	double *c,*d;
	dif=fabs(x-xa[1]);
	c=vector(1,n);
	d=vector(1,n);
	for (i=1;i<=n;i++) {// Here we nd the index ns of the closest table entry,
		if ( (dift=fabs(x-xa[i])) < dif) {
			ns=i;
			dif=dift;
		}
		c[i]=ya[i]; //and initialize the tableau of c's and d's.
		d[i]=ya[i];
	}
	*y=ya[ns--]; //This is the initial approximation to y.
	for (m=1;m<n;m++) {// For each column of the tableau,
		for (i=1;i<=n-m;i++) {// we loop over the current c's and d's and update them.
			ho=xa[i]-x;
			hp=xa[i+m]-x;
			w=c[i+1]-d[i];
			if ( (den=ho-hp) == 0.0) exit(0 );//nrerror("Error in routine polint");
//This error can occur only if two input xa's are (to within roundo) identical.
			den=w/den;
			d[i]=hp*den;// Here the c's and d's are updated.
			c[i]=ho*den;
		}
		*y += (*dy=(2*ns < (n-m) ? c[ns+1] : d[ns--]));
//After each column in the tableau is completed, we decide which correction, c or d,
//we want to add to our accumulating value of y, i.e., which path to take through the
//tableau|forking up or down. We do this in such a way as to take the most \straight
//line" route through the tableau to its apex, updating ns accordingly to keep track of
//where we are. This route keeps the partial approximations centered (insofar as possible)
//on the target x. Thelastdy added is thus the error indication.
	}
	free_vector(d,1,n);
	free_vector(c,1,n);
}


double RomberIntegrator::trapzd(Function &F, double a, double b, int n)
//This routine computes the nth stage of renement of an extended trapezoidal rule. func is input
//as a pointer to the function to be integrated between limits a and b, also input. When called with
//n=1, the routine returns the crudest estimate of
//R
//b
//a f(x)dx. Subsequent calls with n=2,3,...
//(in that sequential order) will improve the accuracy by adding 2 n-2 additional interior points.
{
	double x,tnm,sum,del;
	static double s;
	int it,j;
	if (n == 1) {
		//return (s=0.5*(b-a)*(FUNC(a)+FUNC(b)));
		return (s=0.5*(b-a)*( F.evaluate(Parameters(a)) + F.evaluate(Parameters(b)) ));
	} else {
		for (it=1,j=1;j<n-1;j++) it <<= 1;
		tnm=it;
		del=(b-a)/tnm; //This is the spacing of the points to be added.
		x=a+0.5*del;
		for (sum=0.0,j=1;j<=it;j++,x+=del) sum += F.evaluate(Parameters(x));
		s=0.5*(s+(b-a)*sum/tnm); //This replaces s by its rened value.
		return s;
	}
}

double *RomberIntegrator::vector(int nl,int nh)
{
	double *v;

	v=(double *)malloc((unsigned) (nh-nl+1)*sizeof(double));
	return v-nl;
}

void RomberIntegrator::free_vector(double *v,int nl,int nh)
{
	free((char*) (v+nl));
}