// MetropolisTLNGenerator.h: interface for the MetropolisTLNGenerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METROPOLISTLNGENERATOR_H__E8D6DDC9_76CF_4212_BFF6_7CEC1022FDB7__INCLUDED_)
#define AFX_METROPOLISTLNGENERATOR_H__E8D6DDC9_76CF_4212_BFF6_7CEC1022FDB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MetropolisGenerator.h"

class MetropolisTLNGenerator : public MetropolisGenerator  
{
public:
	MetropolisTLNGenerator();
	virtual ~MetropolisTLNGenerator();
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

#endif // !defined(AFX_METROPOLISTLNGENERATOR_H__E8D6DDC9_76CF_4212_BFF6_7CEC1022FDB7__INCLUDED_)
