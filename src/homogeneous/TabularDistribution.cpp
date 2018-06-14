// TabularDistribution.cpp: implementation of the TabularDistribution class.
//
//////////////////////////////////////////////////////////////////////


#include "TabularDistribution.h"
#include "TabularBinomialSupport.h"
#include "TabularPoissonSupport.h"
#include "TabularLogNormalIntegrator.h"
#include "TabularLogNormalBinomialIntegrator.h"
#include "TabularLogNormalPoissonIntegrator.h"
#include "TabularNormalIntegrator.h"
#include "../parametric/LogNormalDistribution.h"
#include "../parametric/BetaDistribution.h"
#include "../parametric/NormalDistribution.h"
#include "../dcdf/DCDFLIBInterface.h"

#include <math.h>
#include <iostream>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define LOG_ZERO (-3000)

TabularDistribution::TabularDistribution(double * vals, double * cdfs, int length, int type, double p1, double p2, double p3, double p4)
{
	double lox = vals[0];
	double hix = vals[length - 1];

	switch (type) {

	case TABULAR_LIN:
		support = new LinearTabularSupport();
		break;

	case TABULAR_LOG:
		support = new LogarithmicTabularSupport();
		break;

	case TABULAR_LIN_BINOMIAL:
		support = new TabularBinomialSupport2(lox, hix, p1,p2,false);
		break;

	case TABULAR_LOG_BINOMIAL:
		support = new TabularBinomialSupport2(lox, hix, p1,p2,true);
		break;

	case TABULAR_LIN_POISSON:
		support = new TabularPoissonSupport2(lox, hix, p1,p2,false);
		break;

	case TABULAR_LOG_POISSON:
		support = new TabularPoissonSupport2(lox, hix, p1,p2,true);
		break;

	case TABULAR_LOG_LOGNORMAL:
		support = new TabularLogNormalSupport2(lox, hix, p1,p2,true);
		break;

	case TABULAR_LIN_LOGNORMAL:
		support = new TabularLogNormalSupport2(lox, hix, p1,p2,false);
		break;

	case TABULAR_LIN_NORMAL:
		support = new TabularNormalSupport2(lox, hix, p1,p2,false);
		break;

	case TABULAR_LOG_NORMAL:
		// integrator doesn't support this mode.
		support = 0;
		break;

	case TABULAR_LOG_LOGNORMAL_BINOMIAL:
		support = new TabularLogNormalBinomialSupport2(lox,hix,p1,p2,p3,p4,true);
		break;

	case TABULAR_LIN_LOGNORMAL_BINOMIAL:
		support = new TabularLogNormalBinomialSupport2(lox,hix,p1,p2,p3,p4,false);
		break;

	case TABULAR_LOG_LOGNORMAL_POISSON:
		support = new TabularLogNormalPoissonSupport2(lox,hix,p1,p2,p3,p4,true);
		break;

	case TABULAR_LIN_LOGNORMAL_POISSON:
		support = new TabularLogNormalPoissonSupport2(lox,hix,p1,p2,p3,p4,false);
		break;

	default: 
		support = 0;
		break;

	}

	// copy x-axis values
	this->vals = new double[length];
	for (int i = 0 ; i < length ; i++) {
		this->vals[i] = vals[i];
	}

	this->integrals = support->createIntegralArray(vals,length);

	// Check if support creates update. If not, copy cdf array.
	this->cdfs = support->createPosteriorCDF(vals,cdfs,length);
	if (this->cdfs == 0) {
		this->cdfs = new double[length];

		for (int i = 0 ; i < length ; i++) {
			this->cdfs[i] = cdfs[i];
		}
	}

	cdfs[0] = 0;
	cdfs[length - 1] = 1;

	this->length = length;

	stats_computed = false;
	computeStats();

}

TabularDistribution::~TabularDistribution()
{
	delete support;
	delete [] cdfs;
	delete [] vals;
	delete [] integrals;
}

double TabularDistribution::getLogPDF(double x) const
{
	int index = findIndexForX(x);
	if (cdfs[index] == cdfs[index+1]) return LOG_ZERO;
	if (vals[index] == vals[index+1]) return 0;
	double i0hi = integrals != 0 ? integrals[index + 1] : 0;
	double lnpdf = support->getLogPDF(vals[index],vals[index+1],cdfs[index],cdfs[index+1],x,i0hi);
	return lnpdf;
}

