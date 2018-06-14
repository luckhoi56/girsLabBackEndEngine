//
// File:      $Id: NHPrior.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//
 

#if !defined(AFX_NHPRIOR_H__71386731_0CDD_42ED_919C_A4285DAC57F3__INCLUDED_)
#define AFX_NHPRIOR_H__71386731_0CDD_42ED_919C_A4285DAC57F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Parameters.h"

/* Prior for non-homogeneous models. This is a super-class for 
 * prior distributions to be used when the non-homogeneous models
 * are sampled from.
 *
 * It is advisable that this not be a flat prior: in any case, the
 * prior should impose some limits on the parameter space in which
 * the sampling is to take place. This can be achieved by returning
 * a value of 0 within the sample area, and a value of (for instance)
 * -2000 outside.
 *
 * This strategy is important in case of Metropolis sampling: the
 * strongly reduced probability outside the sample region will make
 * it practically impossible for the sampling algorithm to accept 
 * values outside the region.
 */
class NHPrior
{
public:
	virtual double getLogPrior(const Parameters &params) = 0;
	NHPrior();
	virtual ~NHPrior();
};

#endif // !defined(AFX_NHPRIOR_H__71386731_0CDD_42ED_919C_A4285DAC57F3__INCLUDED_)
