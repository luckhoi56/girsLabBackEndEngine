// $Id: SliceSampler.h 522 2005-05-11 18:02:47Z fgroen $

#pragma once
#include "DeviateGenerator.h"
#include "ParamArray.h"

/**
 * Slice sampler operator.
 * The sampler can be used to advance a parameter vector
 * a place in a Markov Chain using the slice sampling 
 * technique.
 * 
 * This slice sampler was tested using Gaussian density
 * functions.jyf
 */
class SliceSampler
{
public:
	SliceSampler(void);
	~SliceSampler(void);
	int advance(ParamArray & params) const;

	void setSeed(const long seed) {deviate.setSeed(seed);}

protected:

	int shrink(ParamArray & params, const int index, const double lny,
				float lo, float hi, float & x1) const;
	DeviateGenerator deviate;
};







