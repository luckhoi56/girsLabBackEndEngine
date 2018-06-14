// SimpleHomogeneous.cpp: implementation of the SimpleHomogeneous class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimpleHomogeneous.h"
#include "SampleSet.h"
#include "GammaDistribution.h"
#include "LogNormalDistribution.h"
#include "NormalDistribution.h"
#include "BetaDistribution.h"
#include "MetropolisSampler.h"
#include "MetropolisLNGenerator.h"
#include "MetropolisTLNGenerator.h"
#include "MetropolisNormalGenerator.h"
#include "LogNormalBinomialDistribution.h"
#include "LogNormalPoissonDistribution.h"

#include "ZBrentRootFinder.h"
#include "VegasIntegrator.h"
#include "SimpsonIntegrator.h"

#include "Parameters.h"
#include <math.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>

 #define TEST_CASES

/**
 * This class defines the procedures for simple homogeneous updating
 * procedures. The class is capable of handling the following cases:
 *
 *   PRIOR       LIKELIHOOD    METHOD
 *   ++++++++++++++++++++++++++++++++++++++++
 *   BETA        BINOMIAL      CONJUGATE PAIR
 *   GAMMA       POISSON       CONJUGATE PAIR
 *   LOGNORMAL   LOGNORMAL     CONJUGATE PAIR
 *   NORMAL      NORMAL        CONJUGATE PAIR
 *   LOGNORMAL   POISSON       M-H SAMPLING
 *   LOGNORMAL   BINOMIAL      M-H SAMPLING
 *   TABULAR     BINOMIAL      PIECEWISE INTEGRATION
 *   TABULAR     POISSON       PIECEWISE INTEGRATION
 *   TABULAR     LOGNORMAL     PIECEWISE INTEGRATION
 *   TABULAR     NORMAL        PIECEWISE INTEGRATION
 *
 * Cases marked as being solved as a conjugate pair undergo simpler 
 * parameter operations in order to arrive at the parameters for the
 * posterior distribution. Results are in the form of parametric 
 * distributions, and are exact.
 * 
 * Case marked as being solved through Metropolis-Hastings (M-H)
 * sampling are solved by generating samples from the product of two
 * distributions, one corresponding to the prior and one corresponding
 * to the likelihood. Note that Beta and Gamma distributions are 
 * substituted for the Binomial and Poisson distributions. Results are
 * in the form of percentiles, derived from the sample distributions. 
 * Accuracy depends on the number of samples.
 *
 * Cases marked as being solved by piecewise construction are solved by
 * performing integration operations within each interval, in which 
 * the prior is assumed to be uniform, and integration can be achieved
 * by calling the likelihood distribution getCDF functions. Results are
 * in the form of percentile values. Assuming the getCDF functions to 
 * be accurate, the results are exact.
 *
 * This class was designed as a substitute for existing classes that
 * were used by redcas. These classes are not at all designed to handle
 * evidence uncertainty.
 */

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

int SimpleHomogeneous::updateBetaBinomial(double alpha, double beta, double k, double N, double &alpha0, double &beta0)
{
	// this function has been verified OK
	alpha0 = alpha + k;
	beta0 = beta + N - k;
//	cout << "BB " << alpha << " " << beta << " " << k << " " << N << " -> " << alpha0 << " " << beta0 << endl;
	return 0;
}

int SimpleHomogeneous::updateGammaPoisson(double shape, double scale, double k, double T, double &shape0, double &scale0)
{
	// scale here has the interpretation of lambda (theta):
	// Gamma mean = alpha / lambda

	// this function has been verified OK
	shape0 = shape + k;
	scale0 = scale + T;
	//cout << "GP " << shape << " " << scale << " " << k << " " << T << " -> " << shape0 << " " << scale0 << endl;
	return 0;
}

int SimpleHomogeneous::updateNormalNormal(double mu, double sigma, double nu, double tau, double &mu0, double &sigma0)
{
	// this function has been verified OK
	double sigma2 = sigma * sigma; // sigma squared
	double tau2 = tau * tau; // tau squared
	mu0 = (sigma2 * nu + tau2 * mu) / (sigma2 + tau2);
	sigma0 = 1 / sqrt(1 / sigma2 + 1 / tau2);
	//cout << "NN " << mu << " " << sigma << " " << nu << " " << tau << " -> " << mu0 << " " << sigma0 << endl;
	return 0;
}

