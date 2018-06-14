//
// File:      $Id: NHPriorBetaBinomial.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//

#if !defined(AFX_NHPRIORBETABINOMIAL_H__2F45F345_4EE6_4F4A_A08B_607D516899A7__INCLUDED_)
#define AFX_NHPRIORBETABINOMIAL_H__2F45F345_4EE6_4F4A_A08B_607D516899A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NHPrior.h"

class NHPriorBetaBinomial : public NHPrior  
{
public:
	double getLogPrior(const Parameters &params);
	NHPriorBetaBinomial();
	virtual ~NHPriorBetaBinomial();
	NHPriorBetaBinomial(double mn_nu, double mn_tau, double se_nu, double se_tau);
private:
	double mn_nu;
	double mn_tau;
	double se_nu;
	double se_tau;

};

#endif // !defined(AFX_NHPRIORBETABINOMIAL_H__2F45F345_4EE6_4F4A_A08B_607D516899A7__INCLUDED_)