double TabularDistribution::getCDF(double x) const
{
	int index = findIndexForX(x);
	if (vals[index] == vals[index+1]) return cdfs[index+1];
	if (cdfs[index] == cdfs[index+1]) return cdfs[index];
	double i0hi = integrals != 0 ? integrals[index + 1] : 0;
	double cdf = support->getCDF(vals[index],vals[index+1],cdfs[index],cdfs[index+1],x,i0hi);
	return cdf;
}

double TabularDistribution::getCDF(int index) const
{
	return cdfs[index];
}

double TabularDistribution::getLowerBound() const
{
	return vals[0];
}

double TabularDistribution::getUpperBound() const
{
	return vals[length - 1];
}

double TabularDistribution::getMean() const
{
	return mean;
}

double TabularDistribution::getVariance() const
{
	return variance;
}

double TabularDistribution::getMedian() const
{
	return median;
}

double TabularDistribution::getQuantile(double z) const
{
	if (stats_computed && (z == 0.05 || z == 0.95)) {
		if (z == 0.05) return p05;
		if (z == 0.95) return p95;
	}
	int index = findIndexForCDF(z);

	if (vals[index] == vals[index+1]) return vals[index];
	if (cdfs[index] == cdfs[index+1]) return vals[index];

	double i0hi = integrals != 0 ? integrals[index + 1] : 0;
	double x = support->getQuantile(vals[index],vals[index+1],cdfs[index],cdfs[index+1],z,i0hi);
	return x;
}

/**
 * Returns index for lower interval containing x.
 * Returns -1 if x is smaller than vals[0].
 * Returns -2 if x is greater than vals[length-1].
 * If x corresponds to a bound, either the interval
 * above is returned.
 */
int TabularDistribution::findIndexForX(double x) const
{
	if (x < vals[0]) return -1;
	if (x > vals[length - 1]) return -2;

	int lo = 0;
	int hi = length - 1;

	while (hi > lo + 1) {
		int md = (hi + lo) / 2;

		if (vals[md] > x) {
			hi = md;
		} else {
			lo = md;
		}
	}

	// ensure that we return the last interval containing x
	// in case x is a boundary value.
	while (lo < length - 2 && vals[hi] == x) lo++;

	return lo;
}

/**
 * Returns index for lower interval containing cdf.
 * If cdf corresponds to cdf at a bound, lower interval
 * is returned.
 * Returns -1 if the cdf value is invalid.
 */
int TabularDistribution::findIndexForCDF(double cdf) const
{
	if (cdf < 0) return -1;
	if (cdf > 1) return -2;
	if (cdf < cdfs[1]) return 0;
	if (cdf > cdfs[length - 2]) return length - 2;

	int lo = 0;
	int hi = length - 1;

	while (hi > lo + 1) {
		int md = (hi + lo) / 2;
		if (cdfs[md] >= cdf) {
			hi = md;
		} else {
			lo = md;
		}
	}

	// ensure that we return the first interval containing cdf
	// in case cdf is a boundary value.
	while ((lo > 0 && cdfs[lo] == cdf) || lo > length - 2) lo--;

	return lo;
}

/**
 * Computes frequently required statistics. This function 
 * returns immediately if the stats have already been computed.
 */
void TabularDistribution::computeStats()
{
	mean = 0;
	variance = 0;
	for (int i = 0 ; i < length - 1 ; i++) {

		double lox = vals[i];
		double hix = vals[i+1];
		double locdf = cdfs[i];
		double hicdf = cdfs[i+1];

		double prob,dmean,dvariance;

		prob = (hicdf - locdf);

		if (lox == hix || prob == 0) {
			dmean = lox;
			dvariance = 0;
		} else {
			dmean =  support->getMean(lox,hix);
			dvariance = support->getVariance(lox,hix);
		}
	
		mean += prob * dmean;
		variance += prob * (dvariance + dmean * dmean);
	}
	variance -= mean * mean;

	p05 = getQuantile(0.05);
	p95 = getQuantile(0.95);
	median = getQuantile(0.5);

	stats_computed = true;
}


//////////////////////////////////////////////////////////////////////////

double LinearTabularSupport::getMean(double lox,double hix) const
{
	double mean = (lox + hix) / 2;
	return mean;
}

double LinearTabularSupport::getVariance(double lox,double hix) const
{
	if (hix == lox) return 0;
	double variance = (pow(hix,3) - pow(lox,3)) / 12 + hix * lox * (lox - hix) / 4;
	variance /= (hix - lox);
	return variance;
}