int SimpleHomogeneous::updateLogNormalLogNormal(double nu, double tau, double mu, double sigma, double &nu0, double &tau0)
{
	// this function has been verified OK
	// w0 = tau^2/(tau^2 + sigma^2)
	// w1 = 1 - w0
	// nu0 = (mu*w0) + (nu*w1)
	// tau0 = (1/tau^2 + 1/sigma^2)^-0.5
	double sigma2 = sigma * sigma; // sigma squared
	double tau2 = tau * tau; // tau squared
	double wnu = sigma2 / (tau2 + sigma2);
	nu0 = nu * wnu + mu * (1 - wnu);
	tau0 = 1.0 / sqrt(1 / sigma2 + 1 / tau2);
	//cout << "LL " << nu << " " << tau << " " << mu << " " << sigma << " -> " << nu0 << " " << tau0 << endl;
	return 0;
}

int SimpleHomogeneous::updateLogNormalPoisson(double nu, double tau, double k, double T, int n, double *x, double *px,int size)
{
	double integral,mean,variance;
	return updateLogNormalPoisson(nu,tau,k,T,n,x,px,size,integral,mean,variance);
}

int SimpleHomogeneous::updateLogNormalPoisson(double nu, double tau, double k, double T, int samplesize, double *x, double *px,int arraysize, double & integral, double & mean, double & var)
{
/*
	integral = 1;
	// this function has been verified OK

	// Poisson likelihood computed using the following transformation:
	// PDF[GammaDistribution[k+1,1/T],lambda] / PDF[PoissonDistribution[lambda * T],k]] = T
	UpdateFunction function(new LogNormalDistribution(nu,tau),new GammaDistribution(1 / T,k + 1));
	Parameters start(1);
	start.setValue(0,0.1);
	//cout << "LP " << nu << " " << tau << " " << k << " " << T << " -> TABULAR" << endl;
	int retval = sampleHomogeneous(function,start,new MetropolisLNGenerator(),samplesize,arraysize,x,px);
*/

	int retval = 0;

	LogNormalPoissonDistribution lpd(nu,tau,k,T);

	mean = lpd.getMean();
	var = lpd.getVariance();

	for (int i = 1 ; i < arraysize - 1; i++) {
		px[i] = 1. * i / (arraysize - 1);
		x[i] = lpd.getQuantile(px[i]);

//		cout << x[i] << " " << px[i] << endl;

	}

	px[0] = 0;
	x[0] = lpd.getQuantile(0.1 / arraysize);
	px[arraysize - 1] = 1;
	x[arraysize - 1] = lpd.getQuantile(1 - 0.1 / arraysize);

	if (retval == 0) {
		UpdateFunction2 function2(new LogNormalDistribution(nu,tau),new GammaDistribution(1 / T,k + 1));
		SimpsonIntegrator simpson;
		integral = simpson.simpson145(x[0],x[arraysize - 1],function2,0.0001,20);
#ifdef _DEBUG
		cout << "Integral: " << integral << endl;
#endif
	}
//	if (retval == 0) {
//		double bounds[2];
//		double sd,chi2a;
//		bounds[0] = x[0];
//		bounds[1] = x[size - 1];
//		UpdateFunction2 function2(new LogNormalDistribution(nu,tau),new GammaDistribution(1 / T,k + 1));
//		VegasIntegrator vgsi;
//		vgsi.vegas(bounds,1,function,0,2000,7,0,&integral,&sd,&chi2a,9876765);
#ifdef _DEBUG
//	cout << "Integrating Log Poisson" << endl;
//	cout << "Integral: " << integral << " sd: " << sd << " chi2a: " << chi2a << endl;
#endif

//	}

	return retval;
}

int SimpleHomogeneous::updateLogNormalBinomial(double nu, double tau, double k, double N, int n, double *x, double *px, int size)
{
	double integral,mean,variance;
	return updateLogNormalBinomial(nu,tau,k,N,n,x,px,size,integral,mean,variance);
}

