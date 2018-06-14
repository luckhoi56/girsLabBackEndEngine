// $Id: AllocatingWeibullPrior.h 169 2005-01-28 14:17:28Z saulo $
#pragma once

#include "../core/NetOperator.h"

/** 
 * This class is not in use.
 */
class AllocatingWeibullPrior :
	public NetOperator
{
public:
	AllocatingWeibullPrior(void);
	~AllocatingWeibullPrior(void);
	double compute(float * params, float * derivs) const;
	NetOperatorPtr copy() const {AllocatingWeibullPrior * op = new AllocatingWeibullPrior(); op->operator_id = operator_id; return op;}
	virtual bool appliesTo(const int index) const {return true;}
	virtual bool specifies(const int index) const {return false;}

	bool isDensity(const int index) const {return false;}

};
