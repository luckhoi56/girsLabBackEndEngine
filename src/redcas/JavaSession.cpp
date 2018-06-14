// $Header: C:/cvsrepo/ptdev/native/redcas/redcas/JavaSession.cpp,v 1.4 2001/09/04 20:44:35 fgroen Exp $

// JavaSession.cpp: implementation of the JavaSession class.
//
//////////////////////////////////////////////////////////////////////

/****************************************************
 *
 *  Bayesian Weibull Sampler.
 *
 *	(c) 2000, Frank Groen. All Rights Reserved.
 *
 ****************************************************/


#include "JavaSession.h"

#include "WeibullFunction.h"
#include "MetropolisSampler.h"
#include "BinnedSampleSpace.h"
// #include "MetropolisLNGenerator.h"
#include "MHWeibullGenerator.h"
#include "BinnedSpace.h"
#include "HyperIndex.h"
#include <math.h>

#ifdef _DEBUG
#include <iostream>
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

JavaSession::JavaSession()
{
	_failures = new WeibullInterpretation(CERTAIN_FAILURE);
	_successes = new WeibullInterpretation(CERTAIN_SUCCESS);
	_bins = new WeibullInterpretation(BINNED_OBSERVATION);
	_joint = new InterpretationSet();
	_joint->addInterpretation(_failures);
	_joint->addInterpretation(_successes);
	_joint->addInterpretation(_bins);
	if (_status != 0L) _status->setStatusCode(STATUS_IDLE);

	_bPriorSettingRequired = false;
}

JavaSession::~JavaSession()
{
	delete _joint;
}

void JavaSession::addDataPoint(double start, double end, bool failure)
{
	if (failure) {
		_failures->addObservation(start,end);
	} else {
		_successes->addObservation(start,end);
	}
	if (_status != 0L) _status->setStatusCode(STATUS_IDLE);
}

void JavaSession::addDataPoint(double start, double end, int population, int failures)
{
	_bins->addObservation(start,end,population,failures);
	if (_status != 0L) _status->setStatusCode(STATUS_IDLE);
}

void JavaSession::clearData()
{
	_failures->clear();
	_successes->clear();
	_bins->clear();
	if (_status != 0L) _status->setStatusCode(STATUS_IDLE);
}

double JavaSession::getSampleAlpha(int index)
{
	return getSampleParameter(0,index);
}

double JavaSession::getSampleBeta(int index)
{
	return getSampleParameter(1,index);
}

int JavaSession::runAnalysis()
{
	int fail_cnt = 0;
	fail_cnt += _failures->getFailedObservationCount();
	fail_cnt += _bins->getFailedObservationCount();

	if (fail_cnt < 3) {
		this->clearSamples();
		return STATUS_INSUFFICIENT_DATA;
	}

	this->_sampler->setGenerator(new MHWeibullGenerator());
	
	try {
		WeibullFunction wFunc = WeibullFunction(*_joint);

		if (_bPriorSettingRequired) {
			wFunc.setPrior(_scale_nu, _scale_tau, _shape_nu, _shape_tau);
		}

		return analyze(wFunc);
	} catch (int errCode) {
#ifdef _DEBUG
		cout << "EXCEPTION THROWN: " << errCode << endl;
#endif
		this->clearSamples();
		return STATUS_ERROR;
	}
}

void JavaSession::checkSamplesChanged()
{
	if (haveSamplesChanged()) {
		_model.setSampleSet(getSampleSet());
		clearSamplesChanged();
	}
}

void JavaSession::addSample(double alpha, double beta, double probability)
{
	addSampleA(Parameters(alpha,beta),probability);
}

double JavaSession::getHazardRate(double time, double quantile)
{
	checkSamplesChanged();
	cursorChanged();
	return _model.getMeasure(hazard,quantile,time,0L);
}

double JavaSession::getReliability(double time, double quantile)
{
	checkSamplesChanged();
	cursorChanged();
	return _model.getMeasure(reliability,quantile,time,0L);
}

double JavaSession::getMTTF(double quantile)
{
	checkSamplesChanged();
	cursorChanged();
	return _model.getMeasure(mttf,quantile,0L);
}

double JavaSession::getCumulative(double time, double quantile)
{
	checkSamplesChanged();
	cursorChanged();
	return _model.getMeasure(cumulative,quantile,time,0L);
}

double JavaSession::getHazardRateMean(double time)
{
	checkSamplesChanged();
	cursorChanged();
	return _model.getMeasureMean(hazard,time,0L);
}

double JavaSession::getCumulativeMean(double time)
{
	checkSamplesChanged();
	cursorChanged();
	return _model.getMeasureMean(cumulative,time,0L);
}

double JavaSession::getReliabilityMean(double time)
{
	checkSamplesChanged();
	cursorChanged();
	return _model.getMeasureMean(reliability,time,0L);
}

double JavaSession::getBinnedAlpha(int index)
{
	return getBinnedParameter(0,index);
}

double JavaSession::getBinnedBeta(int index)
{
	return getBinnedParameter(1,index);
}

double JavaSession::getBinnedDensity(int alpha, int beta)
{
	HyperIndex hyper(2);
	hyper.setValue(0,alpha);
	hyper.setValue(1,beta);
	return getBinnedDensityA(hyper);
}

void JavaSession::setPrior(double nu1, double tau1, double nu2, double tau2)
{
	_bPriorSettingRequired = true;
	_scale_nu = nu1;
	_scale_tau = tau1;
	_shape_nu = nu2;
	_shape_tau = tau2;
}
