//
// File:      $Id: NHLikelihoodLogPoisson.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//

#if !defined(AFX_NHLIKELIHOODLOGPOISSON_H__D516118C_EAC9_4B0E_9B9D_222892A37E13__INCLUDED_)
#define AFX_NHLIKELIHOODLOGPOISSON_H__D516118C_EAC9_4B0E_9B9D_222892A37E13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NHLikelihood.h"
#include "NHObservation.h"
#include "NHLogPoissonSupport.h"
#include "NHModel.h"
#include "../dcdf/DCDFLIBInterface.h"

#include <math.h>

/**
 * Non-homogeneous lognormal-poisson-lognormal likelihood function.
 * This likelihood function computes the likelihood of a set of 
 * observations consisting of run-time data and/or estimates, assuming
 * a lognormal population variability distribution.
 */
class NHLikelihoodLogPoisson : public NHLikelihood  
{
public:
	double getLogLikelihood(const Parameters & params, NHObservationIterator & iterator, int observations);
	void preprocess(const NHObservation &observation);
	NHLikelihoodLogPoisson();
	virtual ~NHLikelihoodLogPoisson();
};

#endif // !defined(AFX_NHLIKELIHOODLOGPOISSON_H__D516118C_EAC9_4B0E_9B9D_222892A37E13__INCLUDED_)
