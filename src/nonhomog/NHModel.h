//
// File:      $Id: NHModel.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//

#if !defined(AFX_NHMODEL_H__3B9ACF25_07D9_4B49_BDD2_62A1A305952F__INCLUDED_)
#define AFX_NHMODEL_H__3B9ACF25_07D9_4B49_BDD2_62A1A305952F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SampleSet.h"
#include "NHInterpretation.h"
#include "NHPrior.h"
#include "MetropolisGenerator.h"
#include "Status.h"

// generic list of interpretation types
#define NHINTERPRETATION_DEFAULT		   -1
#define NHINTERPRETATION_5TH_95TH			1
#define NHINTERPRETATION_MEAN_SDEV			2
#define NHINTERPRETATION_EVENTS_DEMANDS		3
#define NHINTERPRETATION_EVENTS_TIME		4

// generic list of measures
#define NHMEASURE_POPVAR_CDF	1
#define NHMEASURE_POPVAR_PDF	2
#define NHMEASURE_POPVAR_MEAN   3

// generic list of parameters
#define NHPARAMETER_DEFAULT		1

/**
 * The NHModel is a superclass for non-homogeneous model classes.
 * These classes have a number of responsibilities, for which template
 * methods are given in this class:
 * 
 * createInterpretation(int type): creates (dynamically allocates) an 
 * instance of a NHInterpretation class corresponding to the specified
 * type. Type is one of the constants defined above. If the interpretation
 * is not recognized by the model, 0 (null) is returned. The NHModel is 
 * not responsible for deallocating the interpretation objects it created.
 *
 * computeMeasure(...) is used to compute the population variability over
 * a specified parameter in the form of an uncertainty distribution
 * over either the CDF or the PDF (which are considered to be the measure).
 * For now, population variability can only be computed for a default 
 * parameter (e.g. lambda in case of gamma-poisson), but later on it will
 * be possible to compute population variability over other measures (e.g.
 * MTTF). The uncertainty about the CDF or PDF is stored in a Distribution
 * object, which is supplied as an argument. The method returns 0 if the 
 * the distribution was computed successfully.
 *
 * Note that the Distribution class itself contains methods capable of 
 * deriving point estimates (e.g. mean or median) from these uncertainty
 * distributions.
 *
 * A convenience method computeBounds(...) is supplied that returns 
 * reasonable bounds (for display purposes) on the specified parameter
 * (for instance lambda) The calling method can then decide on spacing 
 * of sample points within that range (or any other range if desired).
 * 
 */
class NHModel
{
public:
	// returns a new instance of an interpretation of specified type.
	virtual NHInterpretation * createInterpretation(int type) = 0;
	// returns a newly allocated instance of a MetropolisGenerator;
	virtual MetropolisGenerator * createGenerator() = 0;
	// returns a newly allocated instance of a default prior
	virtual NHPrior * createDefaultPrior() = 0;
	virtual NHPrior * createUserPrior(int mode, double nu1, double tau1, double nu2, double tau2) = 0;
	// sets the sampleset based on which to compute derived measures.
	void setSampleSet(SampleSetPtr sampleset);
	// computes the measure uncertainty at x
	virtual int computeMeasure(int parameter, double value, int measure,
		Distribution & distribution, StatusPtr status) = 0;
	// computes the uncertainty on the given percentile of the population
	// variability distribution
	int computePercentile(double percentile, Distribution & distribution, StatusPtr status);
	
	int computeMean(Distribution & distribution, StatusPtr status);

	int computeVariance(Distribution & distribution, StatusPtr status);

	// computes reasonable bounds for parameter value
	int computeBounds(int parameter, double & lower, double & higher);

	virtual bool accept(NHInterpretation & interpretation) {return true;}

	double findPercentile(double &value, double lower, double higher, double percent, double epsilon = 1.01);
	double findPercentile(double percent, double epsilon);

	NHModel();
	virtual ~NHModel();
protected:
	SampleSetPtr samples;
//	void restrictBoundsEx(double &lower, double &higher);
	virtual double computePercentile(const Parameters & params, double percentile) const = 0;
	virtual double computeMean(const Parameters & params) const = 0;
	virtual double computeVariance(const Parameters & params) const = 0;
	virtual double estimatePercentile(double lo, double lo_cdf, double hi, double hi_cdf, double z) const = 0;
};

#endif // !defined(AFX_NHMODEL_H__3B9ACF25_07D9_4B49_BDD2_62A1A305952F__INCLUDED_)
