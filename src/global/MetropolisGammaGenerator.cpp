// MetropolisGammaGenerator.cpp: implementation of the MetropolisGammaGenerator class.
//
//////////////////////////////////////////////////////////////////////


#include "MetropolisGammaGenerator.h"
#include "../ran/RANLIBInterface.h"
#include "../dcdf/DCDFLIBInterface.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define SMALL_VALUE				1e-20

MetropolisGammaGenerator::MetropolisGammaGenerator()
{
	setMinimumVariation();
}

MetropolisGammaGenerator::~MetropolisGammaGenerator()
{

}

Parameters MetropolisGammaGenerator::generateCandidate(const Parameters &sample)
{
	double scale = 0;
	double value = 0;

	int dimension = sample.getDimension();
	Parameters candidate(dimension);

	for (int i = 0; i < dimension; i++) {
		// sample from a gamma distribution
		// choose prior sample value to be median

		scale = computeScale(i,sample.getValue(i));

		value = 0;
		while (value < SMALL_VALUE) {
			value = scale * RANLIBInterface::generateGammaDeviate(1,_densityShape);
		}

		candidate.setValue(i,value);
	}

	return candidate;
}

double MetropolisGammaGenerator::computeLogDensity(const Parameters &sample, const Parameters &candidate)
{
	double density = 0.0;
	double scale;
	double y;
	double value;

	int dimension = sample.getDimension();

	for (int i = 0; i < dimension; i++) {

		// scale parameter for a gamma distribution with median x
		value = sample.getValue(i);
		scale = computeScale(i,value);

		y = candidate.getValue(i);
		density += -_densityShape * log(scale) /* - _lnDensityGamma */ + 
			(_densityShape - 1) * log(y) - (y / scale);

	}

	return density;
}

double MetropolisGammaGenerator::computeScale(int index, double sample)
{
	return  sample / _densityMedian;
}

void MetropolisGammaGenerator::init(int dimension)
{
	setDensityMode(0);
}

bool MetropolisGammaGenerator::increaseVariation()
{
	return setDensityMode(_densityMode + 1);
}

bool MetropolisGammaGenerator::decreaseVariation()
{
	return setDensityMode(_densityMode - 1);
}

void MetropolisGammaGenerator::setMinimumVariation()
{
	setDensityMode(0);
}

void MetropolisGammaGenerator::setMaximumVariation()
{
	setDensityMode(12);
}

bool MetropolisGammaGenerator::setDensityMode(int mode)
{
	double shapes[] = {200,100,50,20,12,10,7,5,3,1.75,1,0.8,0.5};
//	double medians[] = {199.667,99.667,49.667,19.6677,
//		11.6684,9.66871,6.66964,4.67091,3.67206,2.67406,1.67835};

	if (mode >= 0 && mode < 13) {
		_densityShape = shapes[mode];
		_lnDensityGamma = DCDFLIBInterface::computeLnGamma(_densityShape);
		_densityMedian = DCDFLIBInterface::computeGammaQuantile(_densityShape, 1, 0.5); // medians[mode];
		_densityMode = mode;
		return true;
	} else {
		return false;
	}
}