int SimpleHomogeneous::updateLogNormalBinomial(double nu, double tau, double k, double N, int samplesize, double *x, double *px, int arraysize, double & integral, double & mean, double & var)
{
/*
	integral = 1;
	// this function has been verified OK

	// Binomial likelihood computed using the following transformation:
	// PDF[BetaDistribution[k+1,N-k+1],q] / PDF[BinomialDistribution[N,q],k]] = constant for all q
	UpdateFunction function(new LogNormalDistribution(nu,tau),new BetaDistribution(k + 1,N - k + 1));
	Parameters start(1);
	start.setValue(0,0.1);
#ifdef _DEBUG
	cout << "LB " << nu << " " << tau << " " << k << " " << N << " -> TABULAR" << endl;
#endif
	int retval = sampleHomogeneous(function,start,new MetropolisTLNGenerator(),samplesize,arraysize,x,px);
*/

	int retval = 0;

	LogNormalBinomialDistribution lbd(nu,tau,k,N);

	mean = lbd.getMean();
	var = lbd.getVariance();

	for (int i = 1 ; i < arraysize - 1; i++) {
		px[i] = 1. * i / (arraysize - 1);
		x[i] = lbd.getQuantile(px[i]);
	}

	px[0] = 0;
	x[0] = lbd.getQuantile(0.1 / arraysize);
	px[arraysize - 1] = 1;
	x[arraysize - 1] = lbd.getQuantile(1 - 0.1 / arraysize);

	if (retval == 0) {
		double lower = lbd.getLowerBound();
		double upper = lbd.getUpperBound();
		UpdateFunction2 function2(new LogNormalDistribution(nu,tau),new BetaDistribution(k + 1,N - k + 1));
		SimpsonIntegrator simpson;
		integral = simpson.simpson145(lower,upper,function2,0.0001,20);
	}

	return retval;
}

int SimpleHomogeneous::updateTabularBinomial(double * values, double * probs, int size, double k, double N, int n, double * values0, double * probs0, int size0)
{
	// this function has been found to be OK (preliminary result)
//	cout << "TB " << k << " " << N << " -> TABULAR" << endl;
//	cout << BetaDistribution(k + 1,N - k + 1).getMean() << endl;
	return constructTabular(values,probs,size,BetaDistribution(k + 1,N - k + 1),values0,probs0,size0);
}

int SimpleHomogeneous::updateTabularPoisson(double * values, double * probs, int size, double k, double T, int n, double * values0, double * probs0, int size0)
{
//	cout << "TP " << k << " " << T << " -> TABULAR" << endl;
//	cout << GammaDistribution(1 / T,k + 1).getMean() << endl;
	return constructTabular(values,probs,size,GammaDistribution(1 / T,k + 1),values0,probs0,size0);
}

int SimpleHomogeneous::updateTabularNormal(double * values, double * probs, int size, double mu, double sigma, int n, double * values0, double * probs0, int size0)
{
	//cout << "TB " << mu << " " << sigma << " -> TABULAR" << endl;
	return constructTabular(values,probs,size,NormalDistribution(mu,sigma),values0,probs0,size0);
}

int SimpleHomogeneous::updateTabularLogNormal(double * values, double * probs, int size, double nu, double tau, int n, double * values0, double * probs0, int size0)
{
	//cout << "TB " << nu << " " << tau << " -> TABULAR" << endl;
	return constructTabular(values,probs,size,LogNormalDistribution(nu,tau),values0,probs0,size0);
}
/**
 * Sampling procedure used by LogNormal-Poisson and LogNormal-Binomial combinations.
 * This function calls the Metropolis-Hastings sampler, and constructs the
 * table of percentiles from the returned sample distribution.
 */
int SimpleHomogeneous::sampleHomogeneous(Function & function, Parameters & start, MetropolisGenerator * generator, int samples, int size, double * values, double * probs)
{
	// this function has been verified OK

	SampleSet sampleset;	
	MetropolisSampler sampler(sampleset);
	sampler.setGenerator(generator);
	sampler.setSampleSize(size * 100 > samples ? size * 100 : samples);

	sampler.sample(function);

	Distribution marginal;
	sampleset.getMarginal(0,marginal);

	for (int i = 0 ; i < size ; i++) {
		probs[i] = i * 1.0 / (size - 1);		
		values[i] = marginal.getPercentile(0.0000001 + i * 0.9999998 / (size - 1));
#ifdef _DEBUG
	   if (i % (size / 10) == 0) cout << values[i] << " " << probs[i] << endl;
#endif

	}
	return 0;
}

/**
 * Piecewise integration procedure used by Tabular - XXXX combinations.
 * This function finds the desired percentiles of the posterior distribution
 * by constructing a table of cumulative probabilities at the border of 
 * each interval (specified by vin). Interpolation is then used to 
 * reconstruct the desired percentile of the original likelihood function,
 * itself a probability distribution.
 */