double LinearTabularSupport::getLogPDF(double lox,double hix, double locdf, double hicdf ,double x, double i0hi) const
{
	if (hix == lox) return 0;
	if (hicdf == locdf) return - LOG_ZERO;
	return log((hicdf - locdf) / (hix - lox));
}

double LinearTabularSupport::getCDF(double lox,double hix,double locdf, double hicdf ,double x, double i0hi) const
{
	if (hix == lox) return hicdf;
	return locdf + (hicdf - locdf) * (x - lox) / (hix - lox);
}

double LinearTabularSupport::getQuantile(double lox ,double hix, double locdf, double hicdf,double z, double i0hi) const
{
	if (hix == lox) return hix;
	if (hicdf == locdf) return lox;
	if (z >= hicdf) return hix;
	if (z <= locdf) return lox;
	return lox + (hix - lox) * (z - locdf) / (hicdf - locdf);
}

//////////////////////////////////////////////////////////////////////////

double LogarithmicTabularSupport::getMean(double lox,double hix) const
{
    double mean = (hix - lox) / log(hix/lox);
    return mean;
}

double LogarithmicTabularSupport::getVariance(double lox,double hix) const
{
	if (hix == lox) return 0;
	double deltaln = log(hix/lox);
	double variance = (0.5 * (pow(hix,2) - pow(lox,2)) - pow(hix - lox,2) / deltaln) / deltaln;
    return  variance;
}

double LogarithmicTabularSupport::getLogPDF(double lox,double hix, double locdf, double hicdf ,double x, double i0hi) const
{
	if (hix == lox) return 0;
	if (hicdf == locdf) return LOG_ZERO;
	double lnpdf = log( (hicdf - locdf) / x / log(hix / lox) );
	return lnpdf;
}

double LogarithmicTabularSupport::getCDF(double lox,double hix,double locdf, double hicdf ,double x, double i0hi) const
{
	if (hix == lox) return hicdf;
	double cdf = locdf + (hicdf - locdf) * log(x / lox) / log(hix / lox);
	return cdf;
}

/**
 * Returns the quantile.
 */
double LogarithmicTabularSupport::getQuantile(double lox ,double hix, double locdf, double hicdf,double z, double i0hi) const
{
	if (hix == lox) return hix;
	if (hicdf == locdf) return lox;
	if (z >= hicdf) return hix;
	if (z <= locdf) return lox;
	double quantile = lox * pow(hix/lox,(z-locdf)/(hicdf-locdf));
	return quantile;
}

//////////////////////////////////////////////////////////////////////////

UpdatedTabularSupport::UpdatedTabularSupport() {}

UpdatedTabularSupport::~UpdatedTabularSupport() {}

double UpdatedTabularSupport::getMeanS(TabularIntegrator & sup) const
{
	double retval = 0;
	double i0 = 0;
	double i1 = 0;
	double i2 = 0;
	double lnc = 0;
	sup.computeIntegrals(i0,i1,i2,lnc);
	if (i0 > 0) {
		retval = i1 / i0;
	}
	return retval;
}

double UpdatedTabularSupport::getVarianceS(TabularIntegrator & sup) const
{
	double retval = 0;
	double i0 = 0;
	double i1 = 0;
	double i2 = 0;
	double lnc = 0;

	sup.computeIntegrals(i0,i1,i2,lnc);
	if (i0 > 0) {
		retval = (i2 / i0) - pow(i1 / i0,2);
	} 
	return retval;
}

double UpdatedTabularSupport::getCDFS(TabularIntegrator & sup, double lox,double hix,double locdf, double hicdf ,double x, double i0hi) const
{
	double lnc;
	double i0x = 0;
//	double i0hi = 0;
	if (lox == hix) return hicdf;
//	sup.computeIntegral(hix,i0hi,lnc);
	sup.computeIntegral(x,i0x,lnc);
	if (i0hi == 0) return hicdf;
	double cdf = locdf + (hicdf - locdf) * i0x / i0hi;
	return cdf;
}

double UpdatedTabularSupport::getQuantileS(TabularIntegrator & sup, double lox ,double hix, double locdf, double hicdf,double z, double i0hi) const
{
	if (hix == lox) return hix;
	if (hicdf == locdf) return lox;
	if (z >= hicdf) return hix;
	if (z <= locdf) return lox;
	double quantile = 0;

	//double lnc,i0hix;
	//sup.computeIntegral(hix,i0hix,lnc);

	if (i0hi == 0) return lox;
	double i_x = i0hi * (z - locdf) / (hicdf - locdf);
	sup.inverseIntegral(i_x,quantile);
	return quantile;
}

