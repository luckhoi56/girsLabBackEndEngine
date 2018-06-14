// SampleSet.h: interface for the SampleSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAMPLESET_H__7B40FBC9_3FB7_11D4_9882_002018557056__INCLUDED_)
#define AFX_SAMPLESET_H__7B40FBC9_3FB7_11D4_9882_002018557056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Parameters.h"
#include "Sample.h"
#include "Distribution.h"

#include <list>

using namespace std;

typedef list<Sample> SampleList;

/**
 * 
 * Navigation through the samples takes place by means of the
 * first() and next() methods.
 *
 * The weight associated with each sample must be specified
 * in terms of its natural log value. 
 */
class SampleSet  
{
public:
	int getDimension();

	void getMarginal(int index, Distribution & distribution);

	Parameters getMaxima() ;
	Parameters getMinima() ;
	Parameters getVariance();
	Parameters getMean();

	/* Returns extreme values within sampleset for 
	 * parameter with specified index.
	 */
	void getExtremes(int index, double & minimum, double & maximum);

	/**
	 * Performs unit test on the class.
	 */
	static void testUnit();

	/**
	 * Moves to the first sample in the set.
	 */
	void first();

	/**
	 * Moves to the next sample in the set.
	 */
	void next();

	/**
	 * Advances specified number of steps in the list.
	 */
	void moveBy(int step);

	/**
	 * Returns true when end of samples is reached.
	 */
	bool endOfSamples() ;

	/**
	 * Returns the probability of the current sample.
	 */
	double getProbability();

	/**
	 * Returns natural log of the weight of the current sample.
	 */
	double getLogWeight() const;

	/**
	 * Returns the parameters of the current sample.
	 */
	Parameters getParameters() const;

	/**
	 * Returns the number of samples in the set.
	 */
	int size() const;

	/**
	 * Clears the samples from the set.
	 */
	void clear();

	/**
	 * Adds a new sample to the set.
	 */
	void addSample(const Parameters & params, double logWeight);

	SampleSet();
	virtual ~SampleSet();
private:
	void normalize();
	SampleList _samples;
	SampleList::iterator _cursor;
	double _normalizer;  // natural log of normalization factor
	bool _normalized;	 // true when set has been normalized
};

typedef SampleSet * SampleSetPtr;

#endif // !defined(AFX_SAMPLESET_H__7B40FBC9_3FB7_11D4_9882_002018557056__INCLUDED_)
