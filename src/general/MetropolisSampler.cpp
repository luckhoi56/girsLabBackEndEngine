// MetropolisSampler.cpp: implementation of the MetropolisSampler class.
//
//////////////////////////////////////////////////////////////////////

/****************************************************
 *
 *  Bayesian Weibull Sampler.
 *
 *	(c) 2000, Frank Groen. All Rights Reserved.
 *
 ****************************************************/

#include "stdafx.h"
#include "MetropolisSampler.h"
#include <math.h>
#include <stdlib.h>
#include "RANLIBInterface.h"

#include "HookeMinFinder.h"

#ifdef _DEBUG
#include <iostream>
#include <fstream>
#endif

#define TUNING_SAMPLES (100)
#define RANDOM_SEED (1134154)
#define MINIMUM_REJECTION_RATE (2)
#define STATUS_STEPS 100


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MetropolisSampler::MetropolisSampler(SampleSet & set)
{
	_sampleSize = 10000;
	_sampleSet = &set;
	_generator = &_defaultGenerator;
}

MetropolisSampler::~MetropolisSampler()
{
	if (_generator != 0L && _generator != &_defaultGenerator) {
		delete _generator;
	}
}

void MetropolisSampler::setGenerator(MetropolisGenerator *generator)
{
	if (_generator != 0L && _generator != &_defaultGenerator) {
		delete _generator;
	}

	if (generator != 0L) {
		_generator = generator;
	} else {
		_generator = &_defaultGenerator;
	}
}

void MetropolisSampler::setSampleSize(int size)
{
	_sampleSet->clear();
	_sampleSize = size;
}

int MetropolisSampler::getSampleSize()
{
	return _sampleSet->size();
}

int MetropolisSampler::sample(Function &function)
{
	// sample without status
	Parameters start(function.getDimension());
	for (int i = 0 ; i < function.getDimension() ; i++) 
		start.setValue(i,0.5);

	return this->sample(function,start,0L);
}

int MetropolisSampler::sample(Function & function, Parameters & start)
{
	return this->sample(function,start,0);
}

int MetropolisSampler::sample(Function &function, StatusPtr status)
{
	Parameters start(function.getDimension());
	for (int i = 0 ; i < function.getDimension() ; i++) 
		start.setValue(i,0.5);
	return this->sample(function,start,status);
}


//#pragma optimize("", off)

//#pragma intrinsic(_disable)

