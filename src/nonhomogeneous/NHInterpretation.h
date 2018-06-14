//
// File:      $Id: NHInterpretation.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//
 

#if !defined(AFX_NHINTERPRETATION_H__221B0168_8176_4606_895B_EFABFE89418A__INCLUDED_)
#define AFX_NHINTERPRETATION_H__221B0168_8176_4606_895B_EFABFE89418A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NHObservation.h"
#include "NHLikelihood.h"
#include "../global/Interpretation.h"
#include "../global/Parameters.h"

/**
 * Non-homogoneous interpretation class.
 * Interpretations act as containers for data points, and 
 * know how to compute the likelihood for those data points
 * (referred to as observations).
 *
 * When the interpretation is created, an instance of a
 * NHLikelihood class must be passed. It is the NHLikelihood
 * that actually computes the likelihood function.
 * Note that this likelihood object should be dynamically
 * allocated, and that the interpretation deallocates it 
 * when the interpretation itself gets deallocated.
 * The constructors for sub-classes must therefore start 
 * by calling super(new [likelihood class])
 *
 * Assuming similarity of data content for each type of observation
 * (two doubles), observations are by definition stored in the
 * form of NHObservation classes. Observations are added to 
 * the interpretation by calling the addObservation() method 
 * This method automatically calls the 
 * NHLikelihood::preprocess() method on the observation.
 *
 * Note that different types of interpretations are constructed
 * through composition, not inheritance. Inheritance is possible
 * but should not be necessary.
 */
class NHInterpretation : public Interpretation  
{
public:
	double getEstimateSDev(int default_interpretation);
	double getEstimateMean(int default_interpretation);
	int getObservationCount();
	double getTotalExposure(int default_interpretation);
	double getTotalEvents(int default_interpretation);
	NHInterpretation(NHLikelihood * likelihood);
	virtual ~NHInterpretation();

	/// adds a copy of the observation to the observation list.
	void addObservation(const NHObservation & observation);
	/// returns the natural log of the likelihood function.
	double getLogLikelihood(const Parameters &params);

	void setLogLikeMode(Parameters * params);
private:
	NHLikelihood * likelihood; // likelihood function
	NHObservationList observations; // list of observations
};

#endif // !defined(AFX_NHINTERPRETATION_H__221B0168_8176_4606_895B_EFABFE89418A__INCLUDED_)
