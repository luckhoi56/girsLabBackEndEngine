// Session.cpp: implementation of the Session class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Session.h"

#include "MetropolisSampler.h"
#include "BinnedSampleSpace.h"
#include "HyperIndex.h"
#include <math.h>

#ifdef _DEBUG
#include <iostream>
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Session::Session()
{
	_sampler = new MetropolisSampler(_sampleSet);
	_status = new Status();
	_samplesChanged = true;
	_cursorChanged = true;
	_space = 0L;
	_currentIndex = -1;
	_marginalParam = -1;
}

Session::~Session()
{
	delete _sampler;
	delete _space;
	delete _status;
}


void Session::clearSamples()
{

#ifdef _DEBUG
			cout << "DLL session clearing samples" << endl;
#endif

	_sampleSet.clear();
	_marginal.clear();
	_marginalParam = -1;

	delete _space;
	_space = 0L;

	samplesChanged();
	cursorChanged();

#ifdef _DEBUG
			cout << "DLL session clearing samples done" << endl;
#endif
}

int Session::getSampleCount()
{
	return _sampleSet.size();
}

double Session::getSampleParameter(int parameter, int index)
{
	moveToSample(index);
	return _currentParams.getValue(parameter);
}

double Session::getSampleProbability(int index)
{
	moveToSample(index);
	return _currentProbability;
}

void Session::moveToSample(int index)
{
//	_sampleSet.first();
//	_sampleSet.moveBy(index);

	if (_cursorChanged || index != _currentIndex) {
		if (_cursorChanged || index < _currentIndex) {
			// if cursor in sampleset has possibly changed
			// or cursor beyond desired position then reset cursor
			_currentIndex = 0;
			_sampleSet.first();

			_cursorChanged = false;
		}

		if (index - _currentIndex == 1) {
			_sampleSet.next();
		} else {
			_sampleSet.moveBy(index - _currentIndex);
		}

		_currentIndex = index;
		_currentParams = _sampleSet.getParameters();
		_currentProbability = _sampleSet.getProbability();
	}
}

void Session::setSampleCount(int count)
{
	_sampler->setSampleSize(count);
}

int Session::analyze(Function & function)
{
	int result = _sampler->sample(function,_status);
	samplesChanged();
	cursorChanged();
	return result;
}

void Session::samplesChanged()
{
	/* flag is set whenever operation is performed
	 * that affects the content of the sample set.
	 * Derived results are cleaned up.
	 */
	_samplesChanged = true;
	delete _space;
	_space = 0L;
}

bool Session::haveSamplesChanged()
{
	return _samplesChanged;
}

void Session::clearSamplesChanged()
{
	_samplesChanged = false;
}

void Session::cursorChanged()
{
	/* flag is set whenever operation is performed
	 * that possibly changes the cursor in the 
	 * sample set.
	 */
	_cursorChanged = true;
}

void Session::addSampleA(Parameters & params, double probability)
{
	_sampleSet.addSample(params,log(probability));
	samplesChanged();
	cursorChanged();
}

void Session::constructBinnedSpace(int size, bool logScale)
{

	int dimension = _sampleSet.getDimension();
	Parameters minima(dimension);
	Parameters maxima(dimension);

	if (_sampleSet.size() > 0) {
		Distribution marginal;
		for (int i = 0 ; i < dimension ; i++) {
			_sampleSet.getMarginal(i,marginal);
			minima.setValue(i,marginal.getPercentile(0.005));
			maxima.setValue(i,marginal.getPercentile(0.995));
		}
	}

	constructBinnedSpace(size,minima,maxima,logScale);

	cursorChanged();
}

void Session::constructBinnedSpace(int size, Parameters & minima, Parameters & maxima, bool logScale)
{

	delete _space;
	_space = 0L;

	int mode = (logScale ? BSS_SPACING_LOGARITHMIC : BSS_SPACING_LINEAR);

	if (_sampleSet.size() > 0) {
		// perform binning operation
		BinnedSampleSpace * space = new BinnedSampleSpace(size,mode);
		space->bin(_sampleSet,minima,maxima,false,_status);
		_space = space;
	}

	cursorChanged();
}




double Session::getBinnedParameter(int parameter, int index)
{
	double value = 0;

	if (_space != 0L) {
		value = _space->getCenterValue(parameter,index);
	} else {
		/* bogus output */
		value = index;
	}

	cursorChanged();

	return value;
}

double Session::getBinnedDensityA(HyperIndex & hyper)
{
	double density = 1;

	if (_space != 0L) {
		density = _space->getDensity(hyper);
	}

	cursorChanged();

	return density;
}

double Session::getMarginalDensity(double x0, double x1)
{
	return _marginal.getAvgDensity(x0,x1);
}

double Session::getMarginalCumulative(double x)
{
	return _marginal.getCumulative(x);
}

void Session::getMarginalExtremes(double & min, double & max)
{
	_marginal.getExtremes(min,max);
}


int Session::getMarginalSize()
{
	return _marginal.size();
}

void Session::constructMarginal(int index,int intervals, bool logScale)
{
	if (_sampleSet.size() > 0) {
		if (_marginalParam != index) {
			_sampleSet.getMarginal(index,_marginal);

			_marginal.reduce(intervals,logScale);

			_marginalParam = index;
		}
	}
}

void Session::getMarginalPoint(int point, double &value, double &probability)
{
	_marginal.getPoint(point,value,probability);
}


void Session::getStatus(char *buffer, int maxChars)
{
	if (_status != 0L) 
		_status->getStatus(buffer,maxChars);
	else
		buffer[0] = 0;			
}

int Session::getProgress()
{
	if (_status != 0)
		return _status->getProgress();
	else
		return 0;
}

void Session::requestCancel()
{
	if (_status != 0) _status->requestCancel();
}

int Session::getStatusCode() const
{
	int code = STATUS_IDLE;
	if (_status != 0L) code = _status->getStatusCode();
	return code;
}

void Session::clearStatus()
{
	if (_status != 0) _status->clear();
}

void Session::setStatusObject(Status *status)
{
	delete _status;
	_status = status;
}
