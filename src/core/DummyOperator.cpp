// $Id: DummyOperator.cpp 169 2005-01-28 14:17:28Z saulo $

#include "DummyOperator.h"

#include <math.h>

DummyOperator::DummyOperator(double z)
{
	this->lnz = log(z);
}

DummyOperator::~DummyOperator(void)
{
}

double DummyOperator::compute(float * params, float * derivs) const 
{
	//const double lnz = log(2);
	
	double lnz1 = 1.6;
	double lnz2 = 1 / 1.6;
	
	double lna = params[0];
	double b = params[2];
	double lne = lna - lnz1 / b;
//	double lng = lne - lnz2 / b;

	params[1] = (float)lne;
//	params[2] = lng;

	double lnf = 0; // log(b);

	// normal prior over b.
	lnf += -0.5 * pow((b - 2) / 0.5,2);

	// normal prior over lne.
	lnf += -0.5 * pow((lne - 2) / 1.4,2);

	return lnf;
}

NetOperatorPtr DummyOperator::copy() const 
{
	DummyOperator * op = new DummyOperator(lnz);
	op->operator_id = operator_id;
	op->next = 0;
	return op;
}