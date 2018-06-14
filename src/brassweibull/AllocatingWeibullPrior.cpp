// $Id: AllocatingWeibullPrior.cpp 169 2005-01-28 14:17:28Z saulo $

#include "AllocatingWeibullPrior.h"
#include <math.h>
#include "../core/NetOperator.h"
#include "../core/TimedMeasure.h"
#include "WeibullMeasureFunction.h"
#include "WeibullModelLayout.h"

AllocatingWeibullPrior::AllocatingWeibullPrior(void)
{
}

AllocatingWeibullPrior::~AllocatingWeibullPrior(void)
{
}

double AllocatingWeibullPrior::compute(float * params, float * derivs) const
{

	double ln_prior = 0;

	double lower = 0.01;
	double upper = 0.09;

	double mcum = log(lower * upper) / 2;
	double scum = 0.5 * log(upper / lower) / 1.645;
	if (scum == 0) return 0;
	double time = 100;
	if (time == 0) return 0;
	double ln_time = log(time);

	double lnd = 0;

	double total_cum = 0;
	double transform = 1;

	const int mode_cnt = 3;
	const int node = 0;
	const int node_cnt = 1;
	double cum[mode_cnt];
	double u[] = {3,2,1};

	for (int i = 1 ; i <= mode_cnt ; i++) 
	{
		double b = params[WeibullModelLayout::getShapeIndex(i,node_cnt)];
		double lna = params[WeibullModelLayout::getScaleIndex(node,i,node_cnt)];
		//double cum = exp(b * (ln_time - lna));
		cum[i] = exp(b * (ln_time - lna));
		total_cum += cum[i];
		transform *= b * cum[i];
	}

	double ln_dirichlet = 0;

	for (int i = 0 ; i < mode_cnt ; i++) {
		ln_dirichlet += (u[i] - 1) * log(cum[i] / total_cum); 
	}

	double ln_total_cum = log(total_cum);
	lnd = - pow((ln_total_cum - mcum) / scum,2) / 2 - mode_cnt * ln_total_cum;
	lnd += log(transform);

	lnd += ln_dirichlet;

	return lnd;
}