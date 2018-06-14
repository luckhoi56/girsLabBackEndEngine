// SimpleHomogeneous.h: interface for the SimpleHomogeneous class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMPLEHOMOGENEOUS_H__92A19F9F_71F7_40C5_8986_24F640819018__INCLUDED_)
#define AFX_SIMPLEHOMOGENEOUS_H__92A19F9F_71F7_40C5_8986_24F640819018__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Function.h"
#include "ParametricDistribution.h"
#include "Parameters.h"
#include "MetropolisGenerator.h"

class SimpleHomogeneous  
{
public:
	static int updateLogNormalPoisson(double nu, double tau, double k, double T, int n, double * x, double * px, int size);
	static int updateLogNormalPoisson(double nu, double tau, double k, double T, int n, double * x, double * px, int size, double & integral, double & mean, double & var);
	static int updateLogNormalBinomial(double nu, double tau, double k, double N, int n, double * x, double * px, int size);
	static int updateLogNormalBinomial(double nu, double tau, double k, double N, int n, double * x, double * px, int size, double & integral, double & mean, double & var);
	static int updateLogNormalLogNormal(double nu, double tau, double mu, double sigma, double &nu0, double &tau0);
	static int updateNormalNormal(double mu, double sigma, double nu, double tau, double &mu0, double &sigma0);
	static int updateGammaPoisson(double shape, double scale, double k, double T, double &shape0, double &scale0);
	static int updateBetaBinomial(double alpha, double beta, double k, double N, double &alpha0, double &beta0);
	static int updateTabularBinomial(double * values, double * probs, int size, double k, double N, int n, double * values0, double * probs0, int size0);
	static int updateTabularPoisson(double * values, double * probs, int size, double k, double T, int n, double * values0, double * probs0, int size0);
	static int updateTabularNormal(double * values, double * probs, int size, double mu, double sigma, int n, double * values0, double * probs0, int size0);
	static int updateTabularLogNormal(double * values, double * probs, int size, double nu, double tau, int n, double * values0, double * probs0, int size0);

	static void runTestCases();
	virtual ~SimpleHomogeneous() {};

protected:
	static int constructTabular(double * vin, double * pin, int insize, ParametricDistribution &like, double * vout, double * pout, int outsize);
	static int sampleHomogeneous(Function & function, Parameters & start, MetropolisGenerator * generator, int samples, int size, double * values, double * probs);

	static void testLogNormalBinomial();
	static void testLogNormalPoisson();
	static void testTabular();
};

class UpdateFunction : public Function
{
public:
	UpdateFunction(ParametricDistribution *prior,ParametricDistribution *like);
	virtual ~UpdateFunction();
	double evaluate(const Parameters & params) const;
	virtual int getDimension() const {return 1;}
private:
	ParametricDistribution *prior;
	ParametricDistribution *like;
};

/**
 * Function that returns the value for 
 *    PDF(prior,x) + PDF(like,x) + exp(ln_factor) * x^moment
 * Value of moment must be either 0, 1, or 2. Used to compute the integral, 
 * mean, and variance.
 */
class UpdateFunction2 : public Function
{
public:
	UpdateFunction2(ParametricDistribution *prior,ParametricDistribution *like);
	UpdateFunction2(ParametricDistribution *prior,ParametricDistribution *like, double ln_factor, bool times_x = false);
	virtual ~UpdateFunction2();
	double evaluate(const Parameters & params) const;
	virtual int getDimension() const {return 1;}
	void setMoment(int moment) {this->moment = moment;}
private:
	ParametricDistribution *prior;
	ParametricDistribution *like;
	double ln_factor;
	int moment;
	bool times_x; // flag: multiply function by x.
};


#endif // !defined(AFX_SIMPLEHOMOGENEOUS_H__92A19F9F_71F7_40C5_8986_24F640819018__INCLUDED_)
