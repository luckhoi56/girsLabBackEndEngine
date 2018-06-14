// MetropolisBetaGenerator.cpp: implementation of the MetropolisBetaGenerator class.
//
//////////////////////////////////////////////////////////////////////


#include "MetropolisBetaGenerator.h"
#include "../ran/RANLIBInterface.h"
#include "../dcdf/DCDFLIBInterface.h"
#include <math.h>
#include "Error.h"

#define SMALL_VALUE 1e-25
#define ONE 1
#define MIN_POPULATION 3
#define MAX_POPULATION (3 * 3 * 3 * 3 * 3 * 3 * 3 * 3)
#define POPULATION_STEP 3

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MetropolisBetaGenerator::MetropolisBetaGenerator()
{
	setMinimumVariation();
}

Parameters MetropolisBetaGenerator::generateCandidate(const Parameters &sample)
{
	double scale = 0;
	double value = 0;
	double prev = 0;

	int dimension = sample.getDimension();
	Parameters candidate(dimension);

	for (int i = 0; i < dimension; i++) {

		prev = sample.getValue(i);

		if (prev <= 0.0|| prev >= 1.0) throw INVALID_ARGUMENT;

		double alpha = prev * _densityPopulation;
		double beta = _densityPopulation - alpha;

		if (beta == 0) throw INVALID_ARGUMENT;

		value = 0;

		while (value < SMALL_VALUE || value >= ONE) {
			value = RANLIBInterface::generateBetaDeviate(alpha,beta);
		}

		candidate.setValue(i,value);
	}

	return candidate;
}

double MetropolisBetaGenerator::computeLogDensity(const Parameters &sample, const Parameters &candidate)
{
	double value;
	double y;
	double density = 0.0;

	int dimension = sample.getDimension();

	for (int i = 0; i < dimension; i++) {

		// scale parameter for a gamma distribution with median x
		value = sample.getValue(i);

		double alpha = value * _densityPopulation;
		double beta = (1 - value) * _densityPopulation;

		y = candidate.getValue(i);

		double lny = log(y);

		double lnomy;
		if (lny < -25) {
			lnomy = -y;
		} else {
			lnomy = log(1 - y);
		}

		density += lny * (alpha - 1) + lnomy * (beta - 1) - DCDFLIBInterface::computeLnBeta(alpha,beta);
	}

	return density;
}

bool MetropolisBetaGenerator::increaseVariation()
{
	if (_densityPopulation >= MIN_POPULATION * POPULATION_STEP) {
		_densityPopulation /= POPULATION_STEP;
		return true;
	}
	return false;
}

bool MetropolisBetaGenerator::decreaseVariation()
{
	if (_densityPopulation <= MAX_POPULATION / POPULATION_STEP) {
		_densityPopulation *= POPULATION_STEP;
		return true;
	}
	return false;
}

void MetropolisBetaGenerator::setMinimumVariation()
{
	_densityPopulation = MAX_POPULATION;
}

void MetropolisBetaGenerator::setMaximumVariation()
{
	_densityPopulation = MIN_POPULATION;
}

void MetropolisBetaGenerator::init(int dimension)
{
	setMinimumVariation();
}