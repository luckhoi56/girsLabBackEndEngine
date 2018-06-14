//
// File:      $Id: NHLikelihood.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//

#if !defined(AFX_NHLIKELIHOOD_H__072E8BE4_B3D7_4223_B856_003AF3FA7D90__INCLUDED_)
#define AFX_NHLIKELIHOOD_H__072E8BE4_B3D7_4223_B856_003AF3FA7D90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NHObservation.h"
#include "../global/Parameters.h"

/**
 * Super class for non-homogeneous likelihood classes. The likelihood
 * classes are responsible for computing the likelihood of observations.
 * 
 * Subclasses must implement two methods: getLogLikelihood() and preprocess().
 * The preprocess() method is called when an observation is added to an 
 * interpretation. It is used to compute any statistics that do not depend
 * on the parameters for which to compute the likelihood up front.
 *
 * The getLogLikelihood() method is called when the actual sampling is done.
 * This method gets passed an NHObservationIterator, which is a STL iterator.
 * The observations parameter indicates how many observations are contained
 * in the list to which the iterator refers. Dereference the iterator (*iterator)
 * to get to the actual NHObservation object. Call iterator++ to advance the
 * iterator. A template for this method would therefore be
 *
 * double [class]::getLogLikelihood(...)
 * {
 *		value1 = params.getValue(0);
 *		value2 = params.getValue(1);
 *		double like = 0;
 *		for (int i = 0 ; i++ ; i < observations) {
 *			like += [function of values in (*iterator).field1 and (*iterator).field2
 *				and value1 and value2]
 *			iterator++;
 *		}
 *		return like;
 *	}
 * 
 */
class NHLikelihood  
{
public:
	/// returns the natural log of the likelihood
	virtual double getLogLikelihood(const Parameters & params, NHObservationIterator & iterator, int observations) {return 0;}
	/// preprocesses the observation
	virtual void preprocess(const NHObservation & observation) = 0;

	void setLogLikeMode(Parameters * params);

	NHLikelihood();
	virtual ~NHLikelihood();
protected:
	Parameters *modeParams;
};

#endif // !defined(AFX_NHLIKELIHOOD_H__072E8BE4_B3D7_4223_B856_003AF3FA7D90__INCLUDED_)
