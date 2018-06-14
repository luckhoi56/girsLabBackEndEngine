//
// File:      $Id: NHPriorLogNormalMedianEF.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//
 

#if !defined(AFX_NHPRIORLOGNORMALMEDIANEF_H__BD6EF0A2_43CE_474A_A92B_B700C0FB28AF__INCLUDED_)
#define AFX_NHPRIORLOGNORMALMEDIANEF_H__BD6EF0A2_43CE_474A_A92B_B700C0FB28AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NHPrior.h"

class NHPriorLogNormalMedianEF : public NHPrior  
{
public:
	double getLogPrior(const Parameters &params);
	NHPriorLogNormalMedianEF();
	NHPriorLogNormalMedianEF(double md_nu, double md_tau, double ef_nu, double ef_tau);
	virtual ~NHPriorLogNormalMedianEF();
private:
	double md_nu;
	double md_tau;
	double ef_nu;
	double ef_tau;
};

#endif 
