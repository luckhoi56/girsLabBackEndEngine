// $Id: WeibullTestDataOperator.h 169 2005-01-28 14:17:28Z saulo $

#pragma once

#include "../core/TestData.h"
#include "../core/TestConditions.h"
#include "../core/NetOperator.h"

/**
 * Operator for Weibull test data.
 * This operator computes the ln density for a Weibull model.
 * It computes the likelihood for a particular failure mode.
 * Terminations because of other failure modes are considered
 * to be suspensions.
 *
 * This operator does <b>not</b> consider weights assigned to 
 * individual records in the data set. This operator also does
 * not have the possibility of assigning design credits. 
 */
class WeibullTestDataOperator : public NetOperator
{
public:
	WeibullTestDataOperator(TestData * data, TestConditions * conditions, 
		const int node_cnt, const int mode_cnt, const int node, const int mode);

	~WeibullTestDataOperator(void);
	double compute(float * params, float * derivs) const;
	NetOperatorPtr copy() const;
	virtual bool appliesTo(const int index) const 
		{return alpha == index || beta == index;}
	virtual bool specifies(const int index) const
		{return false;}
	bool isDensity(const int index) const;

private:
	TestData * data;	// data set
	TestConditions * conditions; // test conditions
	int node_cnt;			// number of product nodes
	int mode_cnt;			// number of failure modes
	int alpha;
	int beta;
	int mode;			// failure mode index
	int node;			// product node index
	double weighted_failures;		// number of failures
	double weighted_sumlnt;		// sum of ln(t) for failed tests

	bool is_debug;
};


