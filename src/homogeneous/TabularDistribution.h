// TabularDistribution.h: interface for the TabularDistribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABULARDISTRIBUTION_H__96653477_03E8_44BF_A5F0_D44BD191C9F6__INCLUDED_)
#define AFX_TABULARDISTRIBUTION_H__96653477_03E8_44BF_A5F0_D44BD191C9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TabularIntegrator.h"
#include "../parametric/ParametricDistribution.h"
#include "../parametric/GammaDistribution.h"

#define TABULAR_LIN				1
#define TABULAR_LOG				2
#define TABULAR_LIN_BINOMIAL	3
#define TABULAR_LOG_BINOMIAL	4
#define TABULAR_LIN_POISSON		5
#define TABULAR_LOG_POISSON		6
#define TABULAR_LIN_NORMAL		7
#define TABULAR_LOG_NORMAL		8
#define TABULAR_LIN_LOGNORMAL	9
#define TABULAR_LOG_LOGNORMAL	10
#define TABULAR_LIN_LOGNORMAL_BINOMIAL	11
#define TABULAR_LOG_LOGNORMAL_BINOMIAL	12
#define TABULAR_LIN_LOGNORMAL_POISSON	13
#define TABULAR_LOG_LOGNORMAL_POISSON	14

class TabularSupport
{
public:
	virtual ~TabularSupport() {}
	virtual double getMean(double lox,double hix) const = 0;
	virtual double getVariance(double lox,double hix) const = 0;
	virtual double getLogPDF(double lox,double hix, double locdf, double hicdf ,double x, double i0) const = 0;
	virtual double getCDF(double lox,double hix,double locdf, double hicdf ,double x, double i0) const = 0;
	virtual double getQuantile(double lox ,double hix, double locdf, double hicdf,double z, double i0) const = 0;
	virtual double * createPosteriorCDF(const double * vals, const double * cdfs, int length) const = 0;
	virtual double * createIntegralArray(const double * vals, int length) const = 0;
};

class TabularDistribution : public ParametricDistribution
{
public:
	virtual double getQuantile(double z) const;
	virtual double getMedian() const;
	virtual double getVariance() const;
	virtual double getMean() const;
	virtual double getUpperBound() const;
	virtual double getLowerBound() const;
	virtual double getCDF(double x) const;
	virtual double getCDF(int index) const;
	virtual double getLogPDF(double x) const;
	virtual void   releaseMemory() {};
	TabularDistribution(double * vals, double * cdfs, int length, int type, double p1 = 0, double p2 = 0, double p3 = 0, double p4 = 0);
	virtual ~TabularDistribution();

protected:
	TabularSupport * support;
	double * vals;			// x-values
	double * cdfs;		    // posterior cdf
	double * integrals;
	double mean;			// mean
	double variance;		// variance
	double median;			// median
	double p05;				// 5th percentile
	double p95;				// 95th percentile
	int length;				// length of the arrays
	int findIndexForX(double x) const;
	int findIndexForCDF(double cdf) const;
	void computeStats();
	bool stats_computed;	// flag: stats have been computed.
};

class LinearTabularSupport : public TabularSupport
{
public:
	virtual ~LinearTabularSupport() {}
	virtual double getMean(double lox,double hix) const;
	virtual double getVariance(double lox,double hix) const;
	virtual double getLogPDF(double lox,double hix, double locdf, double hicdf ,double x, double i0 = 0) const;
	virtual double getCDF(double lox,double hix,double locdf, double hicdf ,double x, double i0 = 0) const;
	virtual double getQuantile(double lox ,double hix, double locdf, double hicdf,double z, double i0 = 0) const;
	virtual double * createPosteriorCDF(const double * vals, const double * cdfs, int length) const {return 0;}
	virtual double * createIntegralArray(const double * vals, int length) const {return 0;}
};

