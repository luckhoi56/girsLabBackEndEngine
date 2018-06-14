// BinnedSpace.h: interface for the BinnedSpace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BINNEDSPACE_H__E6060E01_485A_11D4_8735_000102479105__INCLUDED_)
#define AFX_BINNEDSPACE_H__E6060E01_485A_11D4_8735_000102479105__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HyperIndex.h"
#include "Parameters.h"

#define BSS_SPACING_LINEAR 1
#define BSS_SPACING_LOGARITHMIC 2


/* Binned sample space.
 *
 * TODO: allow samples to be binned within a specific region.
 */
class BinnedSpace  
{
public:

	virtual ~BinnedSpace();

	/* Returns upper bound for specified bin in specified dimension
	 * Equal to getLowerBound(index,bin + 1)
	 */
	virtual double getUpperBound(int index, int bin) const = 0;

	/* Returns lower bound for specified bin in specified dimension
	 * Equal to getUpperBound(index,bin - 1);
	 */
	virtual double getLowerBound(int index, int bin) const = 0;

	virtual double getCenterValue(int index, int bin) const = 0;

	/* returns density for specified bin
	 */
	virtual double getDensity(const HyperIndex &hyper) const = 0;

	/* returns probability for specified bin
	 */
	virtual double getProbability(const HyperIndex & hyper) const = 0;

	/* returns maximum parameter values in sample set
	 */
	virtual Parameters getMaxima() = 0;

	/* returns minimum parameter values in sample set
	 */
	virtual Parameters getMinima() = 0;

};

typedef BinnedSpace * BinnedSpacePtr;

#endif // !defined(AFX_BINNEDSPACE_H__E6060E01_485A_11D4_8735_000102479105__INCLUDED_)
