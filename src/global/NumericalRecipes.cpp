// NumericalRecipes.cpp: implementation of the NumericalRecipes class.
//
//////////////////////////////////////////////////////////////////////

/****************************************************
 *
 *  Bayesian Weibull Sampler.
 *
 *	(c) 2000, Frank Groen. All Rights Reserved.
 *
 ****************************************************/


#include "NumericalRecipes.h"
#include <math.h>
#include <stdlib.h>

#if defined _DEBUG
#include <iostream>
//#include "TestFunction.h"
//#include "NumericalReference.h"
#include "SampleSet.h"
#include "BinnedSampleSpace.h"
#include "HyperIndex.h"
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define MAXITER 100
#define ITMAX 200
#define ZEPS	1e-10
#define CGOLD 0.3819660
#define SIGN(a,b) ((b) > 0.0 ? fabs(a) : -fabs(a))
#define SHFT(a,b,c,d) (a)=(b);(b)=(c);(c)=(d);
#define GOLD 1.618034
#define GLIMIT 100.0
#define TINY	1e-30
#define SQR(a) ((a)*(a))
#define TOL 2.0e-4
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define IMIN(a,b) ((a)<(b) ? (a) : (b))
#define MAXBIT 30  // number of bits of which samples are generated
#define MAXDIM 6   // maximum dimension of samples

// ran2 defines
#define IMIN(a,b) ((a)<(b) ? (a) : (b))
#define IM1 2147483563
#define IM2 2147483399
#define AM (1.0/IM1)
#define IMM1 (IM1-1)
#define IA1 40014
#define IA2 40692
#define IQ1 53668
#define IQ2 52774
#define IR1 12211
#define IR2 3791
#define NDIV (1+IMM1/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

// Zbrent defines
#define ITMAXX 100
#define EPSS 3.0e-8

// Broydn defines
#define MAXITS 200
#define TOLF 1.0e-4
#define TOLX EPS
#define STPMX 100.0
#define TOLMIN 1.0e-6
#define FREERETURN {free_vector(fvec,1,n);free_vector(xold,1,n);\
free_vector(w,1,n);free_vector(t,1,n);free_vector(s,1,n);\
free_matrix(r,1,n,1,n);free_matrix(qt,1,n,1,n);free_vector(p,1,n);\
free_vector(g,1,n);free_vector(fvcold,1,n);free_vector(d,1,n);\
free_vector(c,1,n);return;}
#define FMAX(a,b) (a)>(b)?(a):(b)


// Line Search define
#define ALF 1.0e-4



/*************************************************************************/

// sobseq static initialization
double NumericalRecipes::fac = 1.0 / (1L << MAXBIT);
unsigned long NumericalRecipes::mdeg [MAXDIM + 1] = {0, 1, 2, 3, 3, 4, 4};
unsigned long NumericalRecipes::ip [MAXDIM + 1] = {0, 0, 1, 1, 2, 1, 4};
unsigned long NumericalRecipes::iv [MAXDIM * MAXBIT + 1] = {
	0,1,1,1,1,1,1,3,1,3,3,1,1,5,7,7,3,3,5,15,11,5,15,13,9
};

/*************************************************************************/

NumericalRecipes::NumericalRecipes()
{
	// sobseq non-static variable initialization
	ix = new unsigned long [MAXDIM + 1];
	iu = new unsigned long * [MAXBIT + 1];

	// ran2 initialization
	ran2_idum2 = 123456789;
	ran2_iy = 0;
}

NumericalRecipes::~NumericalRecipes()
{
#ifdef _DEBUG
//	cout << "CLEANING UP NRC" << endl;
#endif
	delete ix;
	delete iu;
}

/*
 * All of the following functions have been taken from Numerical Recipes in C
 *
 * This code is not to be made available to the public in any way.
 */

