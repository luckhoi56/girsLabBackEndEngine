// BinnedSampleSpace.cpp: implementation of the BinnedSampleSpace class.
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
#include "BinnedSampleSpace.h"
#include "RANLIBInterface.h"
#include <Math.h>

#ifdef _DEBUG
#include <iostream>
#endif

#define STATUS_STEPS 100
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BinnedSampleSpace::BinnedSampleSpace(int bins, int spacing)
{
	_bins = bins;
	_spacing = spacing;
	_means = 0L;
	_probabilities = 0L;
}

BinnedSampleSpace::~BinnedSampleSpace()
{

#ifdef _DEBUG
//	cout << "CLEANING UP BINNED SAMPLE SPACE" << endl;
#endif

	delete _means;
	delete _probabilities;
	_minima.setDimension(0);
	_maxima.setDimension(0);
}

void BinnedSampleSpace::bin(SampleSet &set)
{
	clear();
	if (set.size() > 0) {
		Parameters minima = set.getMinima();
		Parameters maxima = set.getMaxima();
		bin(set,minima,maxima,false,0L);
	}
}

void BinnedSampleSpace::bin(SampleSet &set, Parameters &minima, Parameters &maxima, bool normalize, StatusPtr status)
{
	int cells;
	int index;
	double cumulative = 0;
	Parameters params;

#ifdef _DEBUG

	int * entries;

		int accepted = 0;
		cout << "BINNING IN FOLLOWING RANGE" << endl;
		minima.dump();
		maxima.dump();
#endif

	clear();

	if (set.size() > 0) {

		set.first();

		params = set.getParameters();

		_dimension = params.getDimension();
		_minima = minima;
		_maxima = maxima;

		cells = 1;

		for (int i = 0; i < _dimension; i++) {
			cells *= _bins;
		}

		_probabilities = new double[cells];

		for (int j = 0; j < cells; j++) {
			_probabilities[j] = 0;
		}

#ifdef _DEBUG
		entries = new int[cells];
		for (int kk = 0 ; kk < cells ; kk++ ) entries[kk] = 0;
#endif

		bool shouldRun = true;
		int size = set.size();
		div_t progress;
		set.first();

		for (int k = 0; k < size && shouldRun ; k++) {
		// perform the binning
//		while (!set.endOfSamples() && !status->isCancelRequested()) {

			params = set.getParameters();

			if (params.inSpace(minima,maxima)) {
				index = computeIndex(params);
				_probabilities[index] += set.getProbability();
				if (normalize) cumulative += _probabilities[index];

#ifdef _DEBUG
				entries[index]++;
				accepted++;
#endif
			}

			if (size > STATUS_STEPS && status != 0L) {
				progress = div(k , size / STATUS_STEPS);
				if (progress.rem == 0) {
					status->setProgress(progress.quot * 100 / STATUS_STEPS);
					shouldRun = !status->isCancelRequested();
				}
			}

			set.next();
		}

		if (normalize && cumulative > 0) {
			for (int k = 0 ; k < cells ; k++) {
				_probabilities[k] /= cumulative;
			}
		}
	}

#ifdef _DEBUG
	cout << "TOTAL SAMPLES    " << set.size() << endl;
	cout << "ACCEPTED SAMPLES " << accepted << endl;

	cout.precision(3);
	cout.setf(ios::scientific,ios::floatfield);

	for (int ii = 0 ; ii < _bins ; ii++) {
		for (int jj = 0 ; jj < _bins ; jj++) {
			HyperIndex hi(2);
			hi.setValue(0,ii);
			hi.setValue(1,jj);
			int binbin = computeIndex(hi);
//			cout << "CELL ";
			cout << ii << "," << jj;
			cout << " " ;
			cout << entries[binbin] ;
			cout << " " ;
			cout << _probabilities[binbin] ;
			cout << " ";
			cout << computeVolume(hi);
			cout << " ";
			cout << getDensity(hi);
//			cout << " ";
//			cout << entries[binbin] / _probabilities[binbin];
			cout << " ";
			cout << entries[binbin] / computeVolume(hi);
			cout << endl;
		}
	}

	delete [] entries;
#endif

	if (status && status->isCancelRequested()) clear();

}

void BinnedSampleSpace::clear()
{
	delete _means;
	delete _probabilities;
	_dimension = 0;
	_means = 0;
	_probabilities = 0;
}

int BinnedSampleSpace::computeIndex(const Parameters &params) const
{
	int index = 0;
	int base = 1;
	int bin = 0;

	for (int i = 0; i < _dimension; i++) {
		bin = computeBin(params.getValue(i),_minima.getValue(i),_maxima.getValue(i),_spacing,_bins);
		index += base * bin;
		base *= _bins;
	}

	return index;
}

