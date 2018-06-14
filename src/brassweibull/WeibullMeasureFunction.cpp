// $Id: WeibullMeasureFunction.cpp 1089 2005-08-08 16:31:42Z fgroen $

#include "WeibullMeasureFunction.h"

WeibullMeasureFunction::WeibullMeasureFunction(void)
{
}

WeibullMeasureFunction::~WeibullMeasureFunction(void)
{
}

/**
 * Returns the cumulative intensity function.
 */
double TotalWeibullCumulativeFunction::compute(const float * arr, const double time) const
{
	double cum = 0;
	for (int i = 1 ; i <= mode_cnt ; i++) {
		int alpha = getScaleIndex(node,i,node_cnt); 
		int beta = getShapeIndex(i,node_cnt);
		cum += getCumulative(exp(arr[alpha]),arr[beta],time);
	}
	return cum;
}

double TotalWeibullI1000Function::compute(const float * arr, const double time) const
{
	double cum = 0;
	for (int i = 1 ; i <= mode_cnt ; i++) {
		int alpha = getScaleIndex(node,i,node_cnt); 
		int beta = getShapeIndex(i,node_cnt);
		cum += getCumulative(exp(arr[alpha]),arr[beta],time);
	}
	return cum * 1000;
}

/**
 * Returns the reliability function.
 */
double TotalWeibullReliabilityFunction::compute(const float * arr, const double time) const
{
	double cum = 0;
	for (int i = 1 ; i <= mode_cnt ; i++) {
		int alpha = getScaleIndex(node,i,node_cnt); 
		int beta = getShapeIndex(i,node_cnt);
		cum += getCumulative(exp(arr[alpha]),arr[beta],time);
	}
	return exp(-cum);
}


/**
 * Returns the failure intensity function.
 */
double TotalWeibullIntensityFunction::compute(const float * arr, const double time) const
{
	double lambda = 0;
	for (int i = 1 ; i <= mode_cnt ; i++) {
		int alpha = getScaleIndex(node,i,node_cnt); 
		int beta = getShapeIndex(i,node_cnt);
		lambda += getIntensity(exp(arr[alpha]),arr[beta],time);
	}
	return lambda;
}