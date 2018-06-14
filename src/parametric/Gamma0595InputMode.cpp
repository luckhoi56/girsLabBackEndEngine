// Gamma0595InputMode.cpp: implementation of the Gamma0595InputMode class.
//
//////////////////////////////////////////////////////////////////////

#include "Gamma0595InputMode.h"
#include <math.h>
#include "DCDFLIBInterface.h"

#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Gamma0595InputMode::Gamma0595InputMode()
{
	lower_z = 0.05;
	upper_z = 0.95;
}

Gamma0595InputMode::Gamma0595InputMode(double plo, double phi)
{
	lower_z = plo;
	upper_z = phi;
}

Gamma0595InputMode::~Gamma0595InputMode()
{
	
}

/**
 * This method computes the shape and scale parameters for the Gamma distribution
 * with specified percentile values.
 * 
 * The shape has the beta interpretation (mean = shape * scale).
 *
 * INPUTS:
 *   i1 : lower percentile
 *   i2 : upper percentile
 *   o1 : shape
 *   o2 : scale
 */
int Gamma0595InputMode::convert(double i1, double i2, double & o1, double & o2) 
{

//	cout << "Computing parameters for " << i1 << " " << i2 << endl;

	double r = i2 / i1;
	double k;
	int result = computeShape(r,lower_z,upper_z,k);

	if (result == 0) {
		double standard_upper = DCDFLIBInterface::computeGammaQuantile(k, 1, upper_z);
		o2 = i2 / standard_upper;
		o1 = k;
//		double qlo = DCDFLIBInterface::computeGammaQuantile(o1, 1/o2, lower_z);
//		double qhi = DCDFLIBInterface::computeGammaQuantile(o1, 1/o2, upper_z);
//		cout << "Scale = " << o2 << " -> " << qlo << "," << qhi << endl;

	} else {
		o1 = 1;
		o2 = 1;
	}
	return result;
}

/**
 * Computes the shape parameter of the Gamma distribution based
 * on the ratio of upper and lower percentile. The shape parameter
 * is obtained using a bisection search.
 *
 * This implementation is specific to the ratio of 5th and 95th percentiles,
 * because of the precomputed values, and is capable of computing the
 * shape for ratio values between 1.004 and 1E25.
 * 
 * This method returns if no good solution was found within the maximum
 * allowed number of iterations.
 */
int Gamma0595InputMode::computeShape(double r, double &k) const
{

	const int ratio_size = 11;

	// table of precomputed values of Log[Quantile[dist, zhi]/Quantile[dist, zlo]]
	// where dist = GammaDistribution[10^k, 1]
	double ln_ratios [ratio_size]= {
		58.988394354162  , 29.912069873042633,
		12.512566601747587  , 6.871078916124395,
		4.0673839494071125  , 1.0629002338507028, 
		0.32967595468603694 , 0.10405194784385363,
		0.032897776688262735, 0.010402990024382313,
		0.0032897079569814474};

	double ln_ks [ratio_size]    = {-1.3,-1,-0.6,-0.3,0,1,2,3,4,5,6};
	
	int iterations = 0;
	int max_iterations = 50;

	double ln_r = log(r); // natural log of ratio

	// check whether ratio in valid range
	if (ln_r >= ln_ratios[0]) {
		k = exp(ln_ks[0]);
		return 1;
	}

	if (ln_r <= ln_ratios[ratio_size - 1]) {
		k = exp(ln_ks[ratio_size - 1]);
		return 1;
	}

	int index = 1;

	while (ln_r < ln_ratios[index] && index < ratio_size - 1) index++;

	double lo_k = ln_ks[index - 1];
	double lo_r = ln_ratios[index - 1];

	double hi_k = ln_ks[index + 1]; // select wide bounds...
	double hi_r = ln_ratios[index + 1];

	double md_k = ln_ks[index];
	double md_r = ln_ratios[index];

	while (lo_r / hi_r > 1.000001 && iterations < max_iterations) {

		if (ln_r < md_r) {
			lo_k = md_k;
			lo_r = md_r;
		} else {
			hi_k = md_k;
			hi_r = md_r;
		}

		md_k = (lo_k + hi_k) / 2;

		k = pow(10,md_k);

		md_r = log(
			DCDFLIBInterface::computeGammaQuantile(k, 1, 0.95) /
			DCDFLIBInterface::computeGammaQuantile(k, 1, 0.05)
		);

		iterations++;
	}

	return iterations < max_iterations ? 0 : 1;
}

/**
 * Computes the shape of the Gamma distribution, given that the ratio between
 * lo and hi, with given cdf values, equals r.
 */
int Gamma0595InputMode::computeShape(double r, double locdf, double hicdf, double &k) const
{
	const double min_k = 0.1;
	const double max_k = 500;

	if (locdf == 0.05 && hicdf == 0.95) 
		return computeShape(r,k);

	double lo,hi;

	int iterations = 0;
	int max_iterations = 100;

	// check whether ratio is too high -> k < min_k
	lo = DCDFLIBInterface::computeGammaQuantile(min_k, 1, locdf);
	hi = DCDFLIBInterface::computeGammaQuantile(min_k, 1, hicdf);
	double lo_r = hi / lo;
	if (r > lo_r) {
		k = min_k;
		return 1;
	}

	// check whether ratio is too low -> k > max_k
	lo = DCDFLIBInterface::computeGammaQuantile(max_k, 1, locdf);
	hi = DCDFLIBInterface::computeGammaQuantile(max_k, 1, hicdf);
	double hi_r = hi/lo;
	if (r < hi_r) {
		k = max_k;
		return 1;
	}

	double hi_k = max_k;
	double lo_k = min_k;

	double md_k = pow(max_k * min_k,0.5);
	double md_r = 
			DCDFLIBInterface::computeGammaQuantile(md_k, 1, hicdf) /
			DCDFLIBInterface::computeGammaQuantile(md_k, 1, locdf);

	while (lo_r / hi_r > 1.00001 && iterations < max_iterations) {

		if (md_r > r) {
			lo_k = md_k;
			lo_r = md_r;
		} else {
			hi_k = md_k;
			hi_r = md_r;
		}

		if (hi_k / lo_k > 5) {
			md_k = pow(lo_k * hi_k,0.5);
		} else {
			md_k = (lo_k + hi_k) / 2;
		}
		md_r = (
			DCDFLIBInterface::computeGammaQuantile(md_k, 1, hicdf) /
			DCDFLIBInterface::computeGammaQuantile(md_k, 1, locdf)
		);

		iterations++;
	}

	k = md_k;
	return iterations < max_iterations ? 0 : 1;
}