int MetropolisSampler::sample(Function &function, Parameters & start, StatusPtr status)
{
	/* Implementation of Metropolis sampler described in
	 * Chib and Greenberg, 'Understanding the Metropolis-Hastings Algorithm'
	 * The American Statistician v49 n4 p327 (1995)
	 *
	 * The function that is passed in is supposed to return
	 * the natural log of a density function when evaluated.
	 */

//	_sampleSet->clear();
//	for (int i = 0 ; i < 40000 ; i++) {
//		double xx = exp((i % 200) / 100.);
//		double yy = exp((i / 200) / 50.);
//		double pp = log(xx * yy);
//		_sampleSet->addSample(Parameters(xx,yy),pp);
//	}
//	return STATUS_OK;

	if (status != 0L) {
		status->clear();
		status->setStatus(0,"Initializing...");
		status->setStatusCode(STATUS_ACTIVE);
	}

	Parameters x;		// accepted sample
	Parameters y;		// candidate sample
	double logPiX;			// likelihood(x)
	double logPiY;			// likelihood(y)
	int weight;			// weight of the sample
	double lnqyx;			// transition density y -> x
	double lnqxy;			// transition density x -> y
	double logAlphaxy;	// natural log of probability of transition
	bool transition;	// transition flag
	double uniform;		// uniform sample value
	bool canceled = false;
	int result = STATUS_OK;

	div_t  progress;

//	if (status != 0L) status->setStatus(0,"Initializing...");

	int count = -TUNING_SAMPLES;
	_rejected = 0;
	int dimension = function.getDimension();

	_sampleSet->clear();

	x = initCandidateGenerator(dimension,function,_generator,start,status);

	function.setLogLikeMode(&x);

#ifdef _DEBUG
	cout << "STARTING POINT ";
	x.dump();

	ofstream out("error.log");

#endif	
	
	logPiX = function.evaluate(x);
	weight = 1;

	_generator->setMinimumVariation();

	if (status != 0L) {
		status->setStatus(0,"Checking sampler settings...");
		canceled = status->isCancelRequested();
	}

	// loop until sufficient number of samples
	while (count <= _sampleSize && !canceled) {
		try {
			// generate candidate
			y = generateCandidate(x);

			logPiY = function.evaluate(y);

			lnqyx = computeLogDensity(y,x);
			lnqxy = computeLogDensity(x,y);

			// compute probability of transition (move)
			logAlphaxy = logPiY - logPiX + lnqyx - lnqxy;

			transition = (logAlphaxy >= 0);

			if (!transition) {
				// uncertain transition
				uniform = generateUniformSample();
				transition = (log(uniform) <= logAlphaxy);
			}
		} catch (int errCode) {
			transition = false;

#ifdef _DEBUG
			cout << "EXCEPTION THROWN DURING SAMPLING: " << errCode << endl;
			cout << "x = " << x.getValue(0) << " : " << x.getValue(1) << endl;
			cout << "y = " << y.getValue(0) << " : " << y.getValue(1) << endl;
			cout << "LogPiX = " << logPiX << endl;
			cout << "LogPiY = " << logPiY << endl;
			cout << "lnqyx = " << lnqyx << endl;
			cout << "lnqxy = " << lnqxy << endl;
			cout << "logAlphaxy = " << logAlphaxy << endl;
			cout << "uniform = " << uniform << endl << endl;

			out << "EXCEPTION THROWN DURING SAMPLING: " << errCode << endl;
			out << "x = " << x.getValue(0) << " : " << x.getValue(1) << endl;
			out << "y = " << y.getValue(0) << " : " << y.getValue(1) << endl;
			out << "LogPiX = " << logPiX << endl;
			out << "LogPiY = " << logPiY << endl;
			out << "lnqyx = " << lnqyx << endl;
			out << "lnqxy = " << lnqxy << endl;
			out << "logAlphaxy = " << logAlphaxy << endl;
			out << "uniform = " << uniform << endl << endl;
#endif
		}

		if (transition) {

#ifdef _DEBUG
			x.dump();
			out << x.getValue(0) << "\t" << x.getValue(1) << endl;
#endif	


			if (count > 0) {
				_sampleSet->addSample(x,log((double)weight));
			}

			count++;

			if (count == 0) {
				if (1. * _rejected / TUNING_SAMPLES < MINIMUM_REJECTION_RATE &&
					_generator->increaseVariation()) {
					if (status != 0L) status->setStatus(0,"Adjusting sampler settings...");
					_rejected = 0;
					count = -TUNING_SAMPLES;
#ifdef _DEBUG
					cout << "increasing variation" << endl;
#endif
				} else {
					if (status != 0L) status->setStatus(0,"Sampling...");
				}
			}

			// make transition
			x = y;
			logPiX = logPiY;
			weight = 1;

			if (status != 0L && count > 0) {
				progress = div(count , _sampleSize / STATUS_STEPS);
				if (progress.rem == 0) {
					status->setProgress(progress.quot * 100 / STATUS_STEPS);
				}
			}
		} else {
			// increase weight of sample
			weight++;
			_rejected++;
		}

		if (status != 0) canceled = status->isCancelRequested();
	}

	if (canceled) {
		if (status != 0L) {
			status->setStatus(0,"Sampling was canceled.");
			status->setStatusCode(STATUS_CANCELED);
		}
		_sampleSet->clear();
		result = STATUS_CANCELED;
	} else {
		if (status != 0L) {
			status->setStatus(0,"Done.");
			status->setStatusCode(STATUS_IDLE);
		}
	}


#ifdef _DEBUG
	out.close();
#endif	


	return result;
}

//#pragma intrinsic(_enable) 

//#pragma optimize("", on)


Parameters MetropolisSampler::initCandidateGenerator(int dimension, Function & function,
	MetropolisGenerator * generator, Parameters & start, StatusPtr status)
{

	// method name is a mismatch. method actually attempts
	// to approximately locate the (a) mode of the distribution

	bool done = false;
	double sLogPi = 0;
	double cLogPi = 0;
	int attempts = 100;
//	int i;

	if (status != 0L) status->setStatus(0,"Finding starting position...");

	generator->init(dimension);

	Parameters candidate(dimension);

	HookeMinFinder finder;

	Parameters sample = start;
	finder.hooke(function,sample,true,true);

//	Parameters sample(dimension);
//	for (i = 0; i < dimension; i++) {
//		sample.setValue(i,0.5);
//	}

//	sLogPi = function.evaluate(sample);

//	generator->setMaximumVariation();

	// approximately locate mode
//	while (!done) {
//		for (i = 0 ; i < attempts ; i++) {
//			candidate = generateCandidate(sample);
//			cLogPi = function.evaluate(candidate);
//			if (cLogPi > sLogPi) {
//				sample = candidate;
//				sLogPi = cLogPi;
//			}
//		}
//		done = !generator->decreaseVariation();
//	}

	return sample;
}

double MetropolisSampler::generateUniformSample()
{
	double uniform = 0;
	while (uniform == 0 || uniform == 1) 
		uniform = RANLIBInterface::generateUniformDeviate();
	return uniform;
}

double MetropolisSampler::getRejectionRate() const
{
	return (_rejected * 1.0) / _sampleSize;
}

Parameters MetropolisSampler::generateCandidate(const Parameters &sample)
{
	return _generator->generateCandidate(sample);
}

double MetropolisSampler::computeLogDensity(const Parameters &sample, const Parameters &candidate)
{
	return _generator->computeLogDensity(sample,candidate);
}