int SimpleHomogeneous::constructTabular(double * vin, double * pin, int insize, ParametricDistribution &like, double * vout, double * pout, int outsize)
{
	// preliminary tests indicate this function is OK

	double * cumulatives = new double[insize];

	// compute posterior cumulative at borders of intervals
	cumulatives[0] = 0;
	for (int i = 1 ; i < insize ; i++) {
		double cp = (pin[i] - pin[i - 1]) / (vin[i] - vin[i -1]);
		double lp = like.getCDF(vin[i]) - like.getCDF(vin[i-1]);
		cumulatives[i] = cumulatives[i - 1] + cp * lp;
	}
	for (int j = 0 ; j < insize ; j++) cumulatives[j] /= cumulatives[insize - 1];

	int index = 1;
	double factor = 1. / (outsize - 1);

	for (int k = 1 ; k < outsize - 1 ; k++) {
		// advance to interval containing desired cumulative
		while(cumulatives[index] < k * factor) index++;
		// find likelihood percentile corresponding to desired percentile
		double cum0 = like.getCDF(vin[index - 1]);
		double cum1 = like.getCDF(vin[index]);
		double cumx = cum0 + (cum1 - cum0) * (k * factor - cumulatives[index - 1]) / (cumulatives[index] - cumulatives[index - 1]);
		vout[k] = like.getQuantile(cumx);
		pout[k] = k * factor;
#ifdef _DEBUG
		if (k % (outsize / 10) == 0) cout << pout[k] << " " << vout[k] << endl;
#endif
	}

	// copy extremes of input tabular distribution
	vout[0] = vin[0];
	pout[0] = 0;
	vout[outsize - 1] = vin[insize - 1]; 
	pout[outsize - 1] = 1;

	delete [] cumulatives;
	return 0;
}

/////////////////////////////////////////////////////////////////

void SimpleHomogeneous::runTestCases()
{
	testLogNormalBinomial();
	testLogNormalPoisson();
//	testTabular();
}

