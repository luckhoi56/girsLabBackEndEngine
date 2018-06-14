//
// File:      $Id: NHLikelihoodGammaPoisson.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//
 
#if !defined(AFX_NHLIKELIHOODGAMMAPOISSON_H__5CC88F56_7B36_4723_83DA_9EC35052CF8F__INCLUDED_)
#define AFX_NHLIKELIHOODGAMMAPOISSON_H__5CC88F56_7B36_4723_83DA_9EC35052CF8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NHLogPoissonSupport.h"
#include "NHLikelihood.h"
#include "NHObservation.h"
#include "NHModel.h"
#include "../dcdf/DCDFLIBInterface.h"

#include <math.h>

/**
 * Non-homogeneous Gamma-poisson-lognormal likelihood function.
 * This likelihood function computes the likelihood of a set of 
 * observations consisting of run-time data and/or estimates, assuming
 * a Gamma population variability distribution.
 */
class NHLikelihoodGammaPoisson : public NHLikelihood  
{
public:
	void preprocess(const NHObservation & observation);
    double getLogLikelihood(const Parameters & params, NHObservationIterator & iterator, int observations);
	NHLikelihoodGammaPoisson();
	virtual ~NHLikelihoodGammaPoisson();
};

#endif // !defined(AFX_NHLIKELIHOODGAMMAPOISSON_H__5CC88F56_7B36_4723_83DA_9EC35052CF8F__INCLUDED_)
