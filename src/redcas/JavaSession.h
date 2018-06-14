// $Header: C:/cvsrepo/ptdev/native/redcas/redcas/JavaSession.h,v 1.4 2001/08/27 13:34:54 fgroen Exp $

// JavaSession.h: interface for the JavaSession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JAVASESSION_H__E564513A_453A_11D4_988C_002018557056__INCLUDED_)
#define AFX_JAVASESSION_H__E564513A_453A_11D4_988C_002018557056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WeibullInterpretation.h"
#include "InterpretationSet.h"
#include "Sampler.h"
#include "Parameters.h"
#include "WeibullModel.h"
#include "BinnedSpace.h"
#include "Session.h"


/* Class that implements functions for Java JNI interface
 * specified in redcas_numerical_NativeWeibull.h. A 
 * corresponding C++ file exists that delegates all function
 * calls to this class.
 *
 * This class is partially prepared for multi-threaded access.
 * It is allowed to call the methods getStatus() and getProgress()
 * and requestCancel() while other methods are running. All other
 * methods should be called in single-threaded fashion, to avoid
 * the risk of data corruption.
 */
class JavaSession : public Session
{
public: 
 
	/* performs unit test. */
	static void testJavaSession();

	/* returns the density of given cell in binned sample space */
	double getBinnedDensity(int alpha, int beta);

	/* returns the alpha representative for cells with specified coordinate */
	double getBinnedAlpha(int index);

	/* returns the beta representative for cells with specified coordinate */
	double getBinnedBeta(int index);

	/* returns the cumulative failure probability at specified time */
	double getCumulative(double time, double quantile);

	/* returns the reliability at the specified time */
	double getReliability(double time, double quantile);

	/* returns the hazard rate at the specified time */
	double getHazardRate(double time, double quantile);

	double getHazardRateMean(double time);

	double getCumulativeMean(double time);

	double getReliabilityMean(double time);

	/* returns the MTTF */
	double getMTTF(double quantile);

	/* adds a sample to the sample set. */
	void addSample(double alpha, double beta, double probability);

	/* runs the analysis of the data */
	int runAnalysis();

	/* returns beta of current sample */
	double getSampleBeta(int index);

	/* returns alpha of current sample */
	double getSampleAlpha(int index);

	/* clears the data */
	void clearData();

	/* adds a data point (observation) */
	void addDataPoint(double start, double end, bool failure);

	/* adds a data point (observation) */
	void addDataPoint(double start, double end, int population, int failures);

	/* Set prior distribution parameters */
	void setPrior(double nu1, double tau1, double nu2, double tau2);

	JavaSession();
	
	virtual ~JavaSession();

private:

	/* checks whether content of sample set has changed,
	 * and acts on it when necessary.
	 */
	void checkSamplesChanged();

	WeibullInterpretation * _failures;	// list of failures
	WeibullInterpretation * _successes; // list of successes
	WeibullInterpretation * _bins;		// list of binned data
	InterpretationSet * _joint;			// joint interpretation
	WeibullModel _model;				// weibull model

	bool _bPriorSettingRequired;
	double _scale_nu;
	double _scale_tau;
	double _shape_nu;
	double _shape_tau;
};

#endif // !defined(AFX_JAVASESSION_H__E564513A_453A_11D4_988C_002018557056__INCLUDED_)