class LogarithmicTabularSupport : public TabularSupport
{
public:
	virtual ~LogarithmicTabularSupport() {}
	virtual double getMean(double lox,double hix) const;
	virtual double getVariance(double lox,double hix) const;
	virtual double getLogPDF(double lox,double hix, double locdf, double hicdf ,double x, double i0 = 0) const;
	virtual double getCDF(double lox,double hix,double locdf, double hicdf ,double x, double i0 = 0) const;
	virtual double getQuantile(double lox ,double hix, double locdf, double hicdf,double z, double i0 = 0) const;
	virtual double * createPosteriorCDF(const double * vals, const double * cdfs, int length) const {return 0;}
	virtual double * createIntegralArray(const double * vals, int length) const {return 0;}
};

class UpdatedTabularSupport : public TabularSupport
{
public:
	UpdatedTabularSupport();
	virtual ~UpdatedTabularSupport();
	virtual double getMean(double lox,double hix) const = 0;
	virtual double getVariance(double lox,double hix) const = 0;
	virtual double getLogPDF(double lox,double hix, double locdf, double hicdf ,double x, double i0 = 0) const;
	virtual double getCDF(double lox,double hix,double locdf, double hicdf ,double x, double i0 = 0) const = 0;
	virtual double getQuantile(double lox ,double hix, double locdf, double hicdf,double z, double i0 = 0) const = 0;
	virtual double * createPosteriorCDF(const double * vals, const double * cdfs, int length) const;
	virtual double * createIntegralArray(const double * vals, int length) const;
protected:
	TabularIntegrator * integrator;
	bool logdensity;

	double getMeanS(TabularIntegrator & sup) const;
	double getVarianceS(TabularIntegrator & sup) const;
	double getCDFS(TabularIntegrator & sup, double lox,double hix,double locdf, double hicdf ,double x, double i0) const;
	double getQuantileS(TabularIntegrator & sup, double lox ,double hix, double locdf, double hicdf,double z, double i0) const;
	double getLogPDFS(double lox,double hix, double locdf, double hicdf ,double x, TabularIntegrator & sup, double i0) const;
	double * createPosteriorCDFS(const double * vals, const double * cdfs, int length, TabularIntegrator & sup, bool logdensity) const;
	double * createIntegralArrayS(const double * vals, int length, TabularIntegrator & sup) const;
};

class TabularBinomialSupport2 : public UpdatedTabularSupport
{
public:
	TabularBinomialSupport2(double lox, double hix, double p1, double p2, bool logdensity);
	virtual ~TabularBinomialSupport2();
	virtual double getMean(double lox,double hix) const;
	virtual double getVariance(double lox,double hix) const;
//	virtual double getLogPDF(double lox,double hix, double locdf, double hicdf ,double x, double i0 = 0) const;
	virtual double getCDF(double lox,double hix,double locdf, double hicdf ,double x, double i0 = 0) const;
	virtual double getQuantile(double lox ,double hix, double locdf, double hicdf,double z, double i0 = 0) const;
//	virtual double * createPosteriorCDF(const double * vals, const double * cdfs, int length) const;
//	virtual double * createIntegralArray(const double * vals, int length) const;
private:
	double k;
	double n;
	int m;			// 1 if logdensity
//	bool logdensity;
	double lnc;
};

class TabularPoissonSupport2 : public UpdatedTabularSupport
{
public:
	TabularPoissonSupport2(double lox, double hix, double p1, double p2, bool logdensity);
	virtual ~TabularPoissonSupport2();
	virtual double getMean(double lox,double hix) const;
	virtual double getVariance(double lox,double hix) const;
//	virtual double getLogPDF(double lox,double hix, double locdf, double hicdf ,double x, double i0 = 0) const;
	virtual double getCDF(double lox,double hix,double locdf, double hicdf ,double x, double i0 = 0) const;
	virtual double getQuantile(double lox ,double hix, double locdf, double hicdf,double z, double i0 = 0) const;
//	virtual double * createPosteriorCDF(const double * vals, const double * cdfs, int length) const;
//	virtual double * createIntegralArray(const double * vals, int length) const;
private:
	double k;
	double t;
//	bool logdensity;
	int m;			// 1 if logdensity
	double gammaln;
};

