//
// File:      $Id: NHPriorGammaPoisson.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//
 

#if !defined(AFX_NHPRIORGAMMAPOISSON_H__7243264F_1478_4D30_8926_6ACB5225BF10__INCLUDED_)
#define AFX_NHPRIORGAMMAPOISSON_H__7243264F_1478_4D30_8926_6ACB5225BF10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NHPrior.h"

class NHPriorGammaPoisson : public NHPrior  
{
public:
	double getLogPrior(const Parameters &params);
	NHPriorGammaPoisson();
	NHPriorGammaPoisson(double mn_nu, double mn_tau, double se_nu, double se_tau);
	virtual ~NHPriorGammaPoisson();
private:
	double mn_nu;
	double mn_tau;
	double se_nu;
	double se_tau;
};

#endif // !defined(AFX_NHPRIORGAMMAPOISSON_H__7243264F_1478_4D30_8926_6ACB5225BF10__INCLUDED_)
