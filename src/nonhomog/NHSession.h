//
// File:      $Id: NHSession.h 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//
 
#if !defined(AFX_NHSESSION_H__058313CC_378F_4188_B6E6_94B9E1E68891__INCLUDED_)
#define AFX_NHSESSION_H__058313CC_378F_4188_B6E6_94B9E1E68891__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Session.h"
#include "NHInterpretation.h"
#include "NHModel.h"

enum NHModels {logpois,gampois,loglog,normnorm,betbin,logbin};
enum NHMeasures {cdf,pdf,quantile,mean,variance};
enum NHParameters {dflt};

class NHSession : public Session  
{
public:
	void clearSamplesEx();
	void computeBounds();
	void addSample(double alpha,double beta, double prob);
	double getMeasureMean(NHParameters parameter, double value, NHMeasures measure);
	double getMeasure(NHParameters parameter, double value, NHMeasures measure, double quantile);
	double getPopVarUpperBound();
	double getPopVarLowerBound();
	double getBinnedBeta(int b);
	double getBinnedAlpha(int a);
	double getBinnedDensity(int a, int b);

	double findMeanPercentile(double cdf);

	double getSampleAlpha(int index);
	double getSampleBeta(int index);

	int getSampleMarginal(int index, Distribution & distribution);

	int runAnalysis();
	void clearData();

	void addDataPoint(double x_star, double tau, double events, double exposure);
	int setModel(NHModels model);

	double computeLnLikelihood(double p1, double p2) const;

	void setPrior(int mode, double nu1, double tau1, double nu2, double tau2);

	NHSession();
	virtual ~NHSession();

	static void testUnit(char * dir);
	static void testBetaBinomial();
	static void testGammaPoisson();
	static void testLogNormalLogNormal();
	static void testLogNormalPoisson();

private:
	void clearBounds();
	void computeMeasure(NHParameters param, double value, NHMeasures measure);
	void clearMeasure();
	NHInterpretation * interpretation;
	NHModel * model;
	NHPrior * user_prior;

	Distribution distribution;	// distribution of last computed measure
	NHParameters distParameter; // parameter type of last computed measure
	double distValue;			// parameter value of last computed measure
	NHMeasures distMeasure;		// type of last computed measure
	double parameterUpperBound; // recommended upper bound for pop-var curve
	double parameterLowerBound; // recommended lower bound for pop-var curve
	bool parameterBoundsComputed; // bounds on the pop-var curve computed

};

#endif // !defined(AFX_NHSESSION_H__058313CC_378F_4188_B6E6_94B9E1E68891__INCLUDED_)