class TabularLogNormalSupport2 : public UpdatedTabularSupport
{
public:
	TabularLogNormalSupport2(double lox, double hix, double nu, double tau, bool logdensity);
	virtual ~TabularLogNormalSupport2();
	virtual double getMean(double lox,double hix) const;
	virtual double getVariance(double lox,double hix) const;
//	virtual double getLogPDF(double lox,double hix, double locdf, double hicdf ,double x, double i0 = 0) const;
	virtual double getCDF(double lox,double hix,double locdf, double hicdf ,double x, double i0 = 0) const;
	virtual double getQuantile(double lox ,double hix, double locdf, double hicdf,double z, double i0 = 0) const;
//	virtual double * createPosteriorCDF(const double * vals, const double * cdfs, int length) const;
//	virtual double * createIntegralArray(const double * vals, int length) const;
private:
	double nu;
	double tau;
//	bool logdensity;
	int m;			// 1 if logdensity
};

class TabularNormalSupport2 : public UpdatedTabularSupport
{
public:
	TabularNormalSupport2(double lox, double hix, double nu, double tau, bool logdensity);
	virtual ~TabularNormalSupport2();
	virtual double getMean(double lox,double hix) const;
	virtual double getVariance(double lox,double hix) const;
//	virtual double getLogPDF(double lox,double hix, double locdf, double hicdf ,double x, double i0 = 0) const;
	virtual double getCDF(double lox,double hix,double locdf, double hicdf ,double x, double i0 = 0) const;
	virtual double getQuantile(double lox ,double hix, double locdf, double hicdf,double z, double i0 = 0) const;
//	virtual double * createPosteriorCDF(const double * vals, const double * cdfs, int length) const;
//	virtual double * createIntegralArray(const double * vals, int length) const;
private:
	double nu;
	double tau;
//	bool logdensity;
	int m;			// 1 if logdensity (currently NOT supported).
};

class TabularLogNormalBinomialSupport2 : public UpdatedTabularSupport
{
public:
	TabularLogNormalBinomialSupport2(double lox, double hix, double nu, double tau, double k, double t, bool logdensity);
	virtual ~TabularLogNormalBinomialSupport2();
	virtual double getMean(double lox,double hix) const;
	virtual double getVariance(double lox,double hix) const;
//	virtual double getLogPDF(double lox,double hix, double locdf, double hicdf ,double x, double i0 = 0) const;
	virtual double getCDF(double lox,double hix,double locdf, double hicdf ,double x, double i0 = 0) const;
	virtual double getQuantile(double lox ,double hix, double locdf, double hicdf,double z, double i0 = 0) const;
//	virtual double * createPosteriorCDF(const double * vals, const double * cdfs, int length) const;
//	virtual double * createIntegralArray(const double * vals, int length) const;
private:
	double nu;
	double tau;
	double k;
	double n;
//	bool logdensity;
	int m; // 1 if logdensity
	double lnnorm;
};

class TabularLogNormalPoissonSupport2 : public UpdatedTabularSupport
{
public:
	TabularLogNormalPoissonSupport2(double lox, double hix, double nu, double tau, double k, double t, bool logdensity);
	virtual ~TabularLogNormalPoissonSupport2();
	virtual double getMean(double lox,double hix) const;
	virtual double getVariance(double lox,double hix) const;
//	virtual double getLogPDF(double lox,double hix, double locdf, double hicdf ,double x, double i0 = 0) const;
	virtual double getCDF(double lox,double hix,double locdf, double hicdf ,double x, double i0 = 0) const;
	virtual double getQuantile(double lox ,double hix, double locdf, double hicdf,double z, double i0 = 0) const;
//	virtual double * createPosteriorCDF(const double * vals, const double * cdfs, int length) const;
//	virtual double * createIntegralArray(const double * vals, int length) const;
private:
	double nu;
	double tau;
	double k;
	double t;
//	bool logdensity;
	int m; // 1 if logdensity
	double lnnorm;
};

#endif // !defined(AFX_TABULARDISTRIBUTION_H__96653477_03E8_44BF_A5F0_D44BD191C9F6__INCLUDED_)
