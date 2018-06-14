//
// File:      $Id: NHPriorLogBinomial.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//
 
#pragma once
#include "nhprior.h"

class NHPriorLogBinomial : public NHPrior
{
public:
	NHPriorLogBinomial(void);
	NHPriorLogBinomial(double md_nu, double md_tau, double ef_nu, double ef_tau);
	~NHPriorLogBinomial(void);
	double getLogPrior(const Parameters &params);
private:
	double md_nu;
	double md_tau;
	double ef_nu;
	double ef_tau;
};
