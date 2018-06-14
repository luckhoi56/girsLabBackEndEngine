// MetropolisLNGenerator.h: interface for the MetropolisLNGenerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METROPOLISLNGENERATOR_H__B664C148_CC00_4903_A69D_13E072050E0A__INCLUDED_)
#define AFX_METROPOLISLNGENERATOR_H__B664C148_CC00_4903_A69D_13E072050E0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MetropolisGenerator.h"

class MetropolisLNGenerator : public MetropolisGenerator  
{
public:
	MetropolisLNGenerator();
	virtual ~MetropolisLNGenerator();

	bool increaseVariation();
	bool decreaseVariation();
	void setMinimumVariation();
	void setMaximumVariation();
	void init(int dimension);

	/* Computes the probability density of transition from x to y */
	virtual double computeLogDensity(const Parameters & x, const Parameters & y);

	/* Generates a candidate, conditional on previous sample. */
	virtual Parameters generateCandidate(const Parameters & sample);
private:
	bool setDensityMode(int mode);
	int	_densityMode;
	double _densityTau;
};

#endif // !defined(AFX_METROPOLISLNGENERATOR_H__B664C148_CC00_4903_A69D_13E072050E0A__INCLUDED_)