double NumericalRecipes::brent(double ax, double bx, double cx , 
							   Parameters &pcom, Parameters &xicom,
							   double tol,double &xmin, const Function &function)
{

	int iter;
	double a, b, d, etemp, fu, fv, fw, fx, p, q, r, tol1, tol2, u, v, w, x, xm;
	double e = 0.0;

	a = ((ax < cx) ? ax : cx);
	b = ((ax > cx) ? ax : cx);
   
	x = w = v = bx;

	fw = fv = fx = f1dim(x, pcom, xicom, function);
   
	for(iter = 1; iter <= ITMAX; iter++)
	{
		xm = 0.5 * (a + b);
        tol2 = 2.0 * (tol1 = tol * fabs(x) + ZEPS);
         
        if(fabs(x - xm) <= (tol2 - 0.5 * (b - a)))
        {
			xmin = x;
            return fx;
        }
         
        if(fabs(e) > tol1)
        {
			r = (x - w) * (fx - fv);
            q = (x - v) * (fx - fw);
            p = (x - v) * q - (x - w) * r;
            q = 2.0 * (q - r);
               
            if(q > 0.0)
				p = -p;

            q = fabs(q);
            etemp = e;
               
            /* compiler warning for following line: uninitialized autovariable (FJG) */
            // if (trap) printf("! uninitialized variable used (1)\n");
            e = d;
               
            if(fabs(p) >= fabs(0.5 * q * etemp) || p <= q * (a - x) || p >= q * (b - x)) {
				d = CGOLD * (e = (x >= xm ? a - x : b - x));
            } else {
                d = p / q;
                u = x + d;
                     
                if(u - a < tol2 || b - u < tol2)
					d = SIGN(tol1, xm - x);
            }
        } else {
            d = CGOLD * (e = (x >= xm ? a - x : b - x));
        }

        u = (fabs(d) >= tol1 ? x + d : x + SIGN(tol1, d));

        fu = f1dim(u, pcom, xicom, function);
         
        if (fu <= fx) {
			if (u >= x)
                a = x;
            else
                b = x;
            SHFT(v,w,x,u)
			SHFT(fv,fw,fx,fu)
		} else {
			if(u < x)
				a = u;
            else
                b = u;
                     
            if (fu <= fw || w == x) {
				v = w;
                w = u;
                fv = fw;
                fw = fu;
            } else if (fu <= fv || v == x || v == w) {
                v = u;
                fv = fu;
            }
        }
    }
         
    //	nrerror("Too many iterations in BRENT");
    xmin = x;
    return fx;
}
      
void NumericalRecipes::mnbrak(double &ax, double &bx, double &cx, double &fa, 
							  double &fb, double &fc, Parameters &pcom, 
							  Parameters &xicom, const Function &function)
{


	double ulim, u, r, q, fu, dum;
    fa = f1dim(ax, pcom, xicom, function);
    fb = f1dim(bx, pcom, xicom, function);

    if(fb > fa) {
		SHFT(dum,ax,bx,dum)
		SHFT(dum,fb,fa,dum)
	}
    
	cx = bx + GOLD * (bx - ax);
    fc = f1dim(cx,pcom,xicom,function);
               
    while (fb > fc) {



		r = (bx - ax) * (fb - fc);
        q = (bx - cx) * (fb - fa);
        u = (bx) - ((bx - cx) * q - (bx - ax) * r) / 
			(2.0 * SIGN(MAX(fabs(q - r), TINY), q - r));   
        /* macro evaluate function call twice */
        ulim = bx + GLIMIT * (cx - bx);
                     
        if ((bx - u) * (u - cx) > 0.0) {
			fu = f1dim(u, pcom, xicom, function);
                           
            if (fu < fc) {
				ax = bx;
                bx = u;
                fa = fb;
                fb = fu;
                return;
            } else if (fu > fb) {
				cx = u;
                fc = fu;
                return;
            }

            u = cx + GOLD * (cx - bx);
            fu = f1dim(u, pcom, xicom, function);

		} else if ((cx - u) * (u - ulim) > 0.0) {
			fu = f1dim(u, pcom, xicom, function);

            if(fu < fc) {
				SHFT(bx,cx,u,cx+GOLD*(cx-bx))
				SHFT(fb,fc,fu,f1dim(u,pcom,xicom,function))
			}
        } else if((u - ulim) * (ulim - cx) >= 0.0) {
			u = ulim;
            fu = f1dim(u, pcom, xicom, function);
		} else {
            u = (cx) + GOLD * (cx - bx);
            fu = f1dim(u, pcom, xicom, function);
        }
		SHFT(ax,bx,cx,u)
		SHFT(fa,fb,fc,fu)
	}
}
                        
