// $Header: C:/cvsrepo/ptdev/native/redcas/parametric/GammaDistribution.cpp,v 1.2 2001/05/04 20:45:50 fgroen Exp $

// GammaDistribution.cpp: implementation of the GammaDistribution class.
//
//////////////////////////////////////////////////////////////////////

#include "GammaDistribution.h"
#include "../dcdf/DCDFLIBInterface.h"
#include "../ran/RANLIBInterface.h"
#include "../global/Error.h"
#include "../global/Distribution.h"
#include <math.h>
#include <iostream> 

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GammaDistribution::GammaDistribution(double beta, double alpha)
{
	if (checkParameters(beta,alpha)) {
		this->beta = beta;
		this->alpha = alpha;
		this->ln_gamma = gammaln(alpha);
	} else throw INVALID_ARGUMENT;	
}

double GammaDistribution::getSample() const
{
	return RANLIBInterface::generateGammaDeviate(1/beta,alpha);
}

double GammaDistribution::getQuantile(double percentage) const
{
	if (percentage < 0 || percentage > 1) throw INVALID_ARGUMENT;
	return DCDFLIBInterface::computeGammaQuantile(alpha,1/beta,percentage);
}

double GammaDistribution::getMean() const
{
	return beta * alpha;
}

double GammaDistribution::getVariance() const
{
	return beta * beta * alpha;
}

double GammaDistribution::getCDF(double x) const
{
	if (x == 0) return 0;
	if (x < 0) throw INVALID_ARGUMENT;
	return DCDFLIBInterface::computeGammaCDF(alpha,1/beta,x);
//	return getGammaIncomplete(alpha, x / beta);
}

double GammaDistribution::getLogPDF(double x) const
{
	if (x == 0) return 0;
	if (x < 0) throw INVALID_ARGUMENT;
//	return -alpha * log(beta) - gammaln(alpha) + (alpha - 1) * log(x) - (x / beta);
	return -alpha * log(beta) - ln_gamma + (alpha - 1) * log(x) - (x / beta);
}

bool GammaDistribution::checkParameters(double beta, double alpha)
{
	return beta > 0 && alpha > 0;
}

double GammaDistribution::gammaln(double x)
{
	return DCDFLIBInterface::computeLnGamma(x);
}

/**
 * Tests the sampling routine. 
 * Dumps a log of the results to cout in LaTeX format.
 */
void GammaDistribution::testGammaDeviates()
{
	int tstcases = 11;
	int alphas[11] = {200,100,50,20,12,10,7,5,4,3,2};
	int sizes[5] = {5000,10000,20000,50000,100000};

	cout << "\\chapter{Gamma Distribution Sampling}" << endl;	

	for (int tstcase = 0 ; tstcase < tstcases ; tstcase++) 
	{
		double beta = 1;
		double alpha = alphas[tstcase];
		Distribution samples[5];
		GammaDistribution gammadist = GammaDistribution(beta,alpha);

		cout << "\\pagebreak\n\\section{Test Case : Shape = " << alpha << "}" << endl;
		cout << "\\begin{center}" << endl;
		cout << "\\begin{tabular}{|c|c|c|c|c|c|c|c|}\\hline" << endl;
		cout << "Percentile & Value & Percentile & 5K & 10K & 20K & 50K & 100K \\\\ \\hline" << endl;

		for (int i = 0; i < 5 ; i++) 
		{
			int size = sizes[i];
			samples[i].clear(size);
			for (int j = 0 ; j < size ; j++) 
				samples[i].add(gammadist.getSample(),1./size);
		}

		double f;

		for (int k = 0 ; k < 21 ; k++) 
		{
			switch(k) {
				case 0  : f = 0.001;break;
				case 1	: f = 0.002;break;
				case 2  : f = 0.005;break;
				case 3  : f = 0.01;break;
				case 4  : f = 0.02;break;
				case 5  : f = 0.05;break;
				case 6  : f = 0.10;break;
				case 7  : f = 0.20;break;
				case 8  : f = 0.30;break;
				case 9  : f = 0.40;break;
				case 10 : f = 0.50;break;
				case 11 : f = 0.60;break;
				case 12 : f = 0.70;break;
				case 13 : f = 0.80;break;
				case 14 : f = 0.90;break;
				case 15 : f = 0.95;break;
				case 16 : f = 0.98;break;
				case 17 : f = 0.99;break;
				case 18 : f = 0.995;break;
				case 19 : f = 0.998;break;
				case 20 : f = 0.999;break;
			}
			double q = gammadist.getQuantile(f);
			cout << f << " & " << q << " & " << gammadist.getCDF(q) << " & " << samples[0].getPercentile(f) << " & " << samples[1].getPercentile(f) << " & " << samples[2].getPercentile(f) << " & " << samples[3].getPercentile(f) << " & " << samples[4].getPercentile(f) << " \\\\" << endl;
		}

		cout << "\\hline\\end{tabular}\\end{center}" << endl;
	}
}
