// $Id: AdjustWeibullOperator.cpp 169 2005-01-28 14:17:28Z saulo $

#include "adjustweibulloperator.h"

#include <math.h>
#include <iostream>

using namespace std;

/**
 * Weibull adjustment operator.
 * Represents the dependency according to the model
 * lambda(gamma,beta) = z * lambda(alpha,beta), where the lambda function is
 * Weibull, and z is lognormally distribution with specified median and error 
 * error factor.
 * @param alpha   index of the alpha parameter
 * @param gamma   index of the gamma parameter
 * @param beta    index of the beta parameter
 * @param median  median of the z parameter distribution
 * @param ef      error factor of the z parameter distribution
 */
AdjustWeibullOperator::AdjustWeibullOperator(const int alpha, const int gamma, const int beta,
											 const double median, const double ef)
{
	this->alpha = alpha;
	this->gamma = gamma;
	this->beta = beta;
	this->median = log(median);
	this->sigma = log(ef) / 1.645;
}

AdjustWeibullOperator::~AdjustWeibullOperator(void)
{

}

AdjustWeibullRateOperator::AdjustWeibullRateOperator(const int alpha, const int gamma, 
		const int beta,	const double median, const double ef) : 
		AdjustWeibullOperator(alpha,gamma,beta,median,ef) 
{
//	cout << "OPERATOR " << operator_id << ": ADJUST WEIBULL RATE" << endl;
}

AdjustWeibullTimeOperator::AdjustWeibullTimeOperator(const int alpha, const int gamma, 
		const int beta, const double median, const double ef) : 
		AdjustWeibullOperator(alpha,gamma,beta,median,ef) 
{
//	cout << "OPERATOR " << operator_id << ": ADJUST WEIBULL TIME" << endl;			
}


/**
 * Returns an unlinked copy of this operator.
 * This method returns a copy of this operator that has as only
 * difference the fact that the operator is not part of an 
 * operator list.
 * @see NetOperator#link(NetOperatorPtr)
 */
NetOperatorPtr AdjustWeibullRateOperator::copy(void) const
{
	AdjustWeibullRateOperator * cpy = new AdjustWeibullRateOperator(*this);
	cpy->operator_id = operator_id;
	cpy->next = 0;
	return cpy;
}

bool AdjustWeibullRateOperator::isDensity(const int index) const
{
	if (this->appliesTo(index)) return true;
	if (next != 0) return next->isDensity(index);
	return false;
}

NetOperatorPtr AdjustWeibullTimeOperator::copy(void) const
{
	AdjustWeibullTimeOperator * cpy = new AdjustWeibullTimeOperator(*this);
	cpy->operator_id = operator_id;
	cpy->next = 0;
	return cpy;
}

/**
 * Computes the log of the likelihood of the parameter vector.
 * The likelihood is computed based on the current values stored in 
 * the params array, and the specified indeces of ln(alpha),ln(gamma),and
 * beta.
 * @param params array of parameter values
 */
double AdjustWeibullRateOperator::compute(float * params, float * derivs) const
{
	double lna = params[alpha];
	double b = params[beta];
	double lng = params[gamma];
	double lnz = (lna - lng) * b;
	double f = -0.5 * pow((lnz - median) / sigma,2) + log(b);
	return f;
}

/**
 * Computes the log of the likelihood of the parameter vector.
 * The likelihood is computed based on the current values stored in 
 * the params array, and the specified indeces of ln(alpha),ln(gamma),and
 * beta.
 * @param params array of parameter values
 */
double AdjustWeibullTimeOperator::compute(float * params, float * derivs) const
{
	float lna = params[alpha];
	float lng = params[gamma];
	float lnz = (lna - lng);
	float f = -0.5f * (float)pow((lnz - median) / sigma,2);
	return f;
}

bool AdjustWeibullTimeOperator::isDensity(const int index) const
{
	if (this->appliesTo(index)) return true;
	if (next != 0) return next->isDensity(index);
	return false;
}
