//
// File:      $Id: NHGammaSampleGenerator.cpp 2117 2013-01-21 05:20:35Z Dongfeng Zhu $
// Revision:  $Revision: 2117 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//
 

#include "stdafx.h"
#include "NHGammaSampleGenerator.h"
#include "RANLIBInterface.h"
#include <math.h>
#include "LogNormalDistribution.h"

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

double NHGammaSampleGenerator::ab_var_modes [] = {0.05,0.1,0.2,0.3,0.5,0.7,0.9,1.2};
double NHGammaSampleGenerator::b_var_modes  [] = {0.1,0.2,0.4,0.6,0.8,1.2,1.6,2.0};

// If TRUE, then the parameterization x = 1 / b, y=a/b is used (b -> lambda)
// If FALSE, then the parameterization x = b, y = a*b is used (b -> beta)
// This depends on the formulation used by the likelihood function.
const bool A_DIV_B_MEAN = true;

NHGammaSampleGenerator::NHGammaSampleGenerator()
{
	ab_var_mode = 0;
	b_var_mode = 0;
}

NHGammaSampleGenerator::~NHGammaSampleGenerator()
{

}

/**
 * Increases the variability used to generate candidates.
 * The variability determines how close around previous point
 * candidates are generated.
 */
bool NHGammaSampleGenerator::increaseVariation() {
	if (ab_var_mode == 7) return false;
	ab_var_mode ++;
	b_var_mode ++;
	return true;
}

/**
 * Decreases the variability used to generate candidates.
 * The variability determines how close around previous point
 * candidates are generated.
 */
bool NHGammaSampleGenerator::decreaseVariation() {
	if (ab_var_mode == 0) return false;
	ab_var_mode --;
	b_var_mode --;
	return true;
}

/**
 * Decreases the variability used to generate candidates to minimum.
 * The variability determines how close around previous point
 * candidates are generated.
 */
void NHGammaSampleGenerator::setMinimumVariation() {
	ab_var_mode = 0;
	b_var_mode = 0;
}

/**
 * Increases the variability used to generate candidates to maximum.
 * The variability determines how close around previous point
 * candidates are generated.
 */
void NHGammaSampleGenerator::setMaximumVariation() {
	ab_var_mode = 7;
	b_var_mode = 7;
}

/// Does nothing.
void NHGammaSampleGenerator::init(int dimension) {
// DO NOTHING
}

/**
 * Computes transition density in alpha/beta space.
 * See class overview for details about sample density.
 */
double NHGammaSampleGenerator::computeLogDensity(const Parameters & from, const Parameters & to) {

	// original sample
	double x0 = (A_DIV_B_MEAN) ? 1 / from.getValue(1) : from.getValue(1);
	double y0 = (A_DIV_B_MEAN) ? from.getValue(0) / from.getValue(1) : from.getValue(0) * from.getValue(1) ;

	// candidate sample
	double x1 = (A_DIV_B_MEAN) ? 1 / to.getValue(1) : to.getValue(1);
	double y1 = (A_DIV_B_MEAN) ? to.getValue(0) / to.getValue(1) : to.getValue(0) * to.getValue(1) ;

	// compute density going from (x0,y0) to (x1,y1)
	double densityx = LogNormalDistribution(log(x0),b_var_modes[b_var_mode]).getLogPDF(x1);
	double densityy = LogNormalDistribution(log(y0),ab_var_modes[ab_var_mode]).getLogPDF(y1);

	// compute ln(jacobian) to account for parameter transformation
	// x = b   -> jac = b = x
	// x = 1/b -> jac = b^(-3) = x^3
	double jacobian = (A_DIV_B_MEAN) ? 3 * log(x1) : log(x1);

	// transition density(a,b) = density(x,y) * det(dxdy/dadb)
	return densityx + densityy + jacobian;
}

/**
 * Generates a new candidate sample point.
 * See class overview for details about sample density.
 */
Parameters NHGammaSampleGenerator::generateCandidate(const Parameters & sample) {

	double x0 = (A_DIV_B_MEAN) ? 1 / sample.getValue(1) : sample.getValue(1);
	double y0 = (A_DIV_B_MEAN) ? sample.getValue(0) / sample.getValue(1) : sample.getValue(0) * sample.getValue(1) ;

	double x1 = LogNormalDistribution(log(x0),b_var_modes[b_var_mode]).getSample();
	double y1 = LogNormalDistribution(log(y0),ab_var_modes[ab_var_mode]).getSample();

	double a1 = y1 / x1;
	double b1 = (A_DIV_B_MEAN) ? 1 / x1 : x1;

	return Parameters(a1,b1);
}

/***************************************************************************************/

void NHGammaSampleGenerator::test()
{
#ifdef _DEBUG

	NHGammaSampleGenerator gen;
	gen.setMinimumVariation();
	gen.increaseVariation();
	Parameters p(2,1000);

	gen.computeLogDensity(p,Parameters(0.2,40));	

	double amax = 0;
	double amin = 1000000;
	double bmax = 0;
	double bmin = 1000000;

	int i, j, l;

	for (i = 0 ; i < 2000 ; i++) {
		Parameters c = gen.generateCandidate(p);
		double a = c.getValue(0);
		if (a > amax) amax = a;
		if (a < amin) amin = a;
		double b = c.getValue(1);
		if (b > bmax) bmax = b;
		if (b < bmin) bmin = b;
	}

	int matrix[200][200];

	for (j = 0 ; j < 200 ; j++)
		for (int k = 0 ; k < 200 ; k++)
			matrix[j][k] = 0;

	amin = 4;
	amax = 5;
	bmin = 1000;
	bmax = 5000;

	for (l = 0 ; l < 1000000 ; l++) {
		Parameters c = gen.generateCandidate(p);
		double a = c.getValue(0);
		double b = c.getValue(1);

		int ai = (int)(40*(a-amin)/(amax-amin));
		int bi = (int)(40*(b-bmin)/(bmax-bmin));

		if (ai >= 0 && ai < 40 && bi >= 0 && bi < 40)
			matrix[ai][bi]++;
	}

	for (i = 0 ; i < 40 ; i++) {
		for (j = 0 ; j < 40 ; j++) {
		}
	}

	for (i = 0 ; i <= 40 ; i++) {
		double a = (amin + i * (amax-amin) / 40);
		for (j = 0 ; j <= 40 ; j++) {
			double b = (bmin + j * (bmax-bmin) / 40);
		}
	}

#endif
}
