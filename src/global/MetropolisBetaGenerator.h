// MetropolisBetaGenerator.h: interface for the MetropolisBetaGenerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METROPOLISBETAGENERATOR_H__06D180D0_4352_4318_ABBB_505078F3E052__INCLUDED_)
#define AFX_METROPOLISBETAGENERATOR_H__06D180D0_4352_4318_ABBB_505078F3E052__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MetropolisGenerator.h"

class MetropolisBetaGenerator : public MetropolisGenerator  
{
public:
	MetropolisBetaGenerator();
	virtual ~MetropolisBetaGenerator() {}
	bool increaseVariation();
	bool decreaseVariation();
	void setMinimumVariation();
	void setMaximumVariation();
	void init(int dimension);
	virtual double computeLogDensity(const Parameters & x, const Parameters & y);
	virtual Parameters generateCandidate(const Parameters & sample);
private:
	double _densityPopulation;
};

#endif // !defined(AFX_METROPOLISBETAGENERATOR_H__06D180D0_4352_4318_ABBB_505078F3E052__INCLUDED_)
