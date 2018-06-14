// HyperIndex.h: interface for the HyperIndex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HYPERINDEX_H__7AD815C3_47C8_11D4_8735_000086586C9E__INCLUDED_)
#define AFX_HYPERINDEX_H__7AD815C3_47C8_11D4_8735_000086586C9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/* Multidimensional index. Instances act as multidimensional
 * indeces in for instance binned hyperspaces.
 */
class HyperIndex  
{
public:

	/* Returns the dimension of the hyperindex. */
	int getDimension() const;

	/* Returns the specified index. */
	int getValue(int index) const;

	/* Sets the specified index. */
	void setValue(int index, int value);

	HyperIndex();
	HyperIndex(int dimension);
	HyperIndex(const HyperIndex & hyper);
	virtual ~HyperIndex();

private:
	int * _index;
	int _dimension;
};

#endif // !defined(AFX_HYPERINDEX_H__7AD815C3_47C8_11D4_8735_000086586C9E__INCLUDED_)
