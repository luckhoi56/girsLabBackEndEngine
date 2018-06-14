// $Id: WeibullGroupedDataOperator.h 352 2005-03-11 17:08:23Z saulo $

#pragma once

#include "../core/GroupedData.h"
#include "../core/NetOperator.h"

/**
 * Likelihood operator for Weibull grouped data.
 */
class WeibullGroupedDataOperator : public NetOperator
{
public:
	WeibullGroupedDataOperator(GroupedData * data, const int node_cnt, 
		const int mode_cnt, const int node, const int mode);
	~WeibullGroupedDataOperator(void);

	double compute(float * params, float * derivs) const;
	NetOperatorPtr copy() const;
	virtual bool appliesTo(const int index) const;
	virtual bool specifies(const int index) const
		{return false;}
	bool isDensity(const int index) const;

private:
	GroupedData * data;		// data set
	int node_cnt;			// number of product nodes
	int mode_cnt;			// number of failure modes
	int alpha;
	int beta;
	int mode;				// failure mode index
	int node;				// product node index

	double * cum_arr;		// array of cumulative intensity values

	double weighted_failures;	// number of failures
	double weighted_sumlnt;		// sum of ln(t) for failed tests
};
