// ThreeDeeFunction.h: interface for the ThreeDeeFunction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREEDEEFUNCTION_H__931EFA83_D4C0_499A_8EBB_76D8B799A419__INCLUDED_)
#define AFX_THREEDEEFUNCTION_H__931EFA83_D4C0_499A_8EBB_76D8B799A419__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../parametric/GammaDistribution.h"
#include "../nonhomogeneous/NHInterpretation.h"
#include "../dcdf/DCDFLIBInterface.h"
#include "../nonhomogeneous/NHObservation.h"
#include "../global/Function.h"

class ThreeDeeFunction : public Function  
{
public:
	ThreeDeeFunction();
	virtual ~ThreeDeeFunction();

	virtual double evaluate(const Parameters & params) const;
	virtual int getDimension() const {return 3;}

	GammaDistribution * homogeneous;
	NHInterpretation * nonhomogeneous;
};

#endif // !defined(AFX_THREEDEEFUNCTION_H__931EFA83_D4C0_499A_8EBB_76D8B799A419__INCLUDED_)