int BinnedSampleSpace::computeIndex(const HyperIndex &hyper) const
{
	int index = 0;
	int base = 1;
	int bin = 0;

	for (int i = 0; i < _dimension; i++) {
		bin = hyper.getValue(i);
		index += base * bin;
		base *= _bins;
	}
	return index;
}

int BinnedSampleSpace::computeBin(double value, double min, double max, int spacing, int bins) const
{
	int bin = 0;

	if (spacing == BSS_SPACING_LINEAR) {
		bin = floor(bins * (value - min) / (max - min));
	} else {
		bin = floor(bins * log(value / min) / log(max / min));
	}

	if (bin >= bins) bin = bins - 1;
	if (bin < 0) bin = 0;

	return bin;
}

/**
 * Returns the probability for the specified cell.
 * Tested OK.
 */
double BinnedSampleSpace::getProbability(const HyperIndex &hyper) const
{
	int index = computeIndex(hyper);
	return _probabilities[index];
}

/**
 * Returns the density for the specified cell.
 * Tested OK.
 */
double BinnedSampleSpace::getDensity(const HyperIndex &hyper) const
{
	return getProbability(hyper) / computeVolume(hyper);
}

/**
 * Computes the volume of the specified cell.
 * Tested OK.
 */
double BinnedSampleSpace::computeVolume(const HyperIndex &hyper) const
{
	double volume = 1;
	int bins = _bins;

	for (int i = 0; i < _dimension; i++) {
		if (_spacing == BSS_SPACING_LINEAR) {
			volume *= (_maxima.getValue(i) - _minima.getValue(i)) / bins;
		} else {
//			delta = log(_maxima.getValue(i) / _minima.getValue(i));
//			min = _minima.getValue(i) * exp(delta * hyper.getValue(i) / bins);
//			max = _minima.getValue(i) * exp(delta * (hyper.getValue(i) + 1) / bins);
//			volume *= max - min;
			volume *= (_maxima.getValue(i) - _minima.getValue(i)) / bins;
		}
	}

	return volume;
}

Parameters BinnedSampleSpace::getMinima() 
{
	return Parameters(_minima);
}

Parameters BinnedSampleSpace::getMaxima() 
{
	return Parameters(_maxima);
}


double BinnedSampleSpace::getCenterValue(int index, int bin) const
{
	double max = getUpperBound(index,bin);
	double min = getLowerBound(index,bin);

	double center = 0;

	if (_spacing == BSS_SPACING_LINEAR) {
		center = (max + min) / 2;
	} else {
		center = sqrt(max * min);
	}
	return center;
}

double BinnedSampleSpace::getLowerBound(int index, int bin) const
{
	// function accepts bin values between 0 and _bins (inclusive)
	double bound = _minima.getValue(index);
	double delta = 0;
	if (_spacing == BSS_SPACING_LINEAR) {
		bound += bin * (_maxima.getValue(index) - _minima.getValue(index)) / _bins;
	} else {
		delta = log(_maxima.getValue(index) / _minima.getValue(index));
		bound *= exp(bin * delta / _bins);
	}
	return bound;
}

double BinnedSampleSpace::getUpperBound(int index, int bin) const
{
	return getLowerBound(index,bin + 1);	
}


void BinnedSampleSpace::testUnit()
{

#ifdef _DEBUG

	SampleSet samples;
	
	for (int i = 0 ; i < 40000 ; i++) {
//		double xx = exp((i % 300) / 50.);
//		double yy = exp((i / 300) / 50.);
//		double pp = log(xx * yy);
//		double xx = .1 + 0.9 * (i % 300) / 300.;
//		double yy = .1 + 0.9 * (i / 300) / 300.;
		double xx = RANLIBInterface::generateUniformDeviate();
		double yy = RANLIBInterface::generateUniformDeviate();
		double pp = 0;
		samples.addSample(Parameters(xx,yy),pp);
	}

	Parameters minima = samples.getMinima();
	Parameters maxima = samples.getMaxima();

	BinnedSampleSpace logbins(4,BSS_SPACING_LOGARITHMIC);
	logbins.bin(samples);

	BinnedSampleSpace linbins(4,BSS_SPACING_LINEAR);
	linbins.bin(samples);


	HyperIndex hi(2);
	for (int j = 0 ; j < 2 ; j++) {
		for (int k = 0 ; k < 2 ; k++) {
			hi.setValue(0,j);
			hi.setValue(1,k);
			cout << "LIN " << j << " " << k << " : " << linbins.getDensity(hi) << endl;
			cout << "LOG " << j << " " << k << " : " << logbins.getDensity(hi) << endl;
		}
	}

#endif
}

