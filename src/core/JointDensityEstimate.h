// $Id: JointDensityEstimate.h 169 2005-01-28 14:17:28Z saulo $

#pragma once

#include "WeibullSampleSet.h"


/**
 * Joint density estimator.
 *
 * The class builds a generic estimate of the joint density estimate over 
 * two variables, based on a sample set. The variables are defined by function
 * objects, which are instances of OperatorFunction. Therefore, the estimator 
 * is generic, and can be used to construct joint densities over any two variables.
 *
 * In the current implementation, the density estimate consists of the 
 * number of samples encountered in a cell of the grid.
 */
class JointDensityEstimate : SampleOperator
{
public:
	JointDensityEstimate(OperatorFunction * f1, OperatorFunction * f2, WeibullSampleSet * sample_set, int draw_points);
	~JointDensityEstimate(void);
	int getEstimate(const int x_index, const int y_index, double & x, double & y, double & density);
	void process(const float * sample);
	/// Returns true if samples should be processed in random order.
	bool randomOrder() const {return false;}

private:
	// int * grid;
	double * grid;
	int draw_points;
	OperatorFunction * x_func;
	OperatorFunction * y_func;
	double x_lo;
	double x_hi;
	double y_lo;
	double y_hi;
};
