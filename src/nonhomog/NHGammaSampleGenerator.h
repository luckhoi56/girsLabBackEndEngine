//
// File:      $Id: NHGammaSampleGenerator.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//
 

#if !defined(AFX_NHGAMMASAMPLEGENERATOR_H__C76E7731_CE1A_49B2_8F3B_53D33425399B__INCLUDED_)
#define AFX_NHGAMMASAMPLEGENERATOR_H__C76E7731_CE1A_49B2_8F3B_53D33425399B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MetropolisGenerator.h"
#include "Parameters.h"

/*
 * MH Sample Generator intended for use in the NH Gamma problem.
 *
 * The sampler draws samples from a distribution p(x,y) = LN(x) LN(y)
 * where 
 *     x = 1 / b
 *     y = a / b
 * or
 *     x = b
 *     y = a * b
 * depending on the setting of A_DIV_B_MEAN in the CPP file (true -> x = 1/b).
 * The choice of which version used depends on the formulation of the Gamma
 * distribution, particularly the scale factor. The first parameterization 
 * corresponds to the 'lambda' case where mean of the Gamma is 1 / b.
 *
 * However, candidates returned are parameterized as (alpha,beta), and also
 * the density returned by computeLogDensity is in terms of alpha and beta.
 */
class NHGammaSampleGenerator : public MetropolisGenerator  
{
public:
	static void test();
	NHGammaSampleGenerator();
	virtual ~NHGammaSampleGenerator();

	virtual bool increaseVariation();
	virtual bool decreaseVariation();
	virtual void setMinimumVariation();
	virtual void setMaximumVariation();

	virtual void init(int dimension);

	/// Computes transition density from x to y.
	virtual double computeLogDensity(const Parameters & x, const Parameters & y);

	/// Generates a candidate sample, conditional on previous sample.
	virtual Parameters generateCandidate(const Parameters & sample);

private:

	int b_var_mode;
	int ab_var_mode;

	static double b_var_modes [];
	static double ab_var_modes [];

};

#endif // !defined(AFX_NHGAMMASAMPLEGENERATOR_H__C76E7731_CE1A_49B2_8F3B_53D33425399B__INCLUDED_)
