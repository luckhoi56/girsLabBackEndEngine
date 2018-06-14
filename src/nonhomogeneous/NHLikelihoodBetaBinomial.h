//
// File:      $Id: NHLikelihoodBetaBinomial.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//

#if !defined(AFX_NHLIKELIHOODBETABINOMIAL1_H__E064BBE4_2EB2_4FFF_BE01_E5B397F9D3B3__INCLUDED_)
#define AFX_NHLIKELIHOODBETABINOMIAL1_H__E064BBE4_2EB2_4FFF_BE01_E5B397F9D3B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NHLikelihood.h"
#include "NHObservation.h"

/**
 * Non-homogeneous Beta-Binomial-lognormal likelihood function.
 * This likelihood function computes the likelihood of a set of 
 * observations consisting of demand-based data and/or estimates, assuming
 * a Beta population variability distribution.
 */
class NHLikelihoodBetaBinomial : public NHLikelihood  
{
public:
	void preprocess(const NHObservation & observation);
    double getLogLikelihood(const Parameters & params, NHObservationIterator & iterator, int observations);
	NHLikelihoodBetaBinomial();
	virtual ~NHLikelihoodBetaBinomial();

};

#endif // !defined(AFX_NHLIKELIHOODBETABINOMIAL1_H__E064BBE4_2EB2_4FFF_BE01_E5B397F9D3B3__INCLUDED_)
