//
// File:      $Id: NHLikelihoodLogBinomial.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//

#pragma once
#include "NHLikelihood.h"

/**
 * Non-homogeneous Lognormal-Binomial-lognormal likelihood function.
 * This likelihood function computes the likelihood of a set of 
 * observations consisting of demand-based data and/or estimates, assuming
 * a Lognormal population variability distribution.
 */
class NHLikelihoodLogBinomial :
	public NHLikelihood
{
public:
	void preprocess(const NHObservation & observation);
    double getLogLikelihood(const Parameters & params, NHObservationIterator & iterator, int observations);
	NHLikelihoodLogBinomial(void);
	~NHLikelihoodLogBinomial(void);
};
