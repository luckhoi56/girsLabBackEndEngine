// $Id: DummyOperator.h 169 2005-01-28 14:17:28Z saulo $

#pragma once
#include "NetOperator.h"

class DummyOperator :
	public NetOperator
{
public:
	DummyOperator(double z);
	~DummyOperator(void);

	virtual double compute(float * params, float * derivs) const ;
	virtual NetOperatorPtr copy() const ;

	virtual bool appliesTo(const int index) const 
		{return true;}
	virtual bool specifies(const int index) const
		{return false;}
		bool isDensity(const int index) const {return false;}


private:
	double lnz;


};
