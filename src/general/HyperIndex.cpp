// HyperIndex.cpp: implementation of the HyperIndex class.
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
#include "HyperIndex.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HyperIndex::HyperIndex()
{
	_dimension = 0;
	_index = 0L;
}

HyperIndex::HyperIndex(const HyperIndex & hyper)
{
	_dimension = hyper.getDimension();
	_index = new int[_dimension];

	for (int i = 0 ; i < _dimension; i++) {
		_index[i] = hyper.getValue(i);
	}
}

HyperIndex::HyperIndex(int dimension)
{
	_dimension = dimension;
	_index = new int[dimension];

	for (int i = 0 ; i < _dimension; i++) {
		_index[i] = 0;
	}

}

HyperIndex::~HyperIndex()
{
	delete _index;
}


void HyperIndex::setValue(int index, int value)
{
	_index[index] = value;
}

int HyperIndex::getValue(int index) const
{
	return _index[index];
}

int HyperIndex::getDimension() const
{
	return _dimension;
}