void NumericalRecipes::linmin(Parameters & p, Parameters & xi, double & fret,
							  const Function & function)
{


    double xx, xmin,bx, ax;
	double fa = 0.0;
	double fb = 0.0;
	double fx = 0.0;

    Parameters pcom(p);
    Parameters xicom(xi);
    
    ax = 0.0;
    xx = 1.0;
    bx = 2.0;

    mnbrak(ax, xx, bx, fa, fx, fb, pcom, xicom, function);

    fret = brent(ax, xx, bx, pcom, xicom, TOL, xmin, function);

	xi = xi * xmin;
	p = p + xi;
}
                        
bool NumericalRecipes::powell(Parameters & p, Matrix & xi, double ftol, int & iter,
							  double & fret, const Function & function)
{
	int i, ibig;
    double t, fptt, fp1, del;

	int n = p.getDimension();

    Parameters pt(p);
	Parameters xit(n);

	fret = function.evaluate(p);
                           
	for(iter = 1; ; ++iter) {


        fp1 = fret;
        ibig = 0;   // direction of biggest decrease
        del = 0.0;   // value of biggest decrease
                                 
        for(i = 0; i < n; i++) {
            fptt = fret;
			Parameters column = xi.getColumn(i);
            linmin(p, column, fret, function);   // minimize along direction i
            if(fabs(fptt - fret) > del) {   // check for biggest decrease
				del = fabs(fptt - fret);
                ibig = i;
            }
        }
                                 
		if(2.0 * fabs(fp1 - fret) <= ftol * (fabs(fp1) + fabs(fret))) {
            return true;
        }

		if (iter > MAXITER) {
			return false;
		}

        fptt = function.evaluate(p * 2 - pt);
		pt = p;

		if(fptt < fp1) {

			t = 2.0 * (fp1 - 2.0 * fret + fptt) * SQR(fp1 - fret - del) - 
				del * SQR(fp1 - fptt);
                                       
			if(t < 0.0) {
				xit = p - pt;
				linmin(p, xit, fret, function);
				xi.setColumn(ibig,xit);
			}
        }
    }
}

void NumericalRecipes::sobseq(int n, Parameters & sample, bool init)
{
	unsigned long j;
    unsigned long i, im, ipp;
	long k, l;

	if (init) {

		for (k = 1;k <= MAXDIM;k++) ix[k] = 0;

        in = 0;

        if(iv[1] != 1) return;

        for(j = 1, k = 0; j <= MAXBIT; j++, k += MAXDIM)
			iu[j] = & iv[k];

        for(k = 1;k <= MAXDIM;k++) {

            for(j = 1; j <= mdeg[k]; j++)
				iu[j][k] <<= (MAXBIT - j);

			for(j = mdeg[k] + 1;j <= MAXBIT;j++) {
				ipp = ip[k];
                i = iu[j - mdeg[k]][k];
                i ^= (i >> mdeg[k]);

                for(l = mdeg[k] - 1;l >= 1;l--) {
					if(ipp & 1)
						i ^= iu[j - 1][k];
                    ipp >>= 1;
				}
                iu[j][k] = i;
            }
        }
	} else {
		im = in++;
        for(j = 1;j <= MAXBIT;j++) {
			if(!(im & 1))
				break;
            im >>= 1;
		}

		im = (j - 1) * MAXDIM;

		for(k = 1;k <= IMIN(n, MAXDIM);k++) {
			ix[k] ^= iv[im + k];
			sample.setValue(k-1,ix[k] * fac);
		}
    }
}

