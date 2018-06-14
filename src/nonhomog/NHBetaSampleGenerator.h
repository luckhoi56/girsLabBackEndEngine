//
// File:      $Id: NHBetaSampleGenerator.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//
 

#if !defined(AFX_NHBETASAMPLEGENERATOR_H__6493FE79_42D6_4A0B_9301_1F69AE4A8B01__INCLUDED_)
#define AFX_NHBETASAMPLEGENERATOR_H__6493FE79_42D6_4A0B_9301_1F69AE4A8B01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MetropolisGenerator.h"
#include "Parameters.h"


/**
 * Sample generator that generates samples for the Beta alpha/beta space.
 * 
 * Samples are taken from lognormal distributions over variables x and y,
 * where
 *   x = a + b
 *   y = a / b
 * The reverse transformation is given by
 *   a = xy / (1 + y)
 *   b = x / (1 + y)
 * The Jacobian is given by
 *
 *   J = (a + b) / b^2
 * 
 * x represents the total number of observed experiments. y is an 
 * approximation to the mean, that is better for lower values of a/b.
 * Samples are returned as a and b.
 */
class NHBetaSampleGenerator : public MetropolisGenerator
{
public:

	static void test();
	NHBetaSampleGenerator();
	virtual ~NHBetaSampleGenerator();

	virtual bool increaseVariation();
	virtual bool decreaseVariation();
	virtual void setMinimumVariation();
	virtual void setMaximumVariation();

	virtual void init(int dimension);

	/* Computes transition density from x to y. */
	virtual double computeLogDensity(const Parameters & x, const Parameters & y);

	/* Generates a candidate sample, conditional on previous sample. */
	virtual Parameters generateCandidate(const Parameters & sample);

private:

	int b_var_mode;
	int ab_var_mode;

	static double b_var_modes [];
	static double ab_var_modes [];
};

#endif // !defined(AFX_NHBETASAMPLEGENERATOR_H__6493FE79_42D6_4A0B_9301_1F69AE4A8B01__INCLUDED_)
