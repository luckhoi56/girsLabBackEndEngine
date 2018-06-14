//
// File:      $Id: NHLogPoissonSupport.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//

#if !defined(AFX_NHLOGPOISSONSUPPORT_H__E0D103BA_1C9C_4844_91AD_D70DCFACC394__INCLUDED_)
#define AFX_NHLOGPOISSONSUPPORT_H__E0D103BA_1C9C_4844_91AD_D70DCFACC394__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../global/Function.h"

/** 
 * Support class for lognormal-poisson-lognormal and
 * gamma-poisson-lognormal nonhomogeneous models.
 * This class provides integration routines for 
 * non conjugate cases.
 */
class NHLogPoissonSupport : public Function 
{
public:

	double integrate(double nu, double tau, double a, double b, bool & fail);
	double evaluate(const Parameters & params) const;
	double evaluate(double x) const;
	virtual int getDimension() const {return 1;}

	NHLogPoissonSupport();
	virtual ~NHLogPoissonSupport();

	long bounds_cycles;
	long integr_cycles;

private: 

	double evaluate_ln(double x) const;
	void determineBounds(double &lowerBound, double &upperBound, double & mode);

	double alpha;
	double beta;
	double nu;		// mean of underlying normal
	double tau;		// sdev of underlying normal
	double ln_norm;
};

#endif // !defined(AFX_NHLOGPOISSONSUPPORT_H__E0D103BA_1C9C_4844_91AD_D70DCFACC394__INCLUDED_)