double NumericalRecipes::ran2(long *idum)
{
	int j;
	long k;
	double temp;
	
	if (*idum<=0) {

		if (-(*idum)<1)
			*idum=1;
		else
			*idum=-(*idum);

		ran2_idum2=(*idum);

		for(j = NTAB + 7; j >= 0; j--) {

			k= (*idum) / IQ1;

			*idum = IA1 * (*idum - k * IQ1) - k * IR1;
			if (*idum < 0) 
				*idum += IM1;
			if (j < NTAB)
				ran2_iv[j] = *idum;
		}

		ran2_iy = iv[0];
	}

	k = (*idum) / IQ1;
	*idum = IA1 * (*idum - k * IQ1) - k * IR1;

	if (*idum < 0)
		*idum += IM1;

	k = (*idum) / IQ2;
	ran2_idum2 = IA2 * (ran2_idum2 - k * IQ2) - k * IR2;

	if (ran2_idum2 < 0)
		ran2_idum2 += IM2;

	j = ran2_iy / NDIV;
	ran2_iy = ran2_iv[j] - ran2_idum2;
	ran2_iv[j] = *idum;

	if (ran2_iy < 1) 
		ran2_iy += IMM1;

	if ((temp = AM * ran2_iy) > RNMX)
		return RNMX;
	else 
		return temp;
}

double NumericalRecipes::zbrent(Function &F, double x1, double x2, double tol)
{
	int iter;
	double a=x1,b=x2,c=x2,d,e,min1,min2;
	double fa, fb, fc,p,q,r,s,tol1,xm;

	fa = F.evaluate(a);
	fb = F.evaluate(b);

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
   		fb = F.evaluate(b);
	}
	return -1; //exit ( 0 ); //nrerror("Maximum number of iterations exceeded in zbrent");
}


