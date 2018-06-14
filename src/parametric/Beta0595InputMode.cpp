// Beta0595InputMode.cpp: implementation of the Beta0595InputMode class.
//
//////////////////////////////////////////////////////////////////////

#include "Beta0595InputMode.h"
#include <math.h>
#include "../dcdf/DCDFLIBInterface.h"

#include <iostream>
using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Beta0595InputMode::Beta0595InputMode()
{
	lower_z = 0.05;
	upper_z = 0.95;
}

/**
 * Constructor.
 * Initializes the input mode to fit to zlo and zhi percentiles.
 * zlo is expected to be between 0.01 and 0.2. zhi is expected 
 * to be between 0.8 and 0.99. Under these conditions, the double-log
 * of the ratio of upper and lower percentiles varies approximately 
 * linearly with the log of alpha.
 *
 */
Beta0595InputMode::Beta0595InputMode(double zlo, double zhi)
{
	lower_z = zlo;
	upper_z = zhi;
}

Beta0595InputMode::~Beta0595InputMode()
{
	
}


/**
 * Computes alpha and beta parameters corresponding to percentile values.
 * The algorithm iteratively searches for the solution along alpha and beta
 * axes. Movement in alpha direction is controlled by p95/p05 ratio. Movement
 * in beta direction is controlled by p05 value.
 * The algorithm determines the alpha and beta using a search in r-phi space,
 * where alpha = r sin(phi) and beta = r cos(phi). The solutions of the two
 * constraints p05 = i1 and p95 = i2 are two curves, which intersect at the
 * solution of both constraints. A property of this space is that if, for a 
 * given value of r, the phi corresponding to the p05 constraint is higher 
 * than the phi corresponding to the p95 constraint, the value of r should be 
 * increased. 
 * Returns 0 if search was successful.
 * @param i1  percentile value corresponding to z_lower
 * @param i2  percentile value corresponding to z_upper
 * @param o1  alpha parameter
 * @param o2  beta parameter
 */
int Beta0595InputMode::convert(double i1, double i2, double & o1, double & o2) {

	int retval = 0;

	if (i2 > 0.9999999999 || i2 / i1 < 1.00001) {

		o1 = 1;
		o2 = 1;
		retval = 1;

	} else if (i1 > 0.5) {

		double swap_z = lower_z;
		lower_z = upper_z;
		upper_z = swap_z;
		retval = convert(1-i2,1-i1,o2,o1);
		upper_z = lower_z;
		lower_z = swap_z;

	} else {

		double epsilon = 1;				 // error measure
		double epsilon_max = 0.00001;  // maximum error measure for valid result

		double r_est, phi_est;          // estimated point
		double phi_upper, phi_lower;	// 

		double r_min = 0;				// r value lower bound
		double r_max = 0;				// r value upper bound

		int iteration = 0;				// number of iterations
		int iteration_max = 50;			// max allowed number of iterations

		// initialize previous and current estimates.
		double mn = (i1 + i2) / 2;
		double sd = (i2 - i1) / 6;
		double mn2 = mn * mn;
		double mn3 = mn2 * mn;
		double sd2 = sd * sd;
		double alpha = (mn2 - mn3 - sd2 * mn) / sd2;
		double beta = (mn - 2 * mn2 + mn3 - sd2 + mn * sd2) / sd2;

		// initial estimate of r
		r_est = sqrt(alpha * alpha + beta * beta);

		// find initial phi values for upper and lower percentile
		phi_upper = intersect_phi(r_est,upper_z,i2,0.001,0.1);
		phi_lower = intersect_phi(r_est,lower_z,i1,0.001,0.1);


		while (epsilon > epsilon_max && iteration < iteration_max) {

			// determine in which direction to move
			bool increase_r = phi_lower > phi_upper;

			// update bounds on r
			if (increase_r) {
				r_min = r_est;
			} else {
				r_max = r_est;
			}

			// determine next value for r
			if (r_max > 0) {
				r_est = (r_min + r_max) / 2;
			} else {
				r_est = r_min * 2;
			}

			// determine values for phi
			phi_upper = intersect_phi(r_est,upper_z,i2,phi_lower,phi_upper);
			phi_lower = intersect_phi(r_est,lower_z,i1,phi_lower,phi_upper);
			phi_est = (phi_upper + phi_lower) / 2;


			if (r_min == 0 || r_max == 0) {
				epsilon = 1;
			} else {
				epsilon = 
					fabs((r_max - r_min) / r_min) + 
					fabs((phi_upper - phi_lower) / phi_lower);
			}

			iteration++;
		}

		o1 = r_est * sin(phi_est);
		o2 = r_est * cos(phi_est);


		return iteration < iteration_max ? 0 : 1;
	}

	return retval;
}

/**
 * Returns the value of phi for which the z percentile of the Beta
 * distribution equals q, given the parameters of the distribution
 * alpha = r sin(phi) and beta = r cos(phi).
 * 
 * Solution is found using a bracket search between 0 and Pi/2, where
 * phi is used as an initial estimate. The search finds the value of phi
 * for which the z percentile equals q, or in other words, for which 
 * CDF(q) = z.
 */
double Beta0595InputMode::intersect_phi(double r, double z, double q, double phi0, double phi1) {

	double phi_hi,phi_lo,phi_md,cdf_hi,cdf_lo,cdf_md,cdf0,cdf1;

	cdf1 = DCDFLIBInterface::computeBetaCDF(r*sin(phi1),r*cos(phi1),q);
	cdf0 = DCDFLIBInterface::computeBetaCDF(r*sin(phi0),r*cos(phi0),q);

	if (z < cdf1) {
		phi_hi = 1.5707963267948966;
		cdf_hi = 0;
		phi_lo = phi1;
		cdf_lo = cdf1;
	} else if (z < cdf0) {
		phi_hi = phi1;
		cdf_hi = cdf1;
		phi_lo = phi0;
		cdf_lo = cdf0;
	} else {
		phi_hi = phi0;
		cdf_hi = cdf0;
		phi_lo = 0;
		cdf_lo = 1;
	}

	// initialize bracket
//	double phi_hi = 1.5707963267948966;
//	double cdf_hi = 0; // alpha = r, beta = 0 -> CDF(q) = 0 for q < 1
//	double phi_lo = 0; 
//	double cdf_lo = 1; // alpha = 0, beta = r -> CDF(q) = 1 for q > 0

	phi_md = (phi_hi + phi_lo) / 2;
	cdf_md = DCDFLIBInterface::computeBetaCDF(r*sin(phi_md),r*cos(phi_md),q);

	int iteration = 0;

	while ((cdf_hi == 0 || cdf_lo / cdf_hi - 1 > 0.000001) && iteration < 50) {

		bool increase_phi = cdf_md > z; // cdf too high -> increase phi -> shift beta right

		if (increase_phi) {
			phi_lo = phi_md;
			cdf_lo = cdf_md;
		} else {
			phi_hi = phi_md;
			cdf_hi = cdf_md;
		}

		phi_md = (phi_hi + phi_lo) / 2;
		cdf_md = DCDFLIBInterface::computeBetaCDF(r*sin(phi_md),r*cos(phi_md),q);

		iteration++;
	}

	return phi_md;
}
