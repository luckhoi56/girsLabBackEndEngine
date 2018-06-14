// $Id: WeibullPrior.cpp 1089 2005-08-08 16:31:42Z fgroen $

#include "WeibullPrior.h"
#include <math.h>

/**
 * Constructor.
 * @param data custom prior data
 * @param node product node to which prior applies
 * @param mode failure mode to which prior applies
 * @param node_cnt number of nodes
 * @param mode_cnt number of failure modes.
 */
WeibullPrior::WeibullPrior(TimedMeasure * data, const int node, const int mode, const int node_cnt, const int mode_cnt)
{
	this->prior = data;
	if (data != 0) data->obtain();
	this->node = node;
	this->mode = mode;
	this->node_cnt = node_cnt;
	this->mode_cnt = mode_cnt;
	this->func = 0;
}

/**
 * Copy constructor.
 */
WeibullPrior::WeibullPrior(const WeibullPrior & prior) 
{
	this->prior = prior.prior;
	if (this->prior) this->prior->obtain();
	this->node = prior.node;
	this->mode = prior.mode;
	this->node_cnt = prior.node_cnt;
	this->mode_cnt = prior.mode_cnt;

	if (prior.func != 0) 
		func = (WeibullMeasureFunction*)prior.func->clone(); 
	else 
		func = 0;
}

WeibullPrior::~WeibullPrior(void)
{
	if (prior->release()) delete prior;
}

NetOperatorPtr WeibullPrior::copy() const {
	WeibullPrior * op = new WeibullPrior(this->prior,this->node,this->mode,
		this->node_cnt,this->mode_cnt);
	op->operator_id = this->operator_id;
	op->next = 0;
	return op;
}

bool WeibullPrior::appliesTo(const int index) const 
{
	if (this->mode > 0) {
		return 
			(index == WeibullModelLayout::getScaleIndex(this->node,this->mode,this->node_cnt)) ||
			(index == WeibullModelLayout::getShapeIndex(this->mode,this->node_cnt));
	} else {
		return 
			(WeibullModelLayout::isScaleParameter(index,this->node,this->node_cnt)) ||
			(WeibullModelLayout::isShapeParameter(index,this->node_cnt));
	}
}

/**
 * Computes the prior density function.
 * This implementation only considers the first record in the prior, and only
 * if the entry is valid.
 */
double WeibullPrior::compute(float * params, float * derivs) const
{
	if (this->prior->getMeasure() == prior->MEASURE_IPER1000)
	{
		return computeI1000(params);
	}
	else
	{
		return computeReliability(params);
	}
}

double WeibullPrior::computeI1000(float * params) const
{
	const double ln1000 = log(1000.0);

	double ln_prior = 0;
	const MeasureRecord * rec = this->prior->getRecord(0);
	if (rec->lower == 0 || rec->upper == 0) return 0;

	double mcum = log(rec->lower * rec->upper) / 2 - ln1000;

	double scum = 0.5 * log(rec->upper / rec->lower) / 1.645;
	double time = rec->time;
	if (time == 0) return 0;
	double ln_time = log(time);
	double lnd = 0;
	if (this->mode > 0) 
	{
		double lna = params[WeibullModelLayout::getScaleIndex(this->node,this->mode,this->node_cnt)];
		double b = params[WeibullModelLayout::getShapeIndex(this->mode,this->node_cnt)];
		double lnb = log(b);
		// implementation 1: transformation of p(lncum,b) to p(lna,b)
		// this implementation gives good results for direct sampling
		// double lncum = b * (log(100) - lna);
		// also works under rate/time adjustments with fixed/variable adjustments
		double lncum = b * (log(time) - lna);
		if (scum > 0) 
		{
			// lognormal distribution over the cumulative
			lnd = lnb - pow((lncum - mcum) / scum,2) / 2;
		}
		else
		{
			// =================================================
			// lower limit on cum rate: drop-off at mcum.
			//lnd = lnb;
			//double ln_cum_rat = lncum - mcum;
			//lnd += ln_cum_rat < 0 ? 1000 * ln_cum_rat : 0;
			// =================================================
			// if lower == upper, always switch to R90/C90 prior
			// beta prior over the reliability
			// multiply by 1000 to overcome i/1000 - cum transformation
	
			double cum = exp(lncum);
			double lnr = mcum + ln1000;
			// default to R90/C90 for invalid input
			if (lnr >= -0.0001) lnr = -0.10536;

			// find n such that CDF(r) = 0.9:
			// n = ln(1 - 0.9) / ln(r)
			double n = log(0.1) / lnr;
			// prior over R: Beta(n,1)
			lnd = -(n - 1) * cum;
			// transform to prior over cumulative
			lnd -= cum;
			// transform to prior over ln(alpha)
			lnd += lnb + lncum;
		}
	} 
	else 
	{
		double total_cum = 0;
		double transform = 1;
		double lntransform = 0;
		for (int i = 1 ; i <= mode_cnt ; i++) 
		{
			double b = params[WeibullModelLayout::getShapeIndex(i,this->node_cnt)];
			double lna = params[WeibullModelLayout::getScaleIndex(this->node,i,this->node_cnt)];
			double cum = exp(b * (ln_time - lna));
			total_cum += cum;
			//transform *= b * cum;
			lntransform += log(b * cum);
		}
		double ln_total_cum = log(total_cum);
		
		if (scum > 0)
		{
			lnd = - pow((ln_total_cum - mcum) / scum,2) / 2 - mode_cnt * ln_total_cum;
			// lnd += log(transform);
			lnd += lntransform;
		}
		else
		{
			double lnr = mcum + ln1000;
			// default to R90/C90 for invalid input
			if (lnr >= -0.0001) lnr = -0.10536;
			// find n such that CDF(r) = 0.9:
			// n = ln(1 - 0.9) / ln(r)
			double n = log(0.1) / lnr;

			// if lower == upper, always switch to R90/C90 prior
			// beta prior over the reliability
			lnd = -(n - 1) * total_cum;
			// transform to prior over cumulative
			lnd -= total_cum;
			// transform to prior over ln(alpha)
			lnd += lntransform;
			// distribute over failure modes (Dirichlet)
			lnd -= (mode_cnt - 1) * ln_total_cum;
		}
	}
	return lnd;
}