void NumericalRecipes::broydn(double x[], int n, int *check, BetaDistMLEFunction & F)
{
	int i,its,j,k,restrt,sing,skip;
	double den,f,fold,stpmax,sum,temp,test,*c,*d,*fvcold;
	double *g,*p,**qt,**r,*s,*t,*w,*xold, *fvec;

	c=vector(1,n);
	d=vector(1,n);
	fvcold=vector(1,n);
	g=vector(1,n);
	p=vector(1,n);
	qt=matrix(1,n,1,n);
	r=matrix(1,n,1,n);
	s=vector(1,n);
	t=vector(1,n);
	w=vector(1,n);
	xold=vector(1,n);
	fvec = vector(1,n);

	
	f=fmin(x, n, F, fvec);// The vector fvec is also computed by this call.
	test=0.0;
	for (i=1;i<=n;i++) 
		if (fabs(fvec[i]) > test)test=fabs(fvec[i]);
	if (test < 0.01*TOLF) {
		*check=0;
		FREERETURN
	}
	for (sum=0.0,i=1;i<=n;i++) sum += SQR(x[i]); //Calculate stpmax for line searches.
	stpmax=STPMX*FMAX(sqrt(sum),(double)n);
	restrt=1; //Ensure initial Jacobian gets computed.
	for (its=1;its<=MAXITS;its++) { //Start of iteration loop.
		if (restrt) {
			fdjac(n,x,fvec,r,F);// Initialize or reinitialize Jacobian in r.
			qrdcmp(r,n,c,d,&sing);
			if (sing) exit(-1); //nrerror("singular Jacobian in broydn");
			for (i=1;i<=n;i++) { //Form Q T explicitly.
				for (j=1;j<=n;j++) qt[i][j]=0.0;
				qt[i][i]=1.0;
			}
			for (k=1;k<n;k++) {
				if (c[k]) {
					for (j=1;j<=n;j++) {
						sum=0.0;
						for (i=k;i<=n;i++)
							sum += r[i][k]*qt[i][j];
						sum /= c[k];
						for (i=k;i<=n;i++)
							qt[i][j] -= sum*r[i][k];
					}
				}
			}
			for (i=1;i<=n;i++) { //Form R explicitly.
				r[i][i]=d[i];
				for (j=1;j<i;j++) r[i][j]=0.0;
			}
		}
		else { //Carry out Broyden update.
			for (i=1;i<=n;i++) s[i]=x[i]-xold[i];
			for (i=1;i<=n;i++) { 
				for (sum=0.0,j=i;j<=n;j++) sum += r[i][j]*s[j];
				t[i]=sum;
			}
			skip=1;
			for (i=1;i<=n;i++) { 
				for (sum=0.0,j=1;j<=n;j++) sum += qt[j][i]*t[j];
				w[i]=fvec[i]-fvcold[i]-sum;
				if (fabs(w[i]) >= EPS*(fabs(fvec[i])+fabs(fvcold[i]))) skip=0;
				else w[i]=0.0;
			}
			if (!skip) {
				for (i=1;i<=n;i++) { 
					for (sum=0.0,j=1;j<=n;j++) sum += qt[i][j]*w[j];
					t[i]=sum;
				}
				for (den=0.0,i=1;i<=n;i++) den += SQR(s[i]);
				for (i=1;i<=n;i++) s[i] /= den;
				qrupdt(r,qt,n,t,s); 
				for (i=1;i<=n;i++) {
					if (r[i][i] == 0.0) exit(-1); //nrerror("r singular in broydn");
					d[i]=r[i][i];
				}
			}
		}
		for (i=1;i<=n;i++) { 
			for (sum=0.0,j=1;j<=n;j++) sum += qt[i][j]*fvec[j];
			g[i]=sum;
		}
		for (i=n;i>=1;i--) {
			for (sum=0.0,j=1;j<=i;j++) sum += r[j][i]*g[j];
			g[i]=sum;
		}
		for (i=1;i<=n;i++) { 
			xold[i]=x[i];
			fvcold[i]=fvec[i];
		}		
		fold=f; 
		for (i=1;i<=n;i++) {
			for (sum=0.0,j=1;j<=n;j++) sum += qt[i][j]*fvec[j];
			p[i] = -sum;
		}
		rsolv(r,n,d,p); 
		//lnsrch(n,xold,fold,g,p,x,&f,stpmax,check,fmin);
		lnsrch(n,xold,fold,g,p,x,&f,stpmax,check, fvec, F);
		test=0.0; 
		for (i=1;i<=n;i++)
			if (fabs(fvec[i]) > test) test=fabs(fvec[i]);
		if (test < TOLF) {
			*check=0;
			FREERETURN
		}
		if (*check) { 
			if (restrt) FREERETURN 
			else {
				test=0.0; 
				den=FMAX(f,0.5*n);
				for (i=1;i<=n;i++) {
					temp=fabs(g[i])*FMAX(fabs(x[i]),1.0)/den;
					if (temp > test) test=temp;
				}
				if (test < TOLMIN) FREERETURN
				else restrt=1;
			}
		}
		else { 
			restrt=0;
			test=0.0;
			for (i=1;i<=n;i++) {
				temp=(fabs(x[i]-xold[i]))/FMAX(fabs(x[i]),1.0);
				if (temp > test) test=temp;
			}
			if (test < TOLX) FREERETURN
		}
	}
	exit(-1); //nrerror("MAXITS exceeded in broydn");
	FREERETURN
}


void NumericalRecipes::fdjac(int n, double x[], double fvec[], double **df,
							 BetaDistMLEFunction & F)
{
	int i,j;
	double h,temp,*f;
	f=vector(1,n);
	for (j=1;j<=n;j++) {
		temp=x[j];
		h=EPS*fabs(temp);
		if (h == 0.0) h=EPS;
		x[j]=temp+h; 
		h=x[j]-temp;
		//(*vecfunc)(n,x,f);
		//f = F.evaluate(x[0], x[1]);
		F.evaluate(x[0], x[1], f);
		x[j]=temp;
		for (i=1;i<=n;i++) df[i][j]=(f[i]-fvec[i])/h; 
	}
	free_vector(f,1,n);
}


