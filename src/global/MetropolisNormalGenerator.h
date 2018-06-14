// MetropolisNormalGenerator.h: interface for the MetropolisNormalGenerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METROPOLISNORMALGENERATOR_H__0EDE8C21_CE8B_4084_930E_B0CCB24165F2__INCLUDED_)
#define AFX_METROPOLISNORMALGENERATOR_H__0EDE8C21_CE8B_4084_930E_B0CCB24165F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MetropolisGenerator.h"

class MetropolisNormalGenerator : public MetropolisGenerator  
{
public:
	MetropolisNormalGenerator();
	virtual ~MetropolisNormalGenerator();
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

#endif // !defined(AFX_METROPOLISNORMALGENERATOR_H__0EDE8C21_CE8B_4084_930E_B0CCB24165F2__INCLUDED_)
