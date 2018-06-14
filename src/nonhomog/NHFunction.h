//
// File:      $Id: NHFunction.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//
 

#if !defined(AFX_NHFUNCTION_H__1B5ABE67_F813_4339_84A9_65F68627550D__INCLUDED_)
#define AFX_NHFUNCTION_H__1B5ABE67_F813_4339_84A9_65F68627550D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Function.h"
#include "NHPrior.h"
#include "Interpretation.h"
#include "NHInterpretation.h"

/**
 * Class used to encapsulate likelihood / prior during
 * Metropolis sampling procedure.
 */
class NHFunction : public Function  
{
public:
	double evaluate(const Parameters & params) const;
	int getDimension() const {return 2;}

	void setLogLikeMode(Parameters * params);
	NHFunction(NHPrior * prior, Interpretation * interpretation);
	virtual ~NHFunction();
private:
	NHPrior * prior;
	Interpretation * interpretation;
};

#endif // !defined(AFX_NHFUNCTION_H__1B5ABE67_F813_4339_84A9_65F68627550D__INCLUDED_)