double UpdatedTabularSupport::getLogPDF(double lox,double hix, double locdf, double hicdf ,double x, double i0hi) const
{
	return getLogPDFS(lox,hix,locdf,hicdf,x,*integrator,i0hi);
}

double UpdatedTabularSupport::getLogPDFS(double lox,double hix, double locdf, double hicdf ,double x, TabularIntegrator & sup, double i0) const
{
//	double i0 = 0;
	double lnc = 0;
//	sup.computeIntegral(hix,i0,lnc);
	if (i0 > 0) {
		double i = 0;
		sup.computeLogIntegrant(x,i,lnc);
		double lnpdf = i + log((hicdf - locdf) / i0);
		return lnpdf;
	}
	return LOG_ZERO;
}

double * UpdatedTabularSupport::createIntegralArray(const double * vals, int length) const
{
	return createIntegralArrayS(vals,length,*integrator);
}

double * UpdatedTabularSupport::createIntegralArrayS(const double * vals, int length, TabularIntegrator & sup) const
{
	double * i0_arr = new double[length];
	double lnc = 0;
	double last = 0;
	double curr = 0;
	i0_arr[0] = 0;
	for (int i = 1 ; i < length ; i++) {
		sup.computeIntegral(vals[i],curr,lnc);
		i0_arr[i] = curr - last;
		last = curr;
	}
	return i0_arr;
}

double * UpdatedTabularSupport::createPosteriorCDF(const double * vals, const double * cdfs, int length) const
{
	return createPosteriorCDFS(vals,cdfs,length,*integrator,logdensity);
}

double * UpdatedTabularSupport::createPosteriorCDFS(const double * vals, const double * cdfs, int length, TabularIntegrator & sup, bool logdensity) const
{
	if (length == 0) return 0;

	double * post = new double[length];
	post[0] = 0;

	double icurr = 0;
	double iprev = 0;
	double lnc;
	int i;

	for (i = 1 ; i < length ; i++) {

		double r = 0;

		if (vals[i] > vals[i - 1]) {

			sup.computeIntegral(vals[i],icurr,lnc);
			if (!logdensity) {
				// multiply by average density over interval
				r = (icurr - iprev) / (vals[i] - vals[i - 1]);
			} else {
				// multiply by average density over interval (?)
				r = (icurr - iprev) / log(vals[i] / vals[i - 1]);
			}

		} else {
			// In order to find the r factor for a discrete probability mass,
			// we must find the ratio between the density in the prior tabular
			// and the derivative of the unnormalized CDF function.
			double lnr;
			sup.computeLogIntegrant(vals[i],lnr,lnc);
			r = exp(lnr);
			if (logdensity) r *= vals[i];

//			sup.computeIntegral(1.000001 * vals[i],icurr,lnc);
//			double rstar = 0;
//
//			if (!logdensity) {
//				// multiply by average density over interval
//				rstar = (icurr - iprev) / (1.000001 * vals[i] - vals[i - 1]);
//			} else {
//				// multiply by average density over interval (?)
//				rstar = (icurr - iprev) / log(vals[i] * 1.000001 / vals[i - 1]);
//			}
		}

		post[i] = post[i - 1] + r * (cdfs[i] - cdfs[i - 1]);

		iprev = icurr;
	}

	// normalize
	post[0] = 0;
	bool done = false;
	for (i = 1 ; i < length && !done ; i++) {
		post[i] /= post[length - 1];
		if (post[i] > 1) post[i] = 1;
		if (post[i] < 0) post[i] = 0;
		done = post[i] == 1;
	}

	for ( ; i < length ; i++) {
		post[i] = 1;
	}

	post[length - 1] = 1;

	return post;
}




//////////////////////////////////////////////////////////////////////////


TabularBinomialSupport2::TabularBinomialSupport2(double lox, double hix, double k, double n, bool logdensity)
{
	this->k = k;
	this->n = n;
	this->logdensity = logdensity;
	this->m = logdensity ? 1 : 0;
	this->lnc = (m == 0 || k > 0) ? DCDFLIBInterface::computeLnGamma(k-m+1) + 
		  DCDFLIBInterface::computeLnGamma(n-k+1) - 
		  DCDFLIBInterface::computeLnGamma(n-m+2) : 0;
	this->integrator = new TabularBinomialSupport(k,n,m,lox,hix,lnc);
}


