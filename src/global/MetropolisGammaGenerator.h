// MetropolisGammaGenerator.h: interface for the MetropolisGammaGenerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METROPOLISGAMMAGENERATOR_H__2BDE07AA_4FBF_11D4_9894_002018557056__INCLUDED_)
#define AFX_METROPOLISGAMMAGENERATOR_H__2BDE07AA_4FBF_11D4_9894_002018557056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MetropolisGenerator.h"

/* Implementation of Metropolis sampler candidate generator.
 * This implementation draws multidimensional candidates from
 * gamma distributions with a median in each dimension equal
 * to the value of the prior sample, and a shape factor 2.
 *
 * MetropolisGammaGenerator is used by MetropolisSampler.
 */
class MetropolisGammaGenerator : public MetropolisGenerator  
{
public:

	bool increaseVariation();
	bool decreaseVariation();
	void setMinimumVariation();
	void setMaximumVariation();

	void init(int dimension);
	MetropolisGammaGenerator();
	virtual ~MetropolisGammaGenerator();

	/* Computes the probability density of transition from x to y */
	virtual double computeLogDensity(const Parameters & x, const Parameters & y);

	/* Generates a candidate, conditional on previous sample. */
	virtual Parameters generateCandidate(const Parameters & sample);
private:
	bool setDensityMode(int mode);

	/* Computes scale factor for a gamma distribution as function of
	 * previous sample value.
	 */
	double computeScale(int index, double sample);

	int	_densityMode;
	double _densityShape;
	double _lnDensityGamma;
	double _densityMedian;

};

#endif // !defined(AFX_METROPOLISGAMMAGENERATOR_H__2BDE07AA_4FBF_11D4_9894_002018557056__INCLUDED_)