double WeibullPrior::computeReliability(float * params) const
{
	double ln_prior = 0;
	const MeasureRecord * rec = this->prior->getRecord(0);

	// lower = reliability
	// upper = confidence

	if (rec->lower <= 0 || rec->upper <= 0) return 0;
	if (rec->lower >= 1 || rec->upper >= 1) return 0;
	double time = rec->time;
	if (time == 0) return 0;
	double ln_time = log(time);

	double lnd = 0;
	if (this->mode > 0) 
	{
		double lna = params[WeibullModelLayout::getScaleIndex(this->node,this->mode,this->node_cnt)];
		double b = params[WeibullModelLayout::getShapeIndex(this->mode,this->node_cnt)];
		double lnb = log(b);
		// implementation 1: transformation of p(lncum,b) to p(lna,b)
		// this implementation gives good results for direct sampling
		// double lncum = b * (log(100) - lna);
		// also works under rate/time adjustments with fixed/variable adjustments
		double lncum = b * (log(time) - lna);
		double cum = exp(lncum);

		double lnr = log(rec->lower);
		double lnc = log(1 - rec->upper);

		// default to R90/C90 for invalid input
		if (lnr >= -0.0001) lnr = -0.10536;

		// find n such that CDF(r) = c:
		// n = ln(1 - c) / ln(r)
		double n = lnc / lnr;
		// prior over R: Beta(n,1)
		lnd = -(n - 1) * cum;
		// transform to prior over cumulative
		lnd -= cum;
		// transform to prior over ln(alpha)
		lnd += lnb + lncum;
	} 
	else 
	{
		double total_cum = 0;
		double transform = 1;
		double lntransform = 0;
		for (int i = 1 ; i <= mode_cnt ; i++) 
		{
			double b = params[WeibullModelLayout::getShapeIndex(i,this->node_cnt)];
			double lna = params[WeibullModelLayout::getScaleIndex(this->node,i,this->node_cnt)];
			double cum = exp(b * (ln_time - lna));
			total_cum += cum;
			lntransform += log(b * cum);
		}
		double ln_total_cum = log(total_cum);	
		double lnr = log(rec->lower);
		double lnc = log(1 - rec->upper);
		// default to R90/C90 for invalid input
		if (lnr >= -0.0001) lnr = -0.10536;
		// find n such that CDF(r) = 0.9:
		// n = ln(1 - 0.9) / ln(r)
		double n = lnc / lnr;

		// if lower == upper, always switch to R90/C90 prior
		// beta prior over the reliability
		lnd = -(n - 1) * total_cum;
		// transform to prior over cumulative
		lnd -= total_cum;
		// transform to prior over ln(alpha)
		lnd += lntransform;
		// distribute over failure modes (Dirichlet)
		lnd -= (mode_cnt - 1) * ln_total_cum;
	}
	return lnd;
}




bool WeibullPrior::isDensity(const int index) const
{
	if (this->appliesTo(index)) return true;
	if (next != 0) return next->isDensity(index);
	return false;
}