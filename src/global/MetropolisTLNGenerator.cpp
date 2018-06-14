// MetropolisTLNGenerator.cpp: implementation of the MetropolisTLNGenerator class.
//
//////////////////////////////////////////////////////////////////////


#include "MetropolisTLNGenerator.h"
#include "../ran/RANLIBInterface.h"
#include "../dcdf/DCDFLIBInterface.h"
#include <math.h>

#define SMALL_VALUE 1e-30

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MetropolisTLNGenerator::MetropolisTLNGenerator()
{
	setMinimumVariation();
}

MetropolisTLNGenerator::~MetropolisTLNGenerator()
{

}

Parameters MetropolisTLNGenerator::generateCandidate(const Parameters &sample)
{
	int dimension = sample.getDimension();
	Parameters candidate(dimension);

	for (int i = 0; i < dimension; i++) {

		double nu = log(sample.getValue(i));

		// avoid generation of too many samples >= 1
		if (nu > -_densityTau) nu = -_densityTau;

		double value = 0;

		while (value < SMALL_VALUE || value >= 1) {
			value = exp(RANLIBInterface::generateNormalDeviate(nu,_densityTau));
		}

		candidate.setValue(i,value);
	}

	return candidate;
}

double MetropolisTLNGenerator::computeLogDensity(const Parameters &sample, const Parameters &candidate)
{
	double density = 0.0;
	double tau = _densityTau;

	int dimension = sample.getDimension();

	for (int i = 0; i < dimension; i++) {

		double nu = log(sample.getValue(i));

		// apply same correction as in generateCandidate()
		if (nu > -_densityTau) nu = -_densityTau;

		double y = candidate.getValue(i);
		double z = (log(y) - nu) / tau;
		density += (-z * z / 2) + log(0.3989422804 / (tau * y));

		// account for truncation through normalization if
		// (nu > - 5 tau) -> truncated probability > 2.87E-7
		if (nu > -5 * _densityTau) {
			density -= log((double)DCDFLIBInterface::computeNormalCDF(nu,tau,log(double(1))));
		}
	}

	return density;
}

void MetropolisTLNGenerator::init(int dimension)
{
	setDensityMode(0);
}

bool MetropolisTLNGenerator::increaseVariation()
{
	return setDensityMode(_densityMode + 1);
}

bool MetropolisTLNGenerator::decreaseVariation()
{
	return setDensityMode(_densityMode - 1);
}

void MetropolisTLNGenerator::setMinimumVariation()
{
	setDensityMode(0);
}

void MetropolisTLNGenerator::setMaximumVariation()
{
	setDensityMode(12);
}

bool MetropolisTLNGenerator::setDensityMode(int mode)
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