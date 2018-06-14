// $Id: DeviateGenerator.h 169 2005-01-28 14:17:28Z saulo $

#pragma once

#include <stdlib.h>

/**
 * Random number generator.
 * This class MUST be used when generating random numbers.
 */
class DeviateGenerator
{
public:
	DeviateGenerator(void);
	~DeviateGenerator(void);
	/// Returns a standard uniform random deviate.
	double uniform() const;
	/// Returns a discrete random number in the specified range.
	int discrete(const int min, const int max) const 
		{return min + (rand() % (max - min + 1));} 
	void setSeed(const long seed) {srand(seed);}
	double snormal(void) const;
};
