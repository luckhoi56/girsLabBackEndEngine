// SampleSet.cpp: implementation of the SampleSet class.
//
//////////////////////////////////////////////////////////////////////

/****************************************************
 *
 *  Bayesian Weibull Sampler.
 *
 *	(c) 2000, Frank Groen. All Rights Reserved.
 *
 ****************************************************/


#include "SampleSet.h"
#include <math.h>

#ifdef _DEBUG
#include <iostream>
#include <stdlib.h>
#include <time.h>
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SampleSet::SampleSet()
{
	_normalized = false;
}

SampleSet::~SampleSet()
{
	_samples.clear();
}

void SampleSet::addSample(const Parameters & params, double logWeight)
{
	_samples.push_back(Sample(params,logWeight));
	_normalized = false;
}

Parameters SampleSet::getParameters() const
{
	return Parameters((*_cursor).getParameters());
}

double SampleSet::getLogWeight() const
{
	return (*_cursor).getLogWeight();
}

double SampleSet::getProbability() 
{
	double probability = 0.0;
	if (!endOfSamples()) {
		if (!_normalized) 
			normalize();
		probability = exp(getLogWeight() -  _normalizer);
	}
	return probability;
}

void SampleSet::normalize()
{
	int index = 0;
	SampleList::iterator iter;
	int size = _samples.size();
	double maxLogWeight = 0;

	if (size > 0) {
		iter = _samples.begin();
		maxLogWeight = (*iter).getLogWeight();
		for (index = 0; index < size; index++) {
			if ((*iter).getLogWeight() > maxLogWeight)
				maxLogWeight = (*iter).getLogWeight();
			iter++;
		}

		iter = _samples.begin();
		double sumOfProbabilities = 0;
		for (index = 0; index < size; index++) {
			sumOfProbabilities += exp((*iter).getLogWeight() - maxLogWeight);
			iter++;
		}

		_normalizer = maxLogWeight + log(sumOfProbabilities);
		_normalized = true;
	}
}

void SampleSet::next()
{
	_cursor++;
}

void SampleSet::moveBy(int index)
{
	advance(_cursor,index);
}

void SampleSet::first()
{
	_cursor = _samples.begin();
}

bool SampleSet::endOfSamples() 
{
	return (_cursor == _samples.end());
}

void SampleSet::clear()
{
	_samples.clear();
}

int SampleSet::size() const
{
	return _samples.size();
}

Parameters SampleSet::getMean()
{
	SampleList::iterator iter;
	int size =0;
	double probability = 0;
	double value = 0;
	int dimension = 0;
	Parameters values;

	size = _samples.size();

	if (size > 0) {
		iter = _samples.begin();
		dimension = (*iter).getParameters().getDimension();
	}

	Parameters mean(dimension);

	if (!_normalized) normalize();

	for (int i = 0; i < size; i++) {

		values = (*iter).getParameters();
		probability = exp((*iter).getLogWeight() -  _normalizer);

		values = values * probability;
		mean = mean + values;
		iter++;
	}

	return mean;
}

Parameters SampleSet::getVariance()
{
	SampleList::iterator iter;
	int size =0;
	double probability = 0;
	double cumulative = 0;
	double value = 0;
	int dimension = 0;
	Parameters values;

	size = _samples.size();

	if (size > 0) {
		iter = _samples.begin();
		dimension = (*iter).getParameters().getDimension();
	}

	Parameters variance(dimension);
	Parameters mean(dimension);

	if (!_normalized) normalize();

	for (int i = 0; i < size; i++) {
		values = (*iter).getParameters();
		probability = exp((*iter).getLogWeight() -  _normalizer);
		cumulative += probability;
		variance = variance + (values.getSquared() * probability);
		iter++;
	}

	mean = getMean();
	variance = variance - mean.getSquared();
	return variance;
}

