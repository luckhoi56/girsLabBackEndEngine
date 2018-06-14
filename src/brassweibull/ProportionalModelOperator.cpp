// $Id: ProportionalModelOperator.cpp 169 2005-01-28 14:17:28Z saulo $

#include "proportionalmodeloperator.h"

#include <math.h>
#include <iostream>

using namespace std;

/**
 * Constructor.
 * @param alpha index of first scale
 * @param gamma index of second scale
 * @param beta index of (shared) shape
 * @param z adjustment factor
 */
ProportionalModelOperator::ProportionalModelOperator(int alpha, int gamma, int beta, double z)
{
	this->alpha = alpha;
	this->gamma = gamma;
	this->lnz = (float)log(z);
	this->beta = beta;
	this->direction = DIR_UNDECIDED;
}

ProportionalModelOperator::~ProportionalModelOperator(void)
{

}

/**
 * Returns true if the variable is the dependent variable for this model.
 * The dependent variable is alpha or  gamma, depending on the selected 
 * evaluation direction. The model must have been initialized.
 * @param var_index variable index.
 */
bool ProportionalModelOperator::specifies(const int var_index) const 
{
	return 
		(direction == DIR_A_TO_E && var_index == gamma) || 
		(direction == DIR_E_TO_A && var_index == alpha);
}

/**
 * Returns the variable specified by this model.
 */
int ProportionalModelOperator::getSpecified(void) const
{
	return direction == DIR_A_TO_E ? gamma : alpha;
}

/**
 * Initializes the model.
 * During this initialization, the model decides whether 
 * it is evaluated from alpha to gamma, or the other way
 * around. If the direction is undecided, the method returns
 * the next variable to be evaluated, or -1 if it can not
 * decide based on the input. If the direction had already
 * been decided, the method returns -1 if the link does not
 * need to be explored further (thereby indicating that a 
 * recursive exploration should be stopped) or -2 to indicate
 * that the input parameter (var_index) has been selected 
 * as the dependent parameter during an earlier call, and will
 * never cause any action by the model.
 */
int ProportionalModelOperator::init(const int var_index)
{
	if (direction == DIR_UNDECIDED) {
		if (var_index == alpha) {
			//cout << alpha << " -> " << gamma << endl;
			direction = DIR_A_TO_E;
			return gamma;
		} else if (var_index == gamma) {
			//cout << gamma << " -> " << alpha << endl;
			direction = DIR_E_TO_A;
			return alpha;
		} else {
			return -1;
		}
	} else return -1;
}

//////////////////////////////////////////////////////////////////

double ProportionalTimeOperator::update(float * params,int & next_var)
{
	if (direction == DIR_UNDECIDED) return -1;

	if (direction == DIR_A_TO_E) {
		float lna = params[alpha];
		float lng = lna - lnz;
		params[gamma] = lng;
		next_var = gamma;
	} else {
		float lng = params[gamma];
		float lna = lng + lnz;
		params[alpha] = lna;
		next_var = alpha;
	}
	return 0;
}

NetModelPtr ProportionalTimeOperator::copy() const {
	ProportionalTimeOperator * op = new ProportionalTimeOperator(*this);
	return op;
}


//////////////////////////////////////////////////////////////////

double ProportionalRateOperator::update(float * params,int & next_var)
{
	if (direction == DIR_UNDECIDED) return -1;

	if (direction == DIR_A_TO_E) {
		float lna = params[alpha];
		float b = params[beta];
		float lng = lna - lnz / b;
		params[gamma] = lng;
		next_var = gamma;
	} else {
		float lng = params[gamma];
		float b = params[beta];
		float lna = lng + lnz / b;
		params[alpha] = lna;
		next_var = alpha;
	}
	return 0;
}

NetModelPtr ProportionalRateOperator::copy() const {
	ProportionalRateOperator * op = new ProportionalRateOperator(*this);
	return op;
}

/**
 * Stream operator for parameter arrays.
 */
ostream & operator<<(ostream & os, const ProportionalModelOperator & m)
{
	m.print(os);
	return os;
}

/**
 * Copies the model to the ostream.
 */
void ProportionalModelOperator::print(ostream & os) const
{
	if (direction == DIR_A_TO_E) {
		os << "(" << alpha << " -> " << gamma << ")";
	} else if (direction == DIR_E_TO_A) {
		os << "(" << gamma << " -> " << alpha << ")";
	} else {
		os << "(?? -> ??)";
	}
}
