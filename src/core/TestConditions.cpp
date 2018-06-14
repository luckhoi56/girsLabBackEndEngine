// $Id: TestConditions.cpp 169 2005-01-28 14:17:28Z saulo $

#include "TestConditions.h"

/**
 * Constructor.
 * Initializes the object for specified number of 
 * test conditions and failure modes. All acceleration
 * factors are initialized to ACCELERATE_NONE.
 */
TestConditions::TestConditions(int conditions, int modes)
{
	use_count = 0;
	condition_cnt = conditions;
	mode_cnt = modes;

	int size = condition_cnt * mode_cnt;
	accelerations = new AccelerationFactor[size];

	for (int i = 0 ; i < size ; i++) {
		accelerations[i].factor = 1;
		accelerations[i].type = AccelerationFactor::ACCELERATE_NONE;
	}
}

/**
 * Destructor.
 */
TestConditions::~TestConditions(void)
{
	delete [] accelerations;
}

/**
 * Sets the acceleration factor details.
 * Type is one of acceleration type constants defined in this class.
 * Failure mode numbering starts at 1. Condition numbering starts at 0.
 * @param condition test condition
 * @param mode failure mode
 * @param type acceleration mode type
 * @param factor acceleration factor
 */
void TestConditions::setAcceleration(int condition, int mode, int type, double factor)
{ 
	AccelerationFactor * f = &accelerations[condition_cnt * (mode - 1) + condition];

	if (factor <= 0) 
	{
		type = AccelerationFactor::ACCELERATE_NULL;
		factor = 0;
	}

	if (type == AccelerationFactor::ACCELERATE_NONE) 
	{
		factor = 1;
	}

	f->factor = factor;
	f->type = type;
}
