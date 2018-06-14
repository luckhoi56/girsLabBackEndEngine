// MetropolisGenerator.h: interface for the MetropolisGenerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METROPOLISGENERATOR_H__2BDE07A9_4FBF_11D4_9894_002018557056__INCLUDED_)
#define AFX_METROPOLISGENERATOR_H__2BDE07A9_4FBF_11D4_9894_002018557056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Parameters.h"

/* Super class for Metropolis sampling candidate generators. Generators
 * support two functions:
 *  - generation of the candidate, conditional on the previous sample.
 *  - computation of transition density, conditional on the previous sample.
 * These two results are used by the MetropolisSampler to generate candidates
 * and to decide on acceptance or rejection of the candidate.
 */
class MetropolisGenerator  
{
public:

	MetropolisGenerator();
	virtual ~MetropolisGenerator();

	virtual bool increaseVariation() = 0;
	virtual bool decreaseVariation() = 0;
	virtual void setMinimumVariation() = 0;
	virtual void setMaximumVariation() = 0;

	virtual void init(int dimension) = 0;

	/* Computes transition density from x to y. */
	virtual double computeLogDensity(const Parameters & x, const Parameters & y) = 0;

	/* Generates a candidate sample, conditional on previous sample. */
	virtual Parameters generateCandidate(const Parameters & sample) = 0;
};

#endif // !defined(AFX_METROPOLISGENERATOR_H__2BDE07A9_4FBF_11D4_9894_002018557056__INCLUDED_)
