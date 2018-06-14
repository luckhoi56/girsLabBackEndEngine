// VegasIntegrator.cpp: implementation of the VegasIntegrator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VegasIntegrator.h"
#include <math.h>

#define ALPH 1.5
#define NDMX 50
#define MXDIM 10
#define TINY 1.0e-30
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
#define NTAB 32
#define NDIV (1+IMM1/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)
#define SQR(a) ((a)*(a))
#define IMIN(a,b) ((a)<(b) ? (a) : (b))
#define IMAX(a,b) ((a)>(b) ? (a) : (b))


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VegasIntegrator::VegasIntegrator()
{

}

VegasIntegrator::~VegasIntegrator()
{

}


void VegasIntegrator::vegas(double regn[], int ndim, Function &F, int init,
							unsigned long ncall, int itmx, int nprn, double *tgral,
							double *sd, double *chi2a, long idum)
//Performs Monte Carlo integration of a user-supplied ndim-dimensional function fxn over a
//rectangular volume specied by regn[1..2*ndim], a vector consisting of ndim \lower left"
//coordinates of the region followed by ndim \upper right" coordinates. The integration consists
//of itmx iterations, each with approximately ncall calls to the function. After each iteration
//the grid is rened; more than 5 or 10 iterations are rarely useful. The input ag init signals
//whether this call is a new start, or a subsequent call for additional iterations (see comments
//below). The input ag nprn (normally 0) controls the amount of diagnostic output. Returned
//answers are tgral (the best estimate of the integral), sd (its standard deviation), and chi2a
//(˜ 2 per degree of freedom, an indicator of whether consistent results are being obtained). See
//text for further details.
{
	//double ran2(long *idum);
	//void rebin(double rc, int nd, double r[], double xin[], double xi[]);
	static int i,it,j,k,mds,nd,ndo,ng,npg,ia[MXDIM+1],kg[MXDIM+1];
	static double calls,dv2g,dxg,f,f2,f2b,fb,rc,ti,tsi,wgt,xjac,xn,xnd,xo;
	static double d[NDMX+1][MXDIM+1],di[NDMX+1][MXDIM+1],dt[MXDIM+1],
					dx[MXDIM+1], r[NDMX+1],x[MXDIM+1],xi[MXDIM+1][NDMX+1],xin[NDMX+1];
	static double schi,si,swgt;
//Best make everything static, allowing restarts.
	if (init <= 0) { //Normal entry. Enter here on a cold start.
		mds=ndo=1; //Change to mds=0 to disable stratied sampling, i.e., use importance sampling only.
		for (j=1;j<=ndim;j++) xi[j][1]=1.0;
	}
	if (init <= 1) si=swgt=schi=0.0;
//Enter here to inherit the grid from a previous call, but not its answers.
	if (init <= 2) {// Enter here to inherit the previous grid and its answers.
		nd=NDMX;
		ng=1;
		if (mds) { //Set up for stratication.
			ng=(int)pow(ncall/2.0+0.25,1.0/ndim);
			mds=1;
			if ((2*ng-NDMX) >= 0) {
				mds = -1;
				npg=ng/NDMX+1;
				nd=ng/npg;
				ng=npg*nd;
			}
		}
		for (k=1,i=1;i<=ndim;i++) k *= ng;
		npg=IMAX(ncall/k,2);
		calls=(double)npg * (double)k;
		dxg=1.0/ng;
		for (dv2g=1,i=1;i<=ndim;i++) dv2g *= dxg;
		dv2g=SQR(calls*dv2g)/npg/npg/(npg-1.0);
		xnd=nd;
		dxg *= xnd;
		xjac=1.0/calls;
		for (j=1;j<=ndim;j++) {
			dx[j]=regn[j+ndim]-regn[j];
			xjac *= dx[j];
		}
		if (nd != ndo) {// Do binning if necessary.
			for (i=1;i<=IMAX(nd,ndo);i++) r[i]=1.0;
			for (j=1;j<=ndim;j++) rebin(ndo/xnd,nd,r,xin,xi[j]);
			ndo=nd;
		}
		if (nprn >= 0) {
			
		}
	}
	for (it=1;it<=itmx;it++) {
		//Main iteration loop. Can enter here (init . 3) to do an additional itmx iterations with
		//all other parameters unchanged.
		ti=tsi=0.0;
		for (j=1;j<=ndim;j++) {
			kg[j]=1;
			for (i=1;i<=nd;i++) d[i][j]=di[i][j]=0.0;
		}
		for (;;) {
			fb=f2b=0.0;
			for (k=1;k<=npg;k++) {
				wgt=xjac;
				for (j=1;j<=ndim;j++) {
					xn=(kg[j]-ran2(&idum))*dxg+1.0;
					ia[j]=IMAX(IMIN((int)(xn),NDMX),1);
					if (ia[j] > 1) {
						xo=xi[j][ia[j]]-xi[j][ia[j]-1];
						rc=xi[j][ia[j]-1]+(xn-ia[j])*xo;
					} else {
						xo=xi[j][ia[j]];
						rc=(xn-ia[j])*xo;
					}
					x[j]=regn[j]+rc*dx[j];
					wgt *= xo*xnd;
				}

				//f=wgt*(*fxn)(x,wgt);
				//Parameters params( MXDIM );
				//for( int paramsK = 0; paramsK < MXDIM; paramsK++ )
					//params.setValue( paramsK, x[paramsK + 1] );
				
				//f = wgt * exp( F.evaluate(Parameters( x[1] )) );
				f = wgt * exp( F.evaluate(Parameters( x[1] )) );

				f2=f*f;
				fb += f;
				f2b += f2;
				for (j=1;j<=ndim;j++) {
					di[ia[j]][j] += f;
					if (mds >= 0) d[ia[j]][j] += f2;
				}
			}
			f2b=sqrt(f2b*npg);
			f2b=(f2b-fb)*(f2b+fb);
			if (f2b <= 0.0) f2b=TINY;
			ti += fb;
			tsi += f2b;
			if (mds < 0) { //Use stratied sampling.
				for (j=1;j<=ndim;j++) d[ia[j]][j] += f2b;
			}
			for (k=ndim;k>=1;k--) {
				kg[k] %= ng;
				if (++kg[k] != 1) break;
			}
			if (k < 1) break;
		}
		tsi *= dv2g; //Compute nal results for this iteration.
		wgt=1.0/tsi;
		si += wgt*ti;
		schi += wgt*ti*ti;
		swgt += wgt;
		*tgral=si/swgt;
		*chi2a=(schi-si*(*tgral))/(it-0.9999);
		if (*chi2a < 0.0) *chi2a = 0.0;
		*sd=sqrt(1.0/swgt);
		tsi=sqrt(tsi);
		if (nprn >= 0) {
			
		}
		for (j=1;j<=ndim;j++) { //Rene the grid. Consult references to understand
			//the subtlety of this procedure. The renement is damped, to avoid rapid,
			//destabilizing changes, and also compressed in range by the exponent ALPH.
			xo=d[1][j];
			xn=d[2][j];
			d[1][j]=(xo+xn)/2.0;
			dt[j]=d[1][j];
			for (i=2;i<nd;i++) {
				rc=xo+xn;
				xo=xn;
				xn=d[i+1][j];
				d[i][j] = (rc+xn)/3.0;
				dt[j] += d[i][j];
			}
			d[nd][j]=(xo+xn)/2.0;
			dt[j] += d[nd][j];
		}
		for (j=1;j<=ndim;j++) {
			rc=0.0;
			for (i=1;i<=nd;i++) {
				if (d[i][j] < TINY) d[i][j]=TINY;
				r[i]=pow((1.0-d[i][j]/dt[j])/(log(dt[j])-log(d[i][j])),ALPH);
				rc += r[i];
			}
			rebin(rc/xnd,nd,r,xin,xi[j]);
		}
	}
}