void SimpleHomogeneous::testLogNormalBinomial()
{
#ifdef TEST_CASES

	ofstream datafile("lnbcases.dat");
	ifstream rsltfile("lnbrslts.dat");

	bool writedata = false;
	if (datafile.is_open()) writedata = true;
	bool readdata = false;
	if (rsltfile.is_open()) readdata = true;

	char buffer1[256];
	char buffer2[256];
	double x[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	double px[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	double y[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	double v[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	double s[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	int cases = 57;
	int iterations = 1;
	double m[21];
	int samplesize = 20000;

	m[0]=0;
	m[20]=0;

	// priors number 1 trough 9
	int prior[57]      = { 1, 5, 9,13,17, 3, 7,11,15, 2, 6,10,14,18, 4, 8,12,16,
						  1,17, 3, 7,11,15, 2, 6,10,14,18, 4, 8,12,16, 1, 5, 9,
						  10,14,18, 4, 8,12,16, 1, 5, 9,13,17, 3, 7,11,15, 2, 6,
						   5, 9,13};
		
	// likelihoods number 1 through 18
	int likelihood[57] = { 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6,
	                       7, 7, 7, 8, 8, 8, 9, 9, 9,10,10,10,12,12,12,13,13,13,
						  14,14,14,15,15,15,18,18,18,19,19,19,20,20,20,24,24,24,
						  25,25,25};

	double nu,tau,k,N;

	cout << "\\chapter{LogNormal-Binomial Test Cases}" << endl;

	cout.precision(3);
	cout.setf(ios::scientific,ios::floatfield);

    _strdate( buffer1 );
	_strtime( buffer2 );

	cout << "Test started on " << buffer1 << " at " << buffer2 << endl;

	for (int i = 0 ; i < cases ; i++) {

		switch ((prior[i] - 1) / 6) {
			case 0 : nu = log(1e-8); break;
			case 1 : nu = log(1e-5); break;
			case 2 : nu = log(1e-2); break;
		}

		switch ((prior[i] - 1) % 6) {
			case 0 : tau = 0.2; break;
			case 1 : tau = 0.5; break;
			case 2 : tau = 1.0; break;
			case 3 : tau = 2.0; break;
			case 4 : tau = 3.5; break;
			case 5 : tau = 5.0; break;
		}

		switch ((likelihood[i] - 1) % 5) {
			case 0 : N = 1; break;
			case 1 : N = 3; break;
			case 2 : N = 10; break;
			case 3 : N = 100; break;
			case 4 : N = 1000; break;
		}

		switch ((likelihood[i] - 1) / 5) {
			case 0 : k = 0; break;
			case 1 : k = 1; break;
			case 2 : k = 3; break;
			case 3 : k = 10; break;
			case 4 : k = 100; break;
		}

		if (k > N) 
			cout << i << ": BAD TEST CASE " << likelihood[i] << "." << prior[i] << endl;

		int badresults = 0;
	    bool noresults = false;

		if (readdata) {
			for (int r = 1 ; r <= 19 ; r++) {
				noresults = rsltfile.eof();
				if (noresults)
					m[r] = 0;
				else
					rsltfile >> m[r];
			}
			noresults = rsltfile.eof();
			if (!noresults) rsltfile >> badresults;
		} else {
			for (int r = 1 ; r <= 19 ; r++) m[r] = 0;
			noresults = true;
		}

		cout << "\\pagebreak\n\\section{Test Case " << likelihood[i] << "." << prior[i] << "}" << endl << endl;
		cout << "\\begin{center}" << endl;
		cout << "\\begin{tabular}{|r|r|r|r|r|}\\hline" << endl;
		cout << "\\multicolumn{5}{|c|}{\\textsc{Input Values}} \\\\ \\hline" << endl;
		cout << "\\multicolumn{2}{|c|}{\\textsc{Parameter}} & \\multicolumn{3}{|c|}{\\textsc{Value}} \\\\ \\hline" << endl;
		cout << "\\multicolumn{2}{|l|}{Prior.$\\nu$} & \\multicolumn{3}{|r|}{\\texttt{" << nu << "}} \\\\" << endl;
		cout << "\\multicolumn{2}{|l|}{Prior.$\\tau$} & \\multicolumn{3}{|r|}{\\texttt{" << tau << "}} \\\\" << endl;
		cout << "\\multicolumn{2}{|l|}{Evidence.Events} & \\multicolumn{3}{|r|}{\\texttt{" << k << "}} \\\\" << endl;
		cout << "\\multicolumn{2}{|l|}{Evidence.Trials} & \\multicolumn{3}{|r|}{\\texttt{" << N << "}} \\\\" << endl;
		cout << "\\multicolumn{2}{|l|}{Test.Iterations} & \\multicolumn{3}{|r|}{\\texttt{" << iterations << "}} \\\\" << endl;
		cout << "\\multicolumn{2}{|l|}{Algorithm.Samples} & \\multicolumn{3}{|r|}{\\texttt{" << samplesize << "}} \\\\ \\hline" << endl;

		for (int i0 = 0 ; i0 < 21 ; i0++) {
			x[i0] = 0;
			px[i0] = 0;
			s[i0] = 0;
			v[i0] = 0;
			y[i0] = 0;
		}

		double integral,lbmean,lbvar;

		for (int i1 = 0; i1 < iterations ; i1++) {
			updateLogNormalBinomial(nu,tau,k,N,samplesize,y,px,21,integral,lbmean,lbvar);
			for (int i2 = 0 ; i2 < 21 ; i2++) {
				x[i2] += y[i2];
				v[i2] += y[i2] * y[i2];
			}
		}

		for (int i3 = 0 ; i3 < 21 ; i3++) {
			x[i3] /= iterations;
			s[i3] = iterations == 1 ? 0 : pow((v[i3] - iterations * x[i3] * x[i3])/(iterations - 1),0.5);
		}

		cout << "\\multicolumn{5}{|c|}{\\textsc{Results}} \\\\ \\hline" << endl;
		cout << "\\multicolumn{1}{|c|}{\\textsc{Probability}} & \\multicolumn{1}{|c|}{\\textsc{Mathematica}} & \\multicolumn{1}{|c|}{\\textsc{M-H MN}} & \\multicolumn{1}{|c|}{\\textsc{Ratio}} & \\multicolumn{1}{|c|}{\\textsc{M-H SD/MN}} \\\\ \\hline" << endl;

		for (int j = 0 ; j < 21 ; j++)
			cout << "\\texttt{" << px[j] << "} & \\texttt{" << m[j] << "} & \\texttt{" << x[j] << "} & \\texttt{" << m[j]/x[j] <<  "} & \\texttt{" << s[j]/x[j] <<  "} \\\\" << endl;
		if (noresults) cout << "\\hline\\multicolumn{5}{|c|}{Mathematica: no or incomplete results} \\\\" << endl;
		else if (badresults == 1) cout << "\\hline\\multicolumn{5}{|c|}{Mathematica: numerical instabilities} \\\\" << endl << endl;
		cout << "\\hline\\end{tabular}" << endl << endl;
		cout << "\\end{center}" << endl;
		cout << "Integral of the entire distribution: " << integral << endl;

		if (writedata) {
			double min = x[10] / pow(x[10] / x[0],2);
			double max = x[10] * pow(x[20] / x[10],2);
			if (max > 1) max = 1;
			datafile << "lnb" << likelihood[i] << "." << prior[i] << " " << nu << " " << tau << " " << k << " " << N << " " << min << " " << max << " " << x[10] << endl;
		}
	}

	cout << "\\pagebreak\n\\section{Completion of the Test}" << endl;
    _strdate( buffer1 );
	_strtime( buffer2 );
	cout << "Test completed on " << buffer1 << " at " << buffer2 << endl;

	cout.unsetf(ios::scientific);
	cout.unsetf(ios::floatfield);
	cout.precision(0);

#endif
}

void SimpleHomogeneous::testLogNormalPoisson()
{
#ifdef TEST_CASES

	ofstream datafile("lnpcases.dat");
	ifstream rsltfile("lnprslts.dat");

	bool writedata = false;
	if (datafile.is_open()) writedata = true;
	bool readdata = false;
	if (rsltfile.is_open()) readdata = true;


	char buffer1[256];
	char buffer2[256];
	double x[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	double px[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	double y[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	double v[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	double s[21] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	int cases = 54;
	double m[21];
	int samplesize = 20000;

	int iterations = 1;

	m[0]=0;
	m[20]=0;

	// priors number 1 trough 9
	int prior[54]      = { 1, 5, 9,13,17, 3, 7,11,15, 2, 6,10,14,18, 4, 8,12,16,
						  13,17, 3, 7,11,15, 2, 6,10,14,18, 4, 8,12,16, 1, 5, 9,
						  10,14,18, 4, 8,12,16, 1, 5, 9,13,17, 3, 7,11,15, 2, 6};
		
	// likelihoods number 1 through 18
	int likelihood[54] = { 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6,
	                       7, 7, 7, 8, 8, 8, 9, 9, 9,10,10,10,11,11,11,12,12,12,
						  13,13,13,14,14,14,15,15,15,16,16,16,17,17,17,18,18,18};

	double nu,tau,t,k;

	cout << "\\chapter{LogNormal-Poisson Test Cases}" << endl;

	cout.precision(3);
	cout.setf(ios::scientific,ios::floatfield);

//	cout.setf(ios::scientific);

	cout << "\\begin{center}" << endl;
	cout << "\\begin{tabular}{|c|c|}\\hline" << endl;
	cout << "\\multicolumn{2}{|c|}{\\textsc{Input Values}} \\\\ \\hline" << endl;
	cout << "\\textsc{Parameter} & \\textsc{Description} \\\\ \\hline" << endl;
	cout << "Prior.$\\nu$ & underlying normal mn\\\\" << endl;
	cout << "Prior$\\tau$ & underlying normal sd\\\\" << endl;
	cout << "Evidence.Events & number of events \\\\" << endl;
	cout << "Evidence.Time & time interval \\\\ \\hline" << endl;
	cout << "\\end{tabular}" << endl;
	cout << "\\end{center}" << endl;

    _strdate( buffer1 );
	_strtime( buffer2 );
	cout << "Test started on " << buffer1 << " at " << buffer2 << endl;

	for (int i = 0 ; i < cases ; i++) {

		switch ((prior[i] - 1) / 6) {
			case 0 : nu = log(1e-8); break;
			case 1 : nu = log(1e-5); break;
			case 2 : nu = log(1e-2); break;
		}

		switch ((prior[i] - 1) % 6) {
			case 0 : tau = 0.2; break;
			case 1 : tau = 0.5; break;
			case 2 : tau = 1.0; break;
			case 3 : tau = 2.0; break;
			case 4 : tau = 3.5; break;
			case 5 : tau = 5.0; break;
		}

		switch ((likelihood[i] - 1) / 3) {
			case 0 : k = 0; break;
			case 1 : k = 1; break;
			case 2 : k = 5; break;
			case 3 : k = 10; break;
			case 4 : k = 100; break;
			case 5 : k = 1000; break;
		}

		switch ((likelihood[i] - 1) % 3) {
			case 0 : t = 10; break;
			case 1 : t = 1000; break;
			case 2 : t = 100000; break;
		}

		int badresults = 0;
	    bool noresults = false;

		if (readdata) {
			for (int r = 1 ; r <= 19 ; r++) {
				noresults = rsltfile.eof();
				if (noresults)
					m[r] = 0;
				else
					rsltfile >> m[r];
			}
			noresults = rsltfile.eof();
			if (!noresults) rsltfile >> badresults;
		} else {
			for (int r = 1 ; r <= 19 ; r++) m[r] = 0;
			noresults = true;
		}

		cout << "\\pagebreak\n\\section{Test Case " << likelihood[i] << "." << prior[i] << "}" << endl << endl;
		cout << "\\begin{center}" << endl;
		cout << "\\begin{tabular}{|r|r|r|r|r|}\\hline" << endl;
		cout << "\\multicolumn{5}{|c|}{\\textsc{Input Values}} \\\\ \\hline" << endl;
		cout << "\\multicolumn{2}{|c|}{\\textsc{Parameter}} & \\multicolumn{3}{|c|}{\\textsc{Value}} \\\\ \\hline" << endl;
		cout << "\\multicolumn{2}{|l|}{Prior.$\\nu$} & \\multicolumn{3}{|r|}{\\texttt{" << nu << "}} \\\\" << endl;
		cout << "\\multicolumn{2}{|l|}{Prior.$\\tau$} & \\multicolumn{3}{|r|}{\\texttt{" << tau << "}} \\\\" << endl;
		cout << "\\multicolumn{2}{|l|}{Evidence.Events} & \\multicolumn{3}{|r|}{\\texttt{" << k << "}} \\\\" << endl;
		cout << "\\multicolumn{2}{|l|}{Evidence.Time} & \\multicolumn{3}{|r|}{\\texttt{" << t << "}} \\\\" << endl;
		cout << "\\multicolumn{2}{|l|}{Algorithm.Samples} & \\multicolumn{3}{|r|}{\\texttt{" << samplesize << "}} \\\\ \\hline" << endl;
//		cout << "\\end{tabular}" << endl << endl;

//		cout << "\\vspace{0.5in}" << endl;


		for (int i0 = 0 ; i0 < 21 ; i0++) {
			x[i0] = 0;
			px[i0] = 0;
			s[i0] = 0;
			v[i0] = 0;
			y[i0] = 0;
		}

//		updateLogNormalPoisson(nu,tau,k,t,21,x,px);

		double integral,lpmean,lpvar;

		for (int i1 = 0; i1 < iterations ; i1++) {
			updateLogNormalPoisson(nu,tau,k,t,samplesize,y,px,21,integral,lpmean,lpvar);
			for (int i2 = 0 ; i2 < 21 ; i2++) {
				x[i2] += y[i2];
				v[i2] += y[i2] * y[i2];
			}
		}

		for (int i3 = 0 ; i3 < 21 ; i3++) {
			x[i3] /= iterations;
			s[i3] = iterations == 1 ? 0 : pow((v[i3] - iterations * x[i3] * x[i3])/(iterations-1),0.5);
		}

//		cout << "\\begin{tabular}{|r|r|r|r|r|}\\hline" << endl;
		cout << "\\multicolumn{5}{|c|}{\\textsc{Results}} \\\\ \\hline" << endl;
		cout << "\\multicolumn{1}{|c|}{\\textsc{Probability}} & \\multicolumn{1}{|c|}{\\textsc{Mathematica}} & \\multicolumn{1}{|c|}{\\textsc{M-H MN}} & \\multicolumn{1}{|c|}{\\textsc{Ratio}} & \\multicolumn{1}{|c|}{\\textsc{M-H SD/MN}} \\\\ \\hline" << endl;

		for (int j = 0 ; j < 21 ; j++)
			cout << "\\texttt{" << px[j] << "} & \\texttt{" << m[j] << "} & \\texttt{" << x[j] << "} & \\texttt{" << m[j]/x[j] <<  "} & \\texttt{" << s[j]/x[j] <<  "} \\\\" << endl;
//		cout << "\\hline\\end{tabular}" << endl << endl;
		if (noresults) cout << "\\hline\\multicolumn{5}{|c|}{Mathematica: no or incomplete results} \\\\" << endl;
		else if (badresults == 1) cout << "\\hline\\multicolumn{5}{|c|}{Mathematica: numerical instabilities} \\\\" << endl << endl;
		cout << "\\hline\\end{tabular}" << endl << endl;
		cout << "\\end{center}" << endl;
		cout << "Integral of the entire distribution: " << integral << endl;

		if (writedata) {
			double min = x[10] / pow(x[10] / x[0],2);
			double max = x[10] * pow(x[20] / x[10],2);
			datafile << "lnp" << likelihood[i] << "." << prior[i] << " " << nu << " " << tau << " " << k << " " << t << " " << min << " " << max << " " << x[10] << endl;
		}
	}

	cout << "\\pagebreak\n\\section{Completion of the Test}" << endl;
    _strdate( buffer1 );
	_strtime( buffer2 );
	cout << "Test completed on " << buffer1 << " at " << buffer2 << endl;

	cout.unsetf(ios::scientific);
	cout.unsetf(ios::floatfield);
	cout.precision(0);

#endif
}

void SimpleHomogeneous::testTabular()
{
	int procedure = 0;
	double * p;
	double * v;
	int s;
	
	ParametricDistribution * like;

	double pin[100];
	double vin[100];
	double pout[21];
	double vout[21];

	double s1 = 2;
	double v1[2] = {0.25,0.75};
	double p1[2] = {0.00,1.00};

	double s2 = 4;
	double v2[4] = {0.15,0.40,0.60,0.85};
	double p2[4] = {0.00,0.25,0.25,1.00};

	double par1,par2;

	int points = 0;

	for (int testcase = 0 ; testcase < 2 ; testcase++) {

		switch(testcase) {
		case 0: par1 = 2; par2 = 8; procedure = 0; break;
		case 1: par1 = 1; par2 = 100; procedure = 0; break;
		}

		switch(procedure) {
		case 0: like = new BetaDistribution(par1 + 1,par2 - par1 + 1); break;
		}

		for (int table = 0 ; table < 2 ; table++) {

			switch(table) {
			case 0: s = s1; p = p1; v = v1; break;
			case 1: s = s2; p = p2; v = v2; break;
			}

			for (int i = 0 ; i < s ; i++) {
				pin[i] = p[i];
				vin[i] = like->getQuantile(v[i]);
			}

			switch(procedure) {
			case 0: updateTabularBinomial(vin,pin,s,par1,par2,0,vout,pout,21);
			}

			for (int j = 0 ; j < 21 ; j++) {
				cout << pout[j] << " " << vout[j] << endl;
			}
		}

		delete like;
	}
}


// ***********************************************************


UpdateFunction::UpdateFunction(ParametricDistribution *prior,ParametricDistribution *like)
{
	this->like = like;
	this->prior = prior;
}

UpdateFunction::~UpdateFunction()
{
	delete prior;
	delete like;
}

double UpdateFunction::evaluate(const Parameters & params) const
{
	double x = params.getValue(0);
    return prior->getLogPDF(x) + like->getLogPDF(x);
}

//***************************************************************************

UpdateFunction2::UpdateFunction2(ParametricDistribution *prior,ParametricDistribution *like)
{
	this->like = like;
	this->prior = prior;
	this->ln_factor = 0;
	this->moment = 0;
}

UpdateFunction2::UpdateFunction2(ParametricDistribution *prior,ParametricDistribution *like, double ln_factor,bool times_x)
{
	this->like = like;
	this->prior = prior;
	this->ln_factor = ln_factor;
	this->moment = 0;
	this->times_x = times_x;
}

UpdateFunction2::~UpdateFunction2()
{
	delete prior;
	delete like;
}

double UpdateFunction2::evaluate(const Parameters & params) const
{
	double x = params.getValue(0);
	if (x == 0) return 0;

	int sw = this->times_x ? moment + 1 : moment;

	switch (moment) {
	case 0: return exp( prior->getLogPDF(x) + like->getLogPDF(x) + ln_factor);
	case 1: return exp( prior->getLogPDF(x) + like->getLogPDF(x) + ln_factor) * x;
	case 2: return exp( prior->getLogPDF(x) + like->getLogPDF(x) + ln_factor) * pow(x,2);
	case 3: return exp( prior->getLogPDF(x) + like->getLogPDF(x) + ln_factor) * pow(x,3);
	}
	return 0;
}