double NumericalRecipes::fmin(double x[], int nn, BetaDistMLEFunction & F,
							  double fvec[])
{
	int i;
	double sum;
	//(*nrfuncv)(nn,x,fvec);
	//fvec = F.evaluate(x[0], x[1]);
	F.evaluate(x[0], x[1], fvec);
	for (sum=0.0,i=1;i<=nn;i++) sum += SQR(fvec[i]);
	return 0.5*sum;
}


void NumericalRecipes::lnsrch(int n, double xold[], double fold, double g[], 
							  double p[], double x[], double *f, double stpmax,
							  int *check, double fvec[], BetaDistMLEFunction & F)
{
	int i;
	double a,alam,alam2,alamin,b,disc,f2,rhs1,rhs2,slope,sum,temp,
	
	test,tmplam;
	*check=0;
	for (sum=0.0,i=1;i<=n;i++) sum += p[i]*p[i];
	sum=sqrt(sum);
	if (sum > stpmax)
	for (i=1;i<=n;i++) p[i] *= stpmax/sum;
	for (slope=0.0,i=1;i<=n;i++)
		slope += g[i]*p[i];
	if (slope >= 0.0) exit(-1); //nrerror("Roundoff problem in lnsrch.");
	test=0.0;
	for (i=1;i<=n;i++) {
		temp=fabs(p[i])/FMAX(fabs(xold[i]),1.0);
		if (temp > test) test=temp;
	}
	alamin=TOLX/test;
	alam=1.0;
	for (;;) {
		for (i=1;i<=n;i++) x[i]=xold[i]+alam*p[i];
		//*f=(*func)(x);
		*f=fmin(x, n, F, fvec);
		if (alam < alamin) {
			for (i=1;i<=n;i++) x[i]=xold[i];
			*check=1;
			return;
		}
		else
			if (*f <= fold+ALF*alam*slope) return;
			else {
				if (alam == 1.0)
				tmplam = -slope/(2.0*(*f-fold-slope)); 
				else {
					rhs1 = *f-fold-alam*slope;
					rhs2=f2-fold-alam2*slope;
					a=(rhs1/(alam*alam)-rhs2/(alam2*alam2))/(alam-alam2);
					b=(-alam2*rhs1/(alam*alam)+alam*rhs2/(alam2*alam2))/(alam-alam2);
					if (a == 0.0) tmplam = -slope/(2.0*b);
					else {
						disc=b*b-3.0*a*slope;
						if (disc < 0.0) tmplam=0.5*alam;
						else
							if (b <= 0.0) tmplam=(-b+sqrt(disc))/(3.0*a);
							else tmplam=-slope/(b+sqrt(disc));
					}
					if (tmplam > 0.5*alam)
						tmplam=0.5*alam;
				}	
			}
		alam2=alam;
		f2 = *f;
		alam=FMAX(tmplam,0.1*alam);
	}
}


void NumericalRecipes::qrdcmp(double **a, int n, double *c, double *d, int *sing)
{
	int i,j,k;
	double scale,sigma,sum,tau;
	*sing=0;
	for (k=1;k<n;k++) {
		scale=0.0;
		for (i=k;i<=n;i++) scale=FMAX(scale,fabs(a[i][k]));
		if (scale == 0.0) { 
			*sing=1;
			c[k]=d[k]=0.0;
		}
		else {
			for (i=k;i<=n;i++) a[i][k] /= scale;
			for (sum=0.0,i=k;i<=n;i++) sum += SQR(a[i][k]);
			sigma=SIGN(sqrt(sum),a[k][k]);
			a[k][k] += sigma;
			c[k]=sigma*a[k][k];
			d[k] = -scale*sigma;
			for (j=k+1;j<=n;j++) {
				for (sum=0.0,i=k;i<=n;i++) sum += a[i][k]*a[i][j];
				tau=sum/c[k];
				for (i=k;i<=n;i++) a[i][j] -= tau*a[i][k];
			}
		}
	}
	d[n]=a[n][n];
	if (d[n] == 0.0) *sing=1;
}


void NumericalRecipes::rsolv(double **a, int n, double d[], double b[])
{
	int i,j;
	double sum;
	b[n] /= d[n];
	for (i=n-1;i>=1;i--) {
		for (sum=0.0,j=i+1;j<=n;j++) sum += a[i][j]*b[j];
		b[i]=(b[i]-sum)/d[i];
	}
}

void NumericalRecipes::qrupdt(double **r, double **qt, int n, double u[], double v[])
{
	int i,j,k;
	for (k=n;k>=1;k--) { 
		if (u[k]) break;
	}
	if (k < 1) k=1;
	for (i=k-1;i>=1;i--) {
		rotate(r,qt,n,i,u[i],-u[i+1]);
		if (u[i] == 0.0)
			u[i]=fabs(u[i+1]);
		else if (fabs(u[i]) > fabs(u[i+1]))
			u[i]=fabs(u[i])*sqrt(1.0+SQR(u[i+1]/u[i]));
		else
			u[i]=fabs(u[i+1])*sqrt(1.0+SQR(u[i]/u[i+1]));
	}
	for (j=1;j<=n;j++) r[1][j] += u[1]*v[j];
	for (i=1;i<k;i++) rotate(r,qt,n,i,r[i][i],-r[i+1][i]);
}

void NumericalRecipes::rotate(double **r, double **qt, int n, int i, double a, double b)
{
	int j;
	double c,fact,s,w,y;
	if (a == 0.0) { 
		c=0.0;
		s=(b >= 0.0 ? 1.0 : -1.0);
	}
	else if (fabs(a) > fabs(b)) {
		fact=b/a;
		c=SIGN(1.0/sqrt(1.0+(fact*fact)),a);
		s=fact*c;
	}
	else {
		fact=a/b;
		s=SIGN(1.0/sqrt(1.0+(fact*fact)),b);
		c=fact*s;
	}
	for (j=i;j<=n;j++) { 
		y=r[i][j];
		w=r[i+1][j];
		r[i][j]=c*y-s*w;
		r[i+1][j]=s*y+c*w;
	}
	for (j=1;j<=n;j++) { 
		y=qt[i][j];
		w=qt[i+1][j];
		qt[i][j]=c*y-s*w;
		qt[i+1][j]=s*y+c*w;
	}
}

double* NumericalRecipes::vector(int nl,int nh)
{
	double *v;

	v=(double *)malloc((size_t) (nh-nl+1+1)*sizeof(double));
//	if (!v) nrerror("allocation failure in vector()");
	return v-nl+1;
}


void NumericalRecipes::free_vector(double *v,int nl,int nh)
{
	free((char*) (v+nl-1));
}


double** NumericalRecipes::matrix(int nrl,int nrh,int ncl,int nch)
{
	int i,nrow=nrh-nrl+1,ncol=nch-ncl+1;
	double **m;

	m=(double **) malloc((size_t)((nrow+1)*sizeof(double*)));
//	if (!m) nrerror("allocation failure 1 in matrix()");
	m += 1;
	m -= nrl;

   m[nrl] = (double *) malloc((size_t)((nrow*ncol+1)*sizeof(double)));
   m[nrl] += 1;
   m[nrl] -= ncl;

   for (i=nrl+1;i<=nrh;i++) m[i] = m[i-1]+ncol;

	return m;
}


void NumericalRecipes::free_matrix(double **m,int nrl,int nrh,int ncl,int nch)
{
//	int i;

//	for(i=nrh;i>=nrl;i--) free((char*) (m[i]+ncl));
//	free((char*) (m+nrl));

	free((char*)(m[nrl]+ncl-1));
    free((char*)(m+nrl-1));
}

/***********************************************************************/

void NumericalRecipes::testPowell()
{
#ifdef _DEBUG


#endif
}



void NumericalRecipes::testSobseq()
{


}



void NumericalRecipes::testRAN2()
{
#ifdef _DEBUG

#endif

}