void SampleSet::getExtremes(int index, double &minimum, double &maximum)
{
	SampleList::iterator iter;
	Parameters params;

	int size = _samples.size();
	double value;

	minimum = 0.0;
	maximum = 0.0;

	if (size > 0) {
		iter = _samples.begin();

		params = (*iter).getParameters();
		value = params.getValue(index);
		minimum = value;
		maximum = value;

		for (int i = 0; i < size; i++) {

			value = (*iter).getParameters().getValue(index);
			if (value < minimum) minimum = value;
			if (value > maximum) maximum = value;

			iter++;
		}
	}
}

Parameters SampleSet::getMinima()
{
	SampleList::iterator iter;

	double minimum;
	double maximum;
	int dimension = 0;
	int size = _samples.size();

	if (size > 0) {
		iter = _samples.begin();
		dimension = (*iter).getParameters().getDimension();
	}

	Parameters minima(dimension);

	for (int i = 0 ; i < dimension; i++) {
		getExtremes(i,minimum,maximum);
		minima.setValue(i,minimum);
	}

	return minima;
}

Parameters SampleSet::getMaxima() 
{
	SampleList::iterator iter;
	double minimum;
	double maximum;
	int dimension = 0;
	int size = _samples.size();

	if (size > 0) {
		iter = _samples.begin();
		dimension = (*iter).getParameters().getDimension();
	}

	Parameters maxima(dimension);

	for (int i = 0 ; i < dimension; i++) {
		getExtremes(i,minimum,maximum);
		maxima.setValue(i,maximum);
	}
	return maxima;
}

int SampleSet::getDimension() 
{
	SampleList::iterator iter;

	int dimension = 0;
	int size = _samples.size();

	if (size > 0) {
		iter = _samples.begin();
		dimension = (*iter).getParameters().getDimension();
	}

	return dimension;
}

void SampleSet::getMarginal(int index, Distribution &distribution)
{
	double value = 0;
	double prob = 0;

	int setSize = size();

	if (setSize > 0) {

		if (!_normalized) normalize();

		distribution.clear(setSize);

		SampleList::iterator iter = _samples.begin();
		
		for (int i = 0 ; i < setSize ; i++) {

			value = (*iter).getParameters().getValue(index);
			prob = exp((*iter).getLogWeight() -  _normalizer);

			if (prob > 0) {
				distribution.add(value,prob);
			}

			iter++;
		}

	} else {
		distribution.clear();
	}
}

void SampleSet::testUnit()
{
#ifdef _DEBUG

	SampleSet samples;
	int i;
	int cycle;
	double sum = 0;
    clock_t start, finish;
	double duration = 0.0;

	Parameters param(1,1);

	samples.addSample(param,0);
	samples.clear();

	for (i = 1; i < 10 ; i++) {
		samples.addSample(Parameters(i*10.,i*1.),log(i*1.));
	}

	Parameters minima = samples.getMinima();
	Parameters maxima = samples.getMaxima();

	cout << "MINIMA ";
	minima.dump();
	cout << "MAXIMA ";
	maxima.dump();

	samples.first();
	while(!samples.endOfSamples()) {
		double lw = samples.getLogWeight();
		double pr = samples.getProbability();
		cout << lw << " : " << pr << endl;
		sum += pr;
		samples.next();
	}
	cout << "TOTAL PROB: " << sum << endl;

	cout << "BEFORE CLEAR: " << Sample::_instances << " : " << Parameters::_instances << endl;
	samples.clear();
	cout << "AFTER CLEAR:  " << Sample::_instances << " : " << Parameters::_instances << endl;

	start = clock();
	for (cycle = 0; cycle < 10000; cycle++) {
		samples.addSample(Parameters(0,0),0);
	}

	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "10000 INSERTIONS : " << duration << " [sec]" << endl;

	sum = 0;
	samples.first();
	while(!samples.endOfSamples()) {
		double pr = samples.getProbability();
		sum += pr;
		samples.next();
	}
	cout << "TOTAL PROB: " << sum << endl;




#endif
}



