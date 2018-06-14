// MetropolisLNGenerator.cpp: implementation of the MetropolisLNGenerator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MetropolisLNGenerator.h"
#include "RANLIBInterface.h"
#include "DCDFLIBInterface.h"
#include <math.h>

#define SMALL_VALUE 1e-30

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MetropolisLNGenerator::MetropolisLNGenerator()
{
	setMinimumVariation();
}

MetropolisLNGenerator::~MetropolisLNGenerator()
{

}

Parameters MetropolisLNGenerator::generateCandidate(const Parameters &sample)
{
	int dimension = sample.getDimension();
	Parameters candidate(dimension);

	for (int i = 0; i < dimension; i++) {

		double nu = log(sample.getValue(i));
		double value = 0;

		while (value < SMALL_VALUE) {
			value = exp(RANLIBInterface::generateNormalDeviate(nu,_densityTau));
		}

		candidate.setValue(i,value);
	}

	return candidate;
}

double MetropolisLNGenerator::computeLogDensity(const Parameters &sample, const Parameters &candidate)
{
	double density = 0.0;
	double tau = _densityTau;

	int dimension = sample.getDimension();

	for (int i = 0; i < dimension; i++) {
		double nu = log(sample.getValue(i));
		double y = candidate.getValue(i);
		double z = (log(y) - nu) / tau;
		density += (-z * z / 2) + log(0.3989422804 / (tau * y));
	}

	return density;
}

void MetropolisLNGenerator::init(int dimension)
{
	setDensityMode(0);
}

bool MetropolisLNGenerator::increaseVariation()
{
	return setDensityMode(_densityMode + 1);
}

bool MetropolisLNGenerator::decreaseVariation()
{
	return setDensityMode(_densityMode - 1);
}

void MetropolisLNGenerator::setMinimumVariation()
{
	setDensityMode(0);
}

void MetropolisLNGenerator::setMaximumVariation()
{
	setDensityMode(12);
}

bool MetropolisLNGenerator::setDensityMode(int mode)
{
	static double taus[] = {0.1, 0.2, 0.3, 0.5, 0.6, 0.8, 1, 1.5, 2.2, 3, 4, 5, 6};

	if (mode >= 0 && mode < 13) {
		_densityTau = taus[mode];
		_densityMode = mode;
		return true;
	} else {
		return false;
	}
}