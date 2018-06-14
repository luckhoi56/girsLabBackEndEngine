// $Id: NormalDensityOperator.cpp 169 2005-01-28 14:17:28Z saulo $

#include "NormalDensityOperator.h"
#include <math.h>
#include <iostream>

using namespace std;

/**
 * Operator that represents the normal (Gaussian) density for the specified parameter.
 */
NormalDensityOperator::NormalDensityOperator(const int param,const double mean,const double sdev)
{
	this->param = param;
	this->mean = mean;
	this->sdev = sdev;
}

NormalDensityOperator::~NormalDensityOperator(void)
{
}

/**
 * Computes the normal density. 
 * A value proportional to the natural log of the density is returned.
 */
double NormalDensityOperator::compute(float * params, float * derivs) const
{
	double lnd = -pow((params[param] - mean)/sdev,2) / 2 ; 
	return lnd;
}

/**
 * Returns an unlinked copy of this operator.
 */
NetOperatorPtr NormalDensityOperator::copy(void) const
{
	NormalDensityOperator * op = new NormalDensityOperator(*this);
	op->operator_id = this->operator_id;
	op->next = 0;
	return op;
}


/**
 * Constructor.
 * @param param int variable to which density applies.
 * @param median double median of the lognormal distribution
 * @param errfac double error factor of the lognormal distribution.
 */
LogNormalDensityOperator::LogNormalDensityOperator(const int param,const double median,const double errfac)
{
	this->param = param;
	this->mean = log(median);
	this->sdev = log(errfac)/1.645;
}

LogNormalDensityOperator::~LogNormalDensityOperator(void)
{
}

/**
 * Computes the normal density. 
 * A value proportional to the natural log of the density is returned.
 */
double LogNormalDensityOperator::compute(float * params, float * derivs) const
{
		double lnx = log(params[param]);
		double lnd = -pow((lnx - mean)/sdev,2) / 2 - lnx ; 
		return lnd;
}

/**
 * Returns an unlinked copy of this operator.
 */
NetOperatorPtr LogNormalDensityOperator::copy(void) const
{
	LogNormalDensityOperator * op = new LogNormalDensityOperator(*this);
	op->operator_id = this->operator_id;
	op->next = 0;
	return op;
}

bool NormalDensityOperator::isDensity(const int index) const
{
	if (this->appliesTo(index)) return true;
	if (next != 0) return next->isDensity(index);
	return false;
}

bool LogNormalDensityOperator::isDensity(const int index) const
{
	if (this->appliesTo(index)) return true;
	if (next != 0) return next->isDensity(index);
	return false;
}