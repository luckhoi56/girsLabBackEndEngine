// ParametricDistributionFitter.h: interface for the ParametricDistributionFitter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARAMETRICDISTRIBUTIONFITTER_H__CC8014E2_78E6_44F5_857D_9911A9ACC6EE__INCLUDED_)
#define AFX_PARAMETRICDISTRIBUTIONFITTER_H__CC8014E2_78E6_44F5_857D_9911A9ACC6EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Function.h"
#include "Parameters.h"

class ParametricDistributionFitter  
{
public:
	ParametricDistributionFitter();
	virtual ~ParametricDistributionFitter();
	static double* FitGamma(int points, double x[]);
	static double* FitBeta(int points, double x[]);
	static double* FitNormal(int points, double x[], double y[]);
	static double* FitLogNormal(int points, double x[], double y[]);

	static void TestFitGamma(void);
	static void TestFitBeta(void);

private:
	static void FitBetaApproxMLE(int points, double x[], double result[]);
	static double* FitBetaMLE(int points, double x[]);
};

class GammaDistAlphaMLEFunction : public Function
{
public:
	GammaDistAlphaMLEFunction(int datapoints, double x[]);
	virtual ~GammaDistAlphaMLEFunction();
	double evaluate(const Parameters & params) const;
	virtual int getDimension() const {return 1;}
private:
	int dataPoints;
	double *xValues;

};


#endif // !defined(AFX_PARAMETRICDISTRIBUTIONFITTER_H__CC8014E2_78E6_44F5_857D_9911A9ACC6EE__INCLUDED_)
