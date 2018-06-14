// Model.h: interface for the Model class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODEL_H__AA37ABDA_A59F_40F2_B74F_22CAA27BEA51__INCLUDED_)
#define AFX_MODEL_H__AA37ABDA_A59F_40F2_B74F_22CAA27BEA51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SampleSet.h"
#include "Distribution.h"
#include "Status.h"

class Model  
{
public:
	bool constructPosterior(SampleSet &samples, StatusPtr status);
	bool constructPosterior(SampleSet & samples);
	bool constructMeasure(int measure, double * parameters, SampleSet & samples, Distribution & distribution, StatusPtr status);

	/* Constructs the uncertainty distribution for the specified measure.
	 * Available measures are model-specific, and are identified by a 
	 * constant identifier.
	 * 
	 * INPUTS:
	 *  measure - identifier of measure to be computed
	 *  parameters - 'independent' parameters required to compute measure
	 *  samples - sampleset representing model parameter distribution
	 *  distribution - distribution in which to store results
	 *
	 * OUTPUT:
	 *  bool - true if distribution was succesfully computed
	 */
	bool constructMeasure(int measure, double * parameters, SampleSet & samples, Distribution & distribution);

	Model();
	virtual ~Model();

protected:
	virtual bool isValidMeasure(int measure) = 0;

	virtual double computeMeasure(int measure, double * parameters, Parameters & sample) = 0;

};

#endif // !defined(AFX_MODEL_H__AA37ABDA_A59F_40F2_B74F_22CAA27BEA51__INCLUDED_)
