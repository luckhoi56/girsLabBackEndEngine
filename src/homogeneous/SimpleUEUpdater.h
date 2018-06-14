// SimpleUEUpdater.h: interface for the SimpleUEUpdater class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMPLEUEUPDATER_H__A50E5BF1_3B1F_47AC_B390_8FC5C1B38779__INCLUDED_)
#define AFX_SIMPLEUEUPDATER_H__A50E5BF1_3B1F_47AC_B390_8FC5C1B38779__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SimpleHomogeneous.h"
#include "ParametricDistribution.h"

enum UEPair {UE_GAMMA_POISSON,UE_BETA_BINOMIAL,UE_LOGNORMAL_POISSON,
   UE_LOGNORMAL_BINOMIAL,UE_TABULAR_BINOMIAL,UE_TABULAR_POISSON};

class UEKTEntry;
class TabularEntry;

class SimpleUEUpdater : public SimpleHomogeneous  
{
public:
	void normalizeInterpretations();
	void setPrior(double par1, double par2);
	void setPrior(double * vals, double * cdfs, int size);
	void addInterpretation(int failures, double time,  double weight);
	int constructCDF(UEPair pair, double * xArray, double * cdfArray, int size);
	SimpleUEUpdater();
	virtual ~SimpleUEUpdater();
private:
	int getInterpretationCount();
	double computeCDF(UEKTEntry * entries,double x);
	void constructCDFr(TabularEntry * tabular, UEKTEntry * entries,double maxDelta);
	UEKTEntry * entries;
	ParametricDistribution ** posteriors;
	double * copyDoubleArray(double * inarr,int size);

	// Parametric prior parameters
	double parameter1;			// first prior parameter
	double parameter2;			// second prior parameter
	// Tabular prior array
	double * valArray;			// array of prior values
	double * cdfArray;			// array of prior cdfs
	int tabularSize;			// size of tabular prior
};

class UEKTEntry
{
public:
	UEKTEntry(int flrs, double tm, double wght, UEKTEntry * nxt);
	~UEKTEntry();

	int failures;		// number of failures
	double time;		// exposure time
	double weight;		// weight of the interpretation
	double probability; // probability (normalized weight)

	ParametricDistribution * posterior;

	UEKTEntry * next;
};


#endif // !defined(AFX_SIMPLEUEUPDATER_H__A50E5BF1_3B1F_47AC_B390_8FC5C1B38779__INCLUDED_)