TabularBinomialSupport2::~TabularBinomialSupport2() {
	delete integrator;
}

double TabularBinomialSupport2::getMean(double lox,double hix) const
{
	TabularBinomialSupport tbs = TabularBinomialSupport(k,n,m,lox,hix,lnc);
	return getMeanS(tbs);
}

double TabularBinomialSupport2::getVariance(double lox,double hix) const
{
	TabularBinomialSupport tbs = TabularBinomialSupport(k,n,m,lox,hix,lnc);
	return getVarianceS(tbs);
}

double TabularBinomialSupport2::getCDF(double lox,double hix,double locdf, double hicdf ,double x, double i0hi) const
{
	TabularBinomialSupport tbs = TabularBinomialSupport(k,n,m,lox,hix,lnc);
	return getCDFS(tbs,lox,hix,locdf,hicdf,x,i0hi);
}

double TabularBinomialSupport2::getQuantile(double lox ,double hix, double locdf, double hicdf,double z, double i0hi) const
{
	TabularBinomialSupport tbs = TabularBinomialSupport(k,n,m,lox,hix,lnc);
	return getQuantileS(tbs,lox,hix,locdf,hicdf,z,i0hi);
}

/*
double TabularBinomialSupport2::getLogPDF(double lox,double hix, double locdf, double hicdf ,double x, double i0hi) const
{
	TabularBinomialSupport sup(k,n,m,lox,hix,lnc);
	return getLogPDFS(lox,hix,locdf,hicdf,x,sup,i0hi);
}

double * TabularBinomialSupport2::createIntegralArray(const double * vals, int length) const
{
	TabularBinomialSupport sup(k,n,m,vals[0],vals[length - 1]);
	return createIntegralArrayS(vals,length,sup);
}

double * TabularBinomialSupport2::createPosteriorCDF(const double * vals, const double * cdfs, int length) const
{
	TabularBinomialSupport sup(k,n,m,vals[0],vals[length - 1]);
	return createPosteriorCDFS(vals,cdfs,length,sup,logdensity);
}
*/

//////////////////////////////////////////////////////////////////////////

TabularPoissonSupport2::TabularPoissonSupport2(double lox, double hix, double k, double t, bool logdensity)
{
	this->k = k;
	this->t = t;
	this->logdensity = logdensity;
	this->m = logdensity ? 1 : 0;
	double a = 1 + k - m;
	gammaln = (a > 0) ? DCDFLIBInterface::computeLnGamma(a) : 0;
	this->integrator = new TabularPoissonSupport(k,t,m,lox,hix,gammaln);
}

TabularPoissonSupport2::~TabularPoissonSupport2() {
	delete integrator;
}

double TabularPoissonSupport2::getMean(double lox,double hix) const
{
//	if (lox == hix) return lox;
	TabularPoissonSupport tps = TabularPoissonSupport(k,t,m,lox,hix,gammaln);
	return getMeanS(tps);
}

double TabularPoissonSupport2::getVariance(double lox,double hix) const 
{
//	if (lox == hix) return 0;
	TabularPoissonSupport tps = TabularPoissonSupport(k,t,m,lox,hix,gammaln);
	return getVarianceS(tps);
}

double TabularPoissonSupport2::getCDF(double lox,double hix,double locdf, double hicdf ,double x, double i0hi) const 
{
//	if (lox == hix || locdf == hicdf) return hicdf;
	if (locdf == hicdf) return hicdf;
	TabularPoissonSupport tps = TabularPoissonSupport(k,t,m,lox,hix,gammaln);
	return getCDFS(tps,lox,hix,locdf,hicdf,x,i0hi);
}

double TabularPoissonSupport2::getQuantile(double lox ,double hix, double locdf, double hicdf,double z, double i0hi) const
{
//	if (lox == hix || locdf == hicdf) return lox;
	TabularPoissonSupport tps = TabularPoissonSupport(k,t,m,lox,hix,gammaln);
	return getQuantileS(tps,lox,hix,locdf,hicdf,z,i0hi);
}

