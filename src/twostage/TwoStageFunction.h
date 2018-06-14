// TwoStageFunction.h: interface for the TwoStageFunction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TWOSTAGEFUNCTION_H__C33B0F4F_1E6F_4C50_8243_2DB2286790FC__INCLUDED_)
#define AFX_TWOSTAGEFUNCTION_H__C33B0F4F_1E6F_4C50_8243_2DB2286790FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../global/Function.h"
#include "../nonhomogeneous/NHSession.h"
#include "../parametric/GammaDistribution.h"

#include <math.h>

class TwoStageFunction : public Function  
{
public:
	TwoStageFunction();
	virtual ~TwoStageFunction();
	virtual double evaluate(const Parameters & params) const
	{ double value = params.getValue(0); return log(prior->getMeasureMean(dflt,value,pdf)) + (likelihood ? likelihood->getLogPDF(value) : 0);}
	virtual int getDimension() const {return 1;}
	GammaDistribution * likelihood;
	NHSession * prior;
};

#endif // !defined(AFX_TWOSTAGEFUNCTION_H__C33B0F4F_1E6F_4C50_8243_2DB2286790FC__INCLUDED_)
