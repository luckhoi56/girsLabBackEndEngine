// $Id: JointDensityEstimate.cpp 352 2005-03-11 17:08:23Z saulo $

#include "jointdensityestimate.h"

JointDensityEstimate::JointDensityEstimate(OperatorFunction * f1, 
										   OperatorFunction * f2, 
										   WeibullSampleSet * sample_set,
										   int draw_points)

{
	// initialize grid
	this->draw_points = draw_points;
	int grid_size = draw_points * draw_points;
	// this->grid = new int[grid_size];
	this->grid = new double[grid_size];
	for (int i = 0 ; i < grid_size ; i++) this->grid[i] = 0;

	// determine bounds for x variable
	QuantileOperator * qxop = new SortingQuantileOperator(f1->clone());
	sample_set->execute(qxop);
	x_lo = qxop->getQuantile(qxop->Q01);
	x_hi = qxop->getQuantile(qxop->Q99);
	delete qxop;

	if (x_lo == x_hi) {
		double x_fixed = x_lo;
		x_lo = x_lo - ((x_fixed != 0) ? fabs(x_fixed) / 10 : 1);
		x_hi = x_hi + ((x_fixed != 0) ? fabs(x_fixed) / 10 : 1);
	}

	// determine bounds for y variable 
	QuantileOperator * qyop = new SortingQuantileOperator(f2->clone());
	sample_set->execute(qyop);
	y_lo = qyop->getQuantile(qyop->Q01);
	y_hi = qyop->getQuantile(qyop->Q99);
	delete qyop;

	if (y_lo == y_hi) {
		double y_fixed = y_lo;
		y_lo = y_lo - ((y_fixed != 0) ? fabs(y_fixed) / 10 : 1);
		y_hi = y_hi + ((y_fixed != 0) ? fabs(y_fixed) / 10 : 1);
	}

	// process the samples in the sample set
	x_func = f1;
	y_func = f2;
	sample_set->execute(this);
}

JointDensityEstimate::~JointDensityEstimate(void)
{
	delete [] grid;
	delete x_func;
	delete y_func;
}

/**
 * Processes the sample point.
 * Implementation of SampleOperator function, which is called for
 * each sample in the sample set. This function increases the 
 * appropriate cell in the grid.
 */
void JointDensityEstimate::process(const float * sample)
{
	double x = x_func->compute(sample);
	if (x <= x_lo || x >= x_hi) return;
	double y = y_func->compute(sample);
	if (y <= y_lo || y >= y_hi) return;

//	int x_index = (int)(draw_points * (x - x_lo) / (x_hi - x_lo));
//	int y_index = (int)(draw_points * (y - y_lo) / (y_hi - y_lo));
//	int cell_index = x_index * draw_points + y_index;
//	grid[cell_index]++;

	double xi = (draw_points - 1) * (x - x_lo) / (x_hi - x_lo);
	int fx = (int)floor(xi);
	int cx = (int)ceil(xi);
	double dx = xi - fx;

	double yi = (draw_points - 1) * (y - y_lo) / (y_hi - y_lo);
	int fy = (int) floor(yi);
	int cy = (int) ceil(yi);
	double dy = yi - fy;

	grid[fx * draw_points + fy] += (1 - dx) * (1 - dy);
	grid[fx * draw_points + cy] += (1 - dx) * dy;
	grid[cx * draw_points + fy] += dx * (1 - dy);
	grid[cx * draw_points + cy] += dx * dy;

}

/**
 * Returns the (unnormalized) density estimate at the given coordinate.
 * The method returns the density estimate, along with the x and y values.
 * The points are indexed based on the number of draw points specified when
 * the density estimate was created.
 * @param x_index index of x variable
 * @param y_index index of y variable
 * @param x x value at grid point
 * @param y y value at grid point
 * @param density density estimate at grid point
 */
int JointDensityEstimate::getEstimate(const int x_index, const int y_index, 
									  double & x, double & y, double & density)
{
//	x = (x_index + 0.5) * (x_hi - x_lo) / draw_points + x_lo;
//	y = (y_index + 0.5) * (y_hi - y_lo) / draw_points + y_lo;
	x = (x_index) * (x_hi - x_lo) / (draw_points - 1) + x_lo;
	y = (y_index) * (y_hi - y_lo) / (draw_points - 1) + y_lo;
	density = grid[x_index * draw_points + y_index];
	return 0;
}
