// BinnedSampleSpace.h: interface for the BinnedSampleSpace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BINNEDSAMPLESPACE_H__7AD815C2_47C8_11D4_8735_000086586C9E__INCLUDED_)
#define AFX_BINNEDSAMPLESPACE_H__7AD815C2_47C8_11D4_8735_000086586C9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SampleSet.h"
#include "HyperIndex.h"
#include "Parameters.h"
#include "BinnedSpace.h"
#include "Status.h"

/* A class that is used to perform a binning on a sampleset with 
 * samples in an n-dimensional sample space. 
 *   
 * The binning is done using a constant number of bins for each 
 * dimension. Spacing of the bins can be either linear or logarithmic.
 * 
 * After a sampleset has been binned, both probability and
 * probability density for each bin are available. No restrictions
 * are made in terms of the dimensionality of the sample space.
 * Dimensionality is derived from the sample set.
 */ 
class BinnedSampleSpace : public BinnedSpace  
{
public:

	/* Bin the samples with the range specified by minima and maxima.
	 * The normalize flag indicates whether the bins should be renormalized.
	 */
	void bin(SampleSet & set, Parameters & minima, Parameters & maxima, bool normalize, StatusPtr status);

	/* Returns upper bound for specified bin in specified dimension
	 * Equal to getLowerBound(index,bin + 1)
	 */
	double getUpperBound(int index, int bin) const;

	/* Returns lower bound for specified bin in specified dimension
	 * Equal to getUpperBound(index,bin - 1);
	 */
	double getLowerBound(int index, int bin) const;

	double getCenterValue(int index, int bin) const;

	/* returns maximum parameter values in sample set
	 */
	Parameters getMaxima() ;

	/* returns minimum parameter values in sample set
	 */
	Parameters getMinima() ;

	/* returns density for specified bin
	 */
	double getDensity(const HyperIndex &hyper) const;

	/* returns probability for specified bin
	 */
	double getProbability(const HyperIndex & hyper) const;

	/* clears the bins
	 */
	void clear();

	/* performs binning of specified sample set.
	 */
	void bin(SampleSet & set);

	static void testUnit();

	BinnedSampleSpace();
	BinnedSampleSpace(int bins, int spacing);
	virtual ~BinnedSampleSpace();
private:
	/* converts hyperindex into index used for internal storage
	 */
	int computeIndex(const HyperIndex & hyper) const;

	/* returns the bin in which the specified value falls. The
	 * parameters min and max specify lower and upper limits of
	 * the sample space (for a single dimension). Spacing is the
	 * type of spacing.
	 */
	int computeBin(double value, double min, double max, int spacing, int bins) const;

	/* returns the index used for internal storage in which the
	 * given set of parameters belongs.
	 */
	int computeIndex(const Parameters & params) const;

	/* computes the volume of the specified bin.
	 */
	double computeVolume(const HyperIndex & hyper) const;

	int _bins;					// number of bins in each dimension
	int _spacing;				// type of spacing
	int _dimension;				// dimensionality of sample space
	Parameters * _means;		// for future use
	double * _probabilities;	// array of bin probabilities
	Parameters _minima;			// minima of sample space ranges
	Parameters _maxima;			// maxima of sample space ranges
};

#endif // !defined(AFX_BINNEDSAMPLESPACE_H__7AD815C2_47C8_11D4_8735_000086586C9E__INCLUDED_)
