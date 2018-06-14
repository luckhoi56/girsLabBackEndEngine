//
// File:      $Id: NHLogBinomialSupport.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//

#pragma once

#include "function.h"

/** 
 * Support class for lognormal-binomial-lognormal and
 * beta-binomial-lognormal nonhomogeneous models.
 * This class provides integration routines for 
 * non conjugate cases. Integration takes place over
 * the expression 
 *
 * x^(a-1) * (1-x)^(b-1) * Exp[-(1/2)*((Log[x]-nu)/tau)^2]
 * 
 * which contains all terms dependent on x in any log-bin-log
 * or bet-bin-log case. The integration procedure locates the mode,
 * seeks out appropriate bounds, and performs the integration.
 */
class NHLogBinomialSupport : public Function
{
public:
	NHLogBinomialSupport(void);
	~NHLogBinomialSupport(void);

	double integrate(double nu, double tau, double a, double b, bool & fail);
	double evaluate(const Parameters & params) const;
	double evaluate(double x) const;
	virtual int getDimension() const {return 1;}

private: 

	double evaluate_ln(double x) const;
	void determineBounds(double &lowerBound, double &upperBound, double & mode);
	double findMode() const; 

	double alpha;
	double beta;
	double nu;		// mean of underlying normal
	double tau;		// sdev of underlying normal
	double ln_norm;

};
