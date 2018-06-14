// DistributionPoint.cpp: implementation of the DistributionPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DistributionPoint.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DistributionPoint::DistributionPoint()
{
	_value = 0;
	_probability = 0;
}

DistributionPoint::DistributionPoint(double value, double probability)
{
	_value = value;
	_probability = probability;
}

DistributionPoint::~DistributionPoint()
{
	// Do nothing
}

double DistributionPoint::getProbability() const
{
	return _probability;
}

double DistributionPoint::getValue() const
{
	return _value;
}

bool DistributionPoint::operator< (const DistributionPoint & point) const
{
	return _value < point.getValue();
}

bool DistributionPoint::operator<= (const DistributionPoint & point) const
{
	return _value <= point.getValue();
}

bool DistributionPoint::operator> (const DistributionPoint & point) const
{
	return _value > point.getValue();
}

bool DistributionPoint::operator>= (const DistributionPoint & point) const
{
	return _value >= point.getValue();
}

bool DistributionPoint::operator== (const DistributionPoint & point) const
{
	return _value == point.getValue();
}

bool DistributionPoint::operator!= (const DistributionPoint & point) const
{
	return _value != point.getValue();
}
