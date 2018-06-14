//
// File:      $Id: NHBetaSampleGenerator.cpp 2117 2013-01-21 05:20:35Z Dongfeng Zhu $
// Revision:  $Revision: 2117 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//


#include "stdafx.h"
#include "NHBetaSampleGenerator.h"
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

double NHBetaSampleGenerator::ab_var_modes [] = {0.08,0.12,0.15,0.25,0.5,0.7,1.0,1.4};
double NHBetaSampleGenerator::b_var_modes  [] = {0.15,0.2,0.3,0.6,0.8,1.2,1.6,2.5};

NHBetaSampleGenerator::NHBetaSampleGenerator()
{
	ab_var_mode = 0;
	b_var_mode = 0;
}

NHBetaSampleGenerator::~NHBetaSampleGenerator()
{

}

/**
 * Increases the variability used to generate candidates to minimum.
 * The variability determines how close around previous point
 * candidates are generated.
 */
bool NHBetaSampleGenerator::increaseVariation() {
	if (ab_var_mode == 7) return false;
	ab_var_mode ++;
	b_var_mode ++;
	return true;
}

/**
 * Decreases the variability used to generate candidates by one step.
 * The variability determines how close around previous point
 * candidates are generated.
 */
bool NHBetaSampleGenerator::decreaseVariation() {
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
void NHBetaSampleGenerator::setMinimumVariation() {
	ab_var_mode = 0;
	b_var_mode = 0;
}

/**
 * Increases the variability used to generate candidates to maximum.
 * The variability determines how close around previous point
 * candidates are generated.
 */
void NHBetaSampleGenerator::setMaximumVariation() {
	ab_var_mode = 7;
	b_var_mode = 7;
}

/**
 * Does nothing.
 */
void NHBetaSampleGenerator::init(int dimension) {
// DO NOTHING
}

/**
 * Computes transition density in alpha/beta space.
 * See class overview for details about sample density.
 */
double NHBetaSampleGenerator::computeLogDensity(const Parameters & from, const Parameters & to) {

	// original sample
	double x0 = from.getValue(0) + from.getValue(1);
	double y0 = from.getValue(0) / from.getValue(1);

	// candidate sample

	double a1 = to.getValue(0);
	double b1 = to.getValue(1);

	double x1 = a1 + b1;
	double y1 = a1 / b1;

	// compute density going from (x0,y0) to (x1,y1)
	double densityx = LogNormalDistribution(log(x0),b_var_modes[b_var_mode]).getLogPDF(x1);
	double densityy = LogNormalDistribution(log(y0),ab_var_modes[ab_var_mode]).getLogPDF(y1);

	// compute ln(jacobian) to account for parameter transformation
	double jacobian = log((a1 + b1) / (b1 * b1));

	// transition density(a,b) = density(x,y) * det(dxdy/dadb)
	return densityx + densityy + jacobian;
}

/**
 * Generates a new candidate sample point.
 * See class overview for details about sample density.
 */
Parameters NHBetaSampleGenerator::generateCandidate(const Parameters & sample) {

	double x0 = sample.getValue(0) + sample.getValue(1);
	double y0 = sample.getValue(0) / sample.getValue(1);

	double x1 = LogNormalDistribution(log(x0),b_var_modes[b_var_mode]).getSample();
	double y1 = LogNormalDistribution(log(y0),ab_var_modes[ab_var_mode]).getSample();

	double a1 = (x1 * y1) / (1 + y1);
	double b1 = x1 / (1 + y1);

	return Parameters(a1,b1);
}

///////////////////////////////////////////////////////////////////////

void NHBetaSampleGenerator::test()
{
#ifdef _DEBUG

	NHBetaSampleGenerator gen;
	gen.setMinimumVariation();
	gen.increaseVariation();
	Parameters p(70000,80000000);

	gen.computeLogDensity(p,Parameters(0.2,40));	

	double amax = 0;
	double amin = 1000000;
	double bmax = 0;
	double bmin = 1000000;
	
	int i;
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

	int j;
	for (j = 0 ; j < 200 ; j++)
		for (int k = 0 ; k < 200 ; k++)
			matrix[j][k] = 0;

//	amin = 4;
	amax = 1e5;
//	bmin = 1000;
	bmax = 1e8;

	int  l;
	for (l = 0 ; l < 1000000 ; l++) {
		Parameters c = gen.generateCandidate(p);
		double a = c.getValue(0);
		double b = c.getValue(1);

		int ai = (int)(40*(a-amin)/(amax-amin));
		int bi = (int)(40*(b-bmin)/(bmax-bmin));

		if (ai >= 0 && ai < 40 && bi >= 0 && bi < 40)
			matrix[ai][bi]++;
	}


//	cout << amin << " " << amax << " " << bmin << " " << bmax << endl;

	for (i = 0 ; i < 40 ; i++) {
		for (j = 0 ; j < 40 ; j++) {
//			cout << matrix[i][j] << " ";
		}
//		cout << endl;
	}

//	cout << endl;


	for (i = 0 ; i <= 40 ; i++) {
		double a = (amin + i * (amax-amin) / 40);
//		cout << a << " " ; 
		for (j = 0 ; j <= 40 ; j++) {
			double b = (bmin + j * (bmax-bmin) / 40);
//			cout << exp(gen.computeLogDensity(p,Parameters(a,b)) )<< " ";
		}
//		cout << endl;
	}

#endif
}
