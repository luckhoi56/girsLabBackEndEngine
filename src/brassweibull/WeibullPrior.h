// $Id: WeibullPrior.h 1089 2005-08-08 16:31:42Z fgroen $

#pragma once
#include "../core/netoperator.h"
#include "../core/timedmeasure.h"
#include "weibullmeasurefunction.h"
#include "WeibullModelLayout.h"

/**
 * Prior for Weibull based models that allows prior to be specified in terms of cumulative failure rate.
 *
 * The current implementation takes the information in the first records of a TimedMeasure data 
 * structure, and uses the values in that record as estimated bounds on the cumulative failure rate.
 * The failure rate concerns either a single failure rate, or the sum of all failure rates.
 *
 * Mathematical details of the implementation are described in the report 'he Specification of 
 * Prior Distributions in BRASS'. 
 * 
 */
class WeibullPrior :
	public NetOperator
{
public:
	WeibullPrior(TimedMeasure * data, const int node, const int mode, const int node_cnt, const int mode_cnt);
	WeibullPrior(const WeibullPrior & prior);
	~WeibullPrior(void);

	double compute(float * params, float * derivs) const;
	NetOperatorPtr copy() const;
	virtual bool appliesTo(const int index) const ;
	virtual bool specifies(const int index) const
		{return false;}
	bool isDensity(const int index) const;

protected:
	// identifier for any failure mode.
	const static int MODE_ANY = -1;

	double computeI1000(float * params) const;
	double computeReliability(float * params) const;

	WeibullMeasureFunction * createMeasureFunction(const int mode, const int measure, 
														   const int node);

	int node_cnt;
	int mode_cnt;
	int node;
	int mode;
	TimedMeasure * prior;
	WeibullMeasureFunction * func;

};