void VegasIntegrator::rebin(double rc, int nd, double r[], double xin[], double xi[])
//Utility routine used by vegas, to rebin a vector of densities xi into new bins dened by a
//vector r.
{
	int i,k=0;
	double dr=0.0,xn=0.0,xo=0.0;
	for (i=1;i<nd;i++) {
		while (rc > dr)
			dr += r[++k];
		if (k > 1) xo=xi[k-1];
		xn=xi[k];
		dr -= rc;
		xin[i]=xn-(xn-xo)*dr/r[k];
	}
	for (i=1;i<nd;i++) xi[i]=xin[i];
	xi[nd]=1.0;
}


double VegasIntegrator::ran2(long *idum)
{
	int j;
	long k;
	static long idum2=123456789;
	static long iy=0;
	static long iv[NTAB];
	double temp;
	if (*idum<=0) {
		if(-(*idum)<1) *idum=1;
		else *idum=-(*idum);
		idum2=(*idum);
		for(j=NTAB+7; j>=0; j--) {
			k=(*idum)/IQ1;
			*idum=IA1*(*idum-k*IQ1)-k*IR1;
			if (*idum<0) *idum+=IM1;
			if (j<NTAB) iv[j]= *idum;
		}
		iy=iv[0];
	}
	k=(*idum)/IQ1;
	*idum=IA1*(*idum-k*IQ1)-k*IR1;
	if(*idum<0) *idum+=IM1;
	k=(*idum)/IQ2;
	idum2=IA2*(idum2-k*IQ2)-k*IR2;
	if(idum2<0) idum2+=IM2;
	j=iy/NDIV;
	iy=iv[j]-idum2;
	iv[j]= *idum;
	if(iy<1) iy+=IMM1;
	if((temp=AM*iy)>RNMX) return RNMX;
	else return temp;
}
