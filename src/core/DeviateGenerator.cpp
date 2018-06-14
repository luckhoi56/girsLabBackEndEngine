// $Id: DeviateGenerator.cpp 169 2005-01-28 14:17:28Z saulo $

#include "deviategenerator.h"

DeviateGenerator::DeviateGenerator(void)
{
}

DeviateGenerator::~DeviateGenerator(void)
{
}

double DeviateGenerator::uniform(void) const
{
	double value = (double)rand();
	while (value == 0) {
		value = (double)rand();
	}
	return value / RAND_MAX;
}

double DeviateGenerator::snormal(void) const
{
	double sum = 0;
	for (int i = 0 ; i < 24 ; i++) sum += this->uniform();
	sum -= 12;
	sum /= 1.41421;
	return sum;
}