/*
double TabularPoissonSupport2::getLogPDF(double lox,double hix, double locdf, double hicdf ,double x, double i0hi) const {
	TabularPoissonSupport sup(k,t,m,lox,hix,gammaln);
	return getLogPDFS(lox,hix,locdf,hicdf,x,sup,i0hi);
}

double * TabularPoissonSupport2::createIntegralArray(const double * vals, int length) const
{
	TabularPoissonSupport sup(k,t,m,vals[0],vals[length - 1],gammaln);
	return createIntegralArrayS(vals,length,sup);
}

double * TabularPoissonSupport2::createPosteriorCDF(const double * vals, const double * cdfs, int length) const
{
	TabularPoissonSupport sup(k,t,m,vals[0],vals[length - 1],gammaln);
	return createPosteriorCDFS(vals,cdfs,length,sup,logdensity);
}
*/

//////////////////////////////////////////////////////////////////////////////////

TabularLogNormalSupport2::TabularLogNormalSupport2(double lox, double hix, double nu, double tau, bool logdensity)
{
	this->nu = nu;
	this->tau = tau;
	this->m = logdensity ? 1 : 0;
	this->logdensity = logdensity;
	this->integrator = new TabularLogNormalIntegrator(nu,tau,m,lox,hix);
}

TabularLogNormalSupport2::~TabularLogNormalSupport2() {
	delete integrator;
}

double TabularLogNormalSupport2::getMean(double lox,double hix) const
{
	TabularLogNormalIntegrator tlni = TabularLogNormalIntegrator(nu,tau,m,lox,hix);
	return getMeanS(tlni);
}

double TabularLogNormalSupport2::getVariance(double lox,double hix) const
{
	TabularLogNormalIntegrator tlni = TabularLogNormalIntegrator(nu,tau,m,lox,hix);
	return getVarianceS(tlni);
}

double TabularLogNormalSupport2::getCDF(double lox,double hix,double locdf, double hicdf ,double x, double i0hi) const
{
	TabularLogNormalIntegrator tlni = TabularLogNormalIntegrator(nu,tau,m,lox,hix);
	return getCDFS(tlni,lox,hix,locdf,hicdf,x,i0hi);
}

double TabularLogNormalSupport2::getQuantile(double lox ,double hix, double locdf, double hicdf,double z, double i0hi) const
{
	TabularLogNormalIntegrator tlni = TabularLogNormalIntegrator(nu,tau,m,lox,hix);
	return getQuantileS(tlni,lox,hix,locdf,hicdf,z,i0hi);
}

/*
double TabularLogNormalSupport2::getLogPDF(double lox,double hix, double locdf, double hicdf ,double x, double i0hi) const
{
	TabularLogNormalIntegrator sup(nu,tau,m,lox,hix);
	return getLogPDFS(lox,hix,locdf,hicdf,x,sup,i0hi);
}

double * TabularLogNormalSupport2::createIntegralArray(const double * vals, int length) const
{
	TabularLogNormalIntegrator sup(nu,tau,m,vals[0],vals[length - 1]);
	return createIntegralArrayS(vals,length,sup);
}

double * TabularLogNormalSupport2::createPosteriorCDF(const double * vals, const double * cdfs, int length) const
{
	TabularLogNormalIntegrator sup(nu,tau,m,vals[0],vals[length - 1]);
	return createPosteriorCDFS(vals,cdfs,length,sup,logdensity);
}
*/

//////////////////////////////////////////////////////////////////////////////////

TabularNormalSupport2::TabularNormalSupport2(double lox, double hix, double nu, double tau, bool logdensity)
{
	this->nu = nu;
	this->tau = tau;
	this->m = logdensity ? 1 : 0;
	this->logdensity = logdensity;
	this->integrator = new TabularNormalIntegrator(nu,tau,m,lox,hix);
}

TabularNormalSupport2::~TabularNormalSupport2() {
	delete integrator;
}

double TabularNormalSupport2::getMean(double lox,double hix) const
{
	TabularNormalIntegrator tni = TabularNormalIntegrator(nu,tau,m,lox,hix);
	return getMeanS(tni);
}

double TabularNormalSupport2::getVariance(double lox,double hix) const
{
	TabularNormalIntegrator tni = TabularNormalIntegrator(nu,tau,m,lox,hix);
	return getVarianceS(tni);
}

double TabularNormalSupport2::getCDF(double lox,double hix,double locdf, double hicdf ,double x, double i0hi) const
{
	TabularNormalIntegrator tni = TabularNormalIntegrator(nu,tau,m,lox,hix);
	return getCDFS(tni,lox,hix,locdf,hicdf,x,i0hi);
}

