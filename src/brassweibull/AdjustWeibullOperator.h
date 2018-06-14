// $Id: AdjustWeibullOperator.h 169 2005-01-28 14:17:28Z saulo $

#pragma once

#include "../core/NetOperator.h"

/**
 * Super-class for Weibull design impact adjustment operator.
 * Represents the dependency according to the model
 * lambda(gamma,beta) = z * lambda(alpha,beta), where the lambda function is
 * Weibull hazard rate, and z is lognormally distribution with specified median and 
 * error factor. In other words, the operator implements the proportional 
 * hazard rate model dependency, where the proportional factor z is uncertain, 
 * and assumed to be lognormally distributed.
 *
 * The operator assumes that the parameter array contains the natural logs of the 
 * alpha, gamma, and beta parameters.
 */
class AdjustWeibullOperator : public NetOperator
{
public:
	AdjustWeibullOperator(const int alpha, const int gamma, const int beta,
		const double median, const double ef);
	virtual ~AdjustWeibullOperator(void);
	virtual bool appliesTo(const int index) const 
		{return alpha == index || gamma == index || beta == index;}
	virtual bool specifies(const int index) const
		{return false;}

protected:

	int alpha;		// alpha index
	int gamma;		// gamma index
	int beta;		// beta index
	double median;	// mean/median of the underlying normal
	double sigma;   // sdev of the underlying normal
};

/**
 * Operator class for Weibull design impact failure rate adjustment operator.
 * Represents the dependency according to the model
 * lambda(gamma,beta) = z * lambda(alpha,beta), where the lambda function is
 * Weibull hazard rate, and z is lognormally distribution with specified median and 
 * error factor. In other words, the operator implements the proportional 
 * hazard rate model dependency, where the proportional factor z is uncertain, 
 * and assumed to be lognormally distributed.
 *
 * The operator assumes that the parameter array contains the natural logs of the 
 * alpha, gamma, and beta parameters.
 * @see ProportionalRateOperator
 * @see AdjustWeibullTimeOperator
 */
class AdjustWeibullRateOperator : public AdjustWeibullOperator
{
public:
	AdjustWeibullRateOperator(const int alpha, const int gamma, const int beta,
		const double median, const double ef);
	virtual ~AdjustWeibullRateOperator(void) {}
	double compute(float * params, float * derivs) const;
	NetOperatorPtr copy(void) const;

	bool isDensity(const int index) const;
};

/**
 * Class for Weibull design impact time-to-failure adjustment operator.
 * Represents the dependency according to the model
 * R(t|gamma,beta) = R(z*t|alpha,beta), where the R function is
 * Weibull reliability, and z is lognormally distribution with specified median and 
 * error factor. In other words, the operator implements the proportional 
 * hazard rate model dependency, where the proportional factor z is uncertain, 
 * and assumed to be lognormally distributed.
 *
 * The operator assumes that the parameter array contains the natural logs of the 
 * alpha, gamma, and beta parameters.
 * @see ProportionalTimeOperator
 * @see AdjustWeibullRateOperator
 */
class AdjustWeibullTimeOperator : public AdjustWeibullOperator
{
public:
	AdjustWeibullTimeOperator(const int alpha, const int gamma, const int beta,
		const double median, const double ef);
	virtual ~AdjustWeibullTimeOperator(void) {}
	double compute(float * params, float * derivs) const;
	NetOperatorPtr copy(void) const;

	bool isDensity(const int index) const;
};
