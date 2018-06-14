// MetropolisNormalGenerator.cpp: implementation of the MetropolisNormalGenerator class.
//
//////////////////////////////////////////////////////////////////////


#include "MetropolisNormalGenerator.h"
#include "../ran/RANLIBInterface.h"
#include "../dcdf/DCDFLIBInterface.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MetropolisNormalGenerator::MetropolisNormalGenerator()
{
	setMinimumVariation();
}

MetropolisNormalGenerator::~MetropolisNormalGenerator()
{

}

Parameters MetropolisNormalGenerator::generateCandidate(const Parameters &sample)
{
	int dimension = sample.getDimension();
	Parameters candidate(dimension);

	for (int i = 0; i < dimension; i++) {

		double nu = sample.getValue(i);
		double value = 0;

		value = RANLIBInterface::generateNormalDeviate(nu,_densityTau);

		candidate.setValue(i,value);
	}

	return candidate;
}

double MetropolisNormalGenerator::computeLogDensity(const Parameters &sample, const Parameters &candidate)
{
	double density = 0.0;
	double tau = _densityTau;

	int dimension = sample.getDimension();

	for (int i = 0; i < dimension; i++) {
		double nu = sample.getValue(i);
		double y = candidate.getValue(i);
		double z = (y - nu) / tau;
		density += (-z * z / 2) + log(0.3989422804 / tau);
	}

	return density;
}

void MetropolisNormalGenerator::init(int dimension)
{
	setDensityMode(0);
}

bool MetropolisNormalGenerator::increaseVariation()
{
	return setDensityMode(_densityMode + 1);
}

bool MetropolisNormalGenerator::decreaseVariation()
{
	return setDensityMode(_densityMode - 1);
}

void MetropolisNormalGenerator::setMinimumVariation()
{
	setDensityMode(0);
}

void MetropolisNormalGenerator::setMaximumVariation()
{
	setDensityMode(12);
}

bool MetropolisNormalGenerator::setDensityMode(int mode)
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