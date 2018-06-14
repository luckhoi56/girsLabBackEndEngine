// $Id:$ 

#include "SliceSamplerAids.h"

#include <math.h>

///////////////////////////////////////////////////////////////////

/**
 * Selects the slice interval.
 */
int DefaultSliceSamplerAid::doStepOut(ParamArray & params, const int index, 
		const double lny, float & lo, float & hi, const DeviateGenerator & deviate) const
{
	float u = (float)deviate.uniform();
	float x0 = params.getParam(index);

	lo = x0 - w * u;
	hi = lo + w;

	double v = deviate.uniform();
	int j = (int)floor(m * v);
	int k = m - 1 - j;

	params.updateParam(index,(float)lo);
	double flo = params.getLnDensity(index);
	while (j > 0 && lny < flo)
	{
		lo -= w;
		params.updateParam(index,(float)lo);
		flo = params.getLnDensity(index);
		j--;
	}

	params.updateParam(index,(float)hi);
	double fhi = params.getLnDensity(index);
	while (k > 0 && lny < fhi)
	{
		hi += w;
		params.updateParam(index,(float)hi);
		fhi = params.getLnDensity(index);
		k--;
	}

	params.updateParam(index,(float)x0);

	return 0;
}

/**
 * Selects the slice interval.
 */
int FixedSliceSamplerAid::doStepOut(ParamArray & params, const int index, const double lny, 
		float & lo, float & hi, const DeviateGenerator & deviate) const
{
	float x = params.getParam(index);
	lo = x - w > min ? x - w : min; 
	hi = x + w < max ? x + w : max;
	return 0;
}

///////////////////////////////////////////////////////////////////
/**
 * Selects the slice interval.
 */
int PositiveSliceSamplerAid::doStepOut(ParamArray & params, const int index, 
		const double lny, float & lo, float & hi, const DeviateGenerator & deviate) const
{
	float u = (float)deviate.uniform();
	float x0 = params.getParam(index);

	lo = x0 - w * u;
	hi = lo + w;

	double v = deviate.uniform();
	int j = (int)floor(m * v);
	int k = m - 1 - j;

	if (lo > 0) {
		params.updateParam(index,(float)lo);
		double flo = params.getLnDensity(index);
		while (j > 0 && lo > 0 && lny < flo)
		{
			lo -= w;
			if (lo > 0) {
				params.updateParam(index,(float)lo);
				flo = params.getLnDensity(index);
			} else lo = 0;
			j--;
		}
	} else lo = 0;

	params.updateParam(index,(float)hi);
	double fhi = params.getLnDensity(index);
	while (k > 0 && lny < fhi)
	{
		hi += w;
		params.updateParam(index,(float)hi);
		fhi = params.getLnDensity(index);
		k--;
	}

	params.updateParam(index,(float)x0);

	return 0;
}

///////////////////////////////////////////////////////////////////
/**
 * Selects the slice interval.
 */
int NegativeSliceSamplerAid::doStepOut(ParamArray & params, const int index, 
		const double lny, float & lo, float & hi, const DeviateGenerator & deviate) const
{
	float u = (float)deviate.uniform();
	float x0 = params.getParam(index);

	lo = x0 - w * u;
	hi = lo + w;

	double v = deviate.uniform();
	int j = (int)floor(m * v);
	int k = m - 1 - j;

	params.updateParam(index,(float)lo);
	double flo = params.getLnDensity(index);
	while (j > 0 && lny < flo)
	{
		lo -= w;
		params.updateParam(index,(float)lo);
		flo = params.getLnDensity(index);
		j--;
	} 

	if (hi < 0) {
		params.updateParam(index,(float)hi);
		double fhi = params.getLnDensity(index);
		while (k > 0 && hi < 0 && lny < fhi)
		{
			hi += w;
			if (hi < 0) {
				params.updateParam(index,(float)hi);
				fhi = params.getLnDensity(index);
			} else hi = 0;
			k--;
		}
	} else hi = 0;

	params.updateParam(index,(float)x0);

	return 0;
}