double TabularNormalSupport2::getQuantile(double lox ,double hix, double locdf, double hicdf,double z, double i0hi) const
{
	TabularNormalIntegrator tni = TabularNormalIntegrator(nu,tau,m,lox,hix);
	return getQuantileS(tni,lox,hix,locdf,hicdf,z,i0hi);
}

/*
double TabularNormalSupport2::getLogPDF(double lox,double hix, double locdf, double hicdf ,double x, double i0hi) const
{
	TabularNormalIntegrator sup(nu,tau,m,lox,hix);
	return getLogPDFS(lox,hix,locdf,hicdf,x,sup,i0hi);
}

double * TabularNormalSupport2::createIntegralArray(const double * vals, int length) const
{
	TabularNormalIntegrator sup(nu,tau,m,vals[0],vals[length - 1]);
	return createIntegralArrayS(vals,length,sup);
}

double * TabularNormalSupport2::createPosteriorCDF(const double * vals, const double * cdfs, int length) const
{
	TabularNormalIntegrator sup(nu,tau,m,vals[0],vals[length - 1]);
	return createPosteriorCDFS(vals,cdfs,length,sup,logdensity);
}
*/

////////////////////////////////////////////////////////////////////////

TabularLogNormalBinomialSupport2::TabularLogNormalBinomialSupport2(double lox, double hix, double nu, double tau, double k, double n, bool logdensity)
{
	this->nu = nu;
	this->tau = tau;
	this->k = k;
	this->n = n;
	this->m = logdensity ? 1 : 0;
	this->logdensity = logdensity;
	this->lnnorm = 0;
	this->integrator = new TabularLogNormalBinomialIntegrator(lox,hix,nu,tau,k,n,m);
	lnnorm = this->integrator->getLnNorm();
//	double i0_tmp;
//	TabularLogNormalBinomialIntegrator sup(lox,hix,nu,tau,k,n,m);
//	sup.computeIntegral(0,i0_tmp,lnnorm);
}

TabularLogNormalBinomialSupport2::~TabularLogNormalBinomialSupport2() {
	delete integrator;
}

double TabularLogNormalBinomialSupport2::getMean(double lox,double hix) const
{
	TabularLogNormalBinomialIntegrator tlnbi = TabularLogNormalBinomialIntegrator(lox,hix,nu,tau,k,n,m,lnnorm);
	return getMeanS(tlnbi);
}

double TabularLogNormalBinomialSupport2::getVariance(double lox,double hix) const
{
	TabularLogNormalBinomialIntegrator tlnbi = TabularLogNormalBinomialIntegrator(lox,hix,nu,tau,k,n,m,lnnorm);
	return getVarianceS(tlnbi);
}


double TabularLogNormalBinomialSupport2::getCDF(double lox,double hix,double locdf, double hicdf ,double x, double i0hi) const
{
	TabularLogNormalBinomialIntegrator tlnbi = TabularLogNormalBinomialIntegrator(lox,hix,nu,tau,k,n,m,lnnorm);
	return getCDFS(tlnbi,lox,hix,locdf,hicdf,x,i0hi);
}

double TabularLogNormalBinomialSupport2::getQuantile(double lox ,double hix, double locdf, double hicdf,double z, double i0hi) const
{
	TabularLogNormalBinomialIntegrator tlnbi = TabularLogNormalBinomialIntegrator(lox,hix,nu,tau,k,n,m,lnnorm);
	return getQuantileS(tlnbi,lox,hix,locdf,hicdf,z,i0hi);
}

/*
double TabularLogNormalBinomialSupport2::getLogPDF(double lox,double hix, double locdf, double hicdf ,double x, double i0hi) const
{
	TabularLogNormalBinomialIntegrator sup(lox,hix,nu,tau,k,n,m,lnnorm);
	return getLogPDFS(lox,hix,locdf,hicdf,x,sup,i0hi);
}

double * TabularLogNormalBinomialSupport2::createIntegralArray(const double * vals, int length) const
{
//	TabularLogNormalBinomialIntegrator sup(vals[0],vals[length - 1],nu,tau,k,n,m);
	return createIntegralArrayS(vals,length,integrator);
}

double * TabularLogNormalBinomialSupport2::createPosteriorCDF(const double * vals, const double * cdfs, int length) const
{
//	TabularLogNormalBinomialIntegrator sup(vals[0],vals[length - 1],nu,tau,k,n,m);
	return createPosteriorCDFS(vals,cdfs,length,sup,integrator);
}
*/

