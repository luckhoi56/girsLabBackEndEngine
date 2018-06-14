// $Id: NormalDensityOperator.h 169 2005-01-28 14:17:28Z saulo $

#pragma once
#include "netoperator.h"

/**
 * Operator that represents the normal (Gaussian) density for the specified parameter.
 */
class NormalDensityOperator :
	public NetOperator
{
public:
	NormalDensityOperator(const int param,const double mean,const double sdev);
	~NormalDensityOperator(void);
	double compute(float * params, float * derivs) const;
	NetOperatorPtr copy(void) const;
	virtual bool appliesTo(const int index) const 
		{return param == index;}
	virtual bool specifies(const int index) const
		{return false;}
	bool isDensity(const int index) const;

protected:
	double mean;
	double sdev;
	int param;
};


/**
 * Operator that represents the lognormal density for the specified parameter.
 */
class LogNormalDensityOperator :
	public NetOperator
{
public:
	LogNormalDensityOperator(const int param,const double median,const double errfac);
	~LogNormalDensityOperator(void);
	double compute(float * params, float * derivs) const;
	NetOperatorPtr copy(void) const;
	virtual bool appliesTo(const int index) const 
		{return param == index;}
	virtual bool specifies(const int index) const
		{return false;}
	bool isDensity(const int index) const;

protected:
	double mean;
	double sdev;
	int param;
};
