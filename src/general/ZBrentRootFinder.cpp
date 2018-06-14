// ZBrentRootFinder.cpp: implementation of the ZBrentRootFinder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ZBrentRootFinder.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ZBrentRootFinder::ZBrentRootFinder()
{
	//integrator = new RomberIntegrator();	
}

ZBrentRootFinder::~ZBrentRootFinder()
{
	//delete integrator;
}

double ZBrentRootFinder::FindRoot( Function &F, double x1, double x2, double tol, double Ran )
{
	return zbrent( F, x1, x2, tol, Ran );
}

double ZBrentRootFinder::zbrent( Function &F, double x1, double x2, double tol, double Ran )
{
	int iter;
	double a=x1,b=x2,c=x2,d,e,min1,min2;
	double fa, fb, fc,p,q,r,s,tol1,xm;

	

	//fa = integrator->qromb(F, 0, a) - Ran;
	//fb = integrator->qromb(F, 0, b) - Ran;
	fa = RomberIntegrator::Integrate(F, 0, a) - Ran;
	fb = RomberIntegrator::Integrate(F, 0, b) - Ran;


	if ((fa > 0.0 && fb > 0.0) || (fa < 0.0 && fb < 0.0))
   	return -1; //exit ( 0 ); //nrerror("Root must be bracketed in zbrent");
   fc=fb;
   for (iter=1;iter<=ITMAXX;iter++) {
   	if ((fb > 0.0 && fc > 0.0) || (fb < 0.0 && fc < 0.0)) {
      	c=a;
         fc=fa;
         e=d=b-a;
      }
      if (fabs(fc) < fabs(fb)) {
      	a=b;
         b=c;
         c=a;
         fa=fb;
         fb=fc;
         fc=fa;
      }
      tol1=2.0*EPSS*fabs(b)+0.5*tol;
      xm=0.5*(c-b);
      if (fabs(xm) <= tol1 || fb == 0.0)
      	return b;
      if (fabs(e) >= tol1 && fabs(fa) > fabs(fb)) {
      	s=fb/fa;
      	if (a == c) {
      		p=2.0*xm*s;
         	q=1.0-s;
         }
      	else {
      		q=fa/fc;
         	r=fb/fc;
         	p=s*(2.0*xm*q*(q-r)-(b-a)*(r-1.0));
         	q=(q-1.0)*(r-1.0)*(s-1.0);
      	}
      	if (p > 0.0)
      		q = -q;
      	p=fabs(p);
      	min1=3.0*xm*q-fabs(tol1*q);
      	min2=fabs(e*q);
      	if (2.0*p < (min1 < min2 ? min1 : min2)) {
      		e=d;
         	d=p/q;
      	}
      	else {
      		d=xm;
         	e=d;
      	}
   	}
   	else {
   		d=xm;
      	e=d;
   	}
   	a=b;
   	fa=fb;
   	if (fabs(d) > tol1)
   		b +=d;
   	else
   		b += SIGN(tol1,xm);
   	//fb = integrator->qromb(F, 0, b) - Ran;
	fb = RomberIntegrator::Integrate(F, 0, b) - Ran;
	}
	return -1; //exit ( 0 ); //nrerror("Maximum number of iterations exceeded in zbrent");
	return 0.0;
}

