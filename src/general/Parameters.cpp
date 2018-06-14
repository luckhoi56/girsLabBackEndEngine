// Parameters.cpp: implementation of the Parameters class.
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
#include "Parameters.h"
#include "math.h"

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif

#ifdef _DEBUG
int Parameters::_instances = 0;
int Parameters::_nextID = 1;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Parameters::Parameters()
{
#ifdef _DEBUG
	_instances++;
	_objectID = _nextID;
	_nextID++;
//	cout << "ALLOCATING PARAMETERS : " << _objectID << endl;
#endif
	_values = 0L;
	_dimension = 0;
}

Parameters::Parameters(int dimension)
{
#ifdef _DEBUG
	_instances++;
	_objectID = _nextID;
	_nextID++;
//	cout << "ALLOCATING PARAMETERS : " << _objectID << endl;
#endif
	_values = new double[dimension];
	_dimension = dimension;
	for (int index = 0; index < dimension; index++) {
		_values[index] = 0.0;
	}
}

Parameters::Parameters(double value0, double value1)
{
#ifdef _DEBUG
	_instances++;
	_objectID = _nextID;
	_nextID++;
//	cout << "ALLOCATING PARAMETERS : " << _objectID << endl;
#endif
	_dimension = 2;
	_values = new double[2];
	_values[0] = value0;
	_values[1] = value1;
}
 

Parameters::Parameters(double value)
{
	_dimension = 1;
	_values = new double[1];
	_values[0] = value;
}

Parameters::Parameters(const Parameters &params)
{
#ifdef _DEBUG
	_instances++;
	_objectID = _nextID;
	_nextID++;
//	cout << "ALLOCATING PARAMETERS : " << _objectID << endl;
#endif
	_dimension = params.getDimension();
	_values = new double[_dimension];
	for (int index = 0; index < _dimension; index++) {
		_values[index] = params.getValue(index);
	}
}

Parameters::~Parameters()
{
	if (_values != 0L) {
		delete _values;
	}

#ifdef _DEBUG
//	cout << "DEALLOCATING PARAMETERS : " << _objectID << endl;
	_instances --;
//	if (_instances == 0)
//		cout << "ALL PARAMETERS DEALLOCATED" << endl;
//	if (_instances == -1)
//		cout << "DEALLOCATION ERROR" << endl;
#endif
}

Parameters Parameters::operator+ (const Parameters & rhs) const
{
	Parameters result(_dimension);
	if (rhs.getDimension() == _dimension) {
		for (int index = 0; index < _dimension; index++) {
			result.setValue(index,_values[index] + rhs.getValue(index));
		}
	}
	return result;
}

void Parameters::setDimension(int dimension)
{
	if (dimension != _dimension) {

		if (_values != 0L) {
			delete _values;
		}

		_values = new double[dimension];
		_dimension = dimension;
	}
}


Parameters Parameters::operator- (const Parameters & rhs) const
{
	Parameters result(_dimension);
	if (rhs.getDimension() == _dimension) {
		for (int index = 0; index < _dimension; index++) {
			result.setValue(index,_values[index] - rhs.getValue(index));
		}
	}
	return result;
}

Parameters Parameters::operator* (const double rhs) const
{
	Parameters result(_dimension);
	for (int index = 0; index < _dimension; index++) {
		result.setValue(index,_values[index] * rhs);
	}
	return result;
}

//Parameters & Parameters::operator= (const Parameters & rhs)
void Parameters::operator= (const Parameters & rhs)
{
	// resize if necessary
	setDimension(rhs.getDimension());

	for (int index = 0; index < _dimension; index++) {
		_values[index] = rhs.getValue(index);
	}

//	return *this;
}

Parameters Parameters::getExponential() const
{
	Parameters exponent(_dimension);
	for (int i = 0; i < _dimension; i++) {
		exponent.setValue(i,exp(_values[i]));
	}
	return exponent;
}

Parameters Parameters::getSquared() const
{
	Parameters square(_dimension);
	for (int i = 0; i < _dimension; i++) {
		square.setValue(i,_values[i] * _values[i]);
	}
	return square;
}

double Parameters::getValue(int index) const
{
#ifdef _DEBUG
	if (index >= _dimension) 
		return -99;
	else
#endif
	return _values[index];
}

void Parameters::setValue(int index,double value)
{
	_values[index] = value;
}

int Parameters::getDimension() const
{
	return _dimension;
}

bool Parameters::inSpace(const Parameters &minima, const Parameters &maxima) const
{
	bool inside = true;

	for (int i = 0 ; i < _dimension && inside ; i++) {
		inside = (_values[i] >= minima.getValue(i)) && (_values[i] <= maxima.getValue(i));
	}

	return inside;
}

/***********************************************************/

void Parameters::dump() {
#ifdef _DEBUG
	for (int i = 0; i < _dimension; i++) cout << _values[i] << " : " ;
	cout << endl;
#endif
}

void Parameters::testUnit() {
#ifdef _DEBUG
	Parameters x(2);
	Parameters p11(1,1);
	Parameters p01(0,1);

	x = p11;
	x.dump();

	x = x + p11;
	x.dump();

	x = x - p11;
	x.dump();

	x = x * 3;
	x.dump();

	x = p11 + p01;
	x.dump();

	x = p11 * 1 + p01 * 2;
	x.dump();

#endif
}


