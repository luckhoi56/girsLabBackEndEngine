// $Id: SliceSampler.cpp 2137 2013-08-01 03:33:59Z Dongfeng Zhu $

#include "slicesampler.h"
#include "slicesampleraids.h"

#include <math.h>

SliceSampler::SliceSampler(void)
{
}

SliceSampler::~SliceSampler(void)
{
}

/**
 * Advances the parameter vector to a next point in the Markov Chain.
 * 
 * The chain is advanced using the slice sampling algorithm.
 * The implementation is based on a description by Neal in the Annals
 * of Statistics (2003).
 *
 * The sampler samples new values for all non-dependent variables in
 * the model (the values of dependent variables are modified according
 * to the deterministic models).
 * 
 */
int SliceSampler::advance(ParamArray & params) const
{
	float lo,hi,x1;

	DefaultSliceSamplerAid default_aid((float)log(5.0),4,0);
	FixedSliceSamplerAid fixed_aid(0.1f,20,1);

	int len = params.size();

	for (int i = 0 ; i < len ; i++) 
	{
		short range = params.getRange(i);

		if (range != ParamArray::RANGE_DEPENDENT && range != ParamArray::RANGE_FIXED) 
		{
			float x0 = params.getParam(i);
			double lnfx0 = params.getLnDensity(i);

			// sample the auxiliary variable (Neal, Slice Sampling, 2003)
			double lnu = log(deviate.uniform());
			double lny = lnfx0 + lnu;

			SliceSamplerAid * aid = 0;

			if (range == ParamArray::RANGE_VARIABLE) {
				aid = params.getAid(i);
			}

			if (aid != 0) 
			{
				float x0 = params.getParam(i);
				aid->doStepOut(params,i,lny,lo,hi,deviate);
				shrink(params,i,lny,lo,hi,x1);
				params.updateParam(i,(float)x1);
				aid->notify(x1 - x0);
			}
		}
	}

	return 0;
}


/**
 * Performs the shrink operation.
 * This methods leaves the selected value x1 for the parameter in params.
 */
int SliceSampler::shrink(ParamArray & params, const int index, const double lny, float lo, float hi, float & x1) const
{
	double fx1,u;
	float x0 = params.getParam(index);
	bool done = false;

	do 
	{
		u = deviate.uniform();
		x1 = lo + (float)u * (hi - lo);
		params.updateParam(index,(float)x1);
		fx1 = params.getLnDensity(index);

		if (lny <= fx1) {
			done = true;
		} else if (x1 < x0) {
			lo = x1;
		} else {
			hi = x1;
		}
	} while (!done);

	return 0;
}