////////////////////////////////////////////////////////////////////////

/**
 *
 * @param lox lower bound of first interval in the tabular distribution
 * @param hix upper bound of last interval in the tabular distribution
 * @param nu  mean of underlying normal
 * @param tau sd of underlying normal
 * @param k number of events
 * @param t total exposure time
 * @param logdensity true for loguniform density in each interval of prior tab
 */
TabularLogNormalPoissonSupport2::TabularLogNormalPoissonSupport2(double lox, double hix, double nu, double tau, double k, double t, bool logdensity)
{
	this->nu = nu;
	this->tau = tau;
	this->k = k;
	this->t = t;
	this->m = logdensity ? 1 : 0;
	this->logdensity = logdensity;
	this->integrator = new TabularLogNormalPoissonIntegrator(lox,hix,nu,tau,k,t,m);
	lnnorm = this->integrator->getLnNorm();
//	this->lnnorm = 0;
//	double i0_tmp;
//	TabularLogNormalPoissonIntegrator sup(0,1e100,nu,tau,k,t,m);
//	sup.computeIntegral(0,i0_tmp,lnnorm);
}

TabularLogNormalPoissonSupport2::~TabularLogNormalPoissonSupport2() {
	delete integrator;
}

/**
 * Returns the mean over the given interval.
 */
double TabularLogNormalPoissonSupport2::getMean(double lox,double hix) const
{
	TabularLogNormalPoissonIntegrator tlnpi = TabularLogNormalPoissonIntegrator(lox,hix,nu,tau,k,t,m,lnnorm);
	return getMeanS(tlnpi);
}

/**
 * Returns the variance over the specified interval.
 */
double TabularLogNormalPoissonSupport2::getVariance(double lox,double hix) const
{
	TabularLogNormalPoissonIntegrator tlnpi = TabularLogNormalPoissonIntegrator(lox,hix,nu,tau,k,t,m,lnnorm);
	return getVarianceS(tlnpi);
}

double TabularLogNormalPoissonSupport2::getCDF(double lox,double hix,double locdf, double hicdf ,double x, double i0hi) const
{
	TabularLogNormalPoissonIntegrator tlnpi = TabularLogNormalPoissonIntegrator(lox,hix,nu,tau,k,t,m,lnnorm);
	return getCDFS(tlnpi,lox,hix,locdf,hicdf,x,i0hi);
}

double TabularLogNormalPoissonSupport2::getQuantile(double lox ,double hix, double locdf, double hicdf,double z, double i0hi) const
{
	TabularLogNormalPoissonIntegrator tlnpi = TabularLogNormalPoissonIntegrator(lox,hix,nu,tau,k,t,m,lnnorm);
	return getQuantileS(tlnpi,lox,hix,locdf,hicdf,z,i0hi);
}

/*
 * Returns the pdf at the specified location. This can be called for a specific interval in
 * the distribution.
 * @param lox lower bound of interval
 * @param hix upper bound of interval
 * @param locdf CDF at lower bound
 * @param hicdf CDF at higher bound
 * @param x location for which to compute PDF
 * @param i0hi integral over given interval
 */

/*
double TabularLogNormalPoissonSupport2::getLogPDF(double lox,double hix, double locdf, double hicdf ,double x, double i0hi) const
{
	TabularLogNormalPoissonIntegrator sup(lox,hix,nu,tau,k,t,m,lnnorm);
	return getLogPDFS(lox,hix,locdf,hicdf,x,sup,i0hi);
}
  
double * TabularLogNormalPoissonSupport2::createIntegralArray(const double * vals, int length) const
{
//	TabularLogNormalPoissonIntegrator sup(vals[0],vals[length - 1],nu,tau,k,t,m);
//	return createIntegralArrayS(vals,length,sup);
	return createIntegralArrayS(vals,length,*integrator);
}

double * TabularLogNormalPoissonSupport2::createPosteriorCDF(const double * vals, const double * cdfs, int length) const
{
//	TabularLogNormalPoissonIntegrator sup(vals[0],vals[length - 1],nu,tau,k,t,m);
//	return createPosteriorCDFS(vals,cdfs,length,sup,logdensity);
	return createPosteriorCDFS(vals,cdfs,length,*integrator,logdensity);
}
*/