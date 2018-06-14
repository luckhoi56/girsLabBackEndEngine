// Session.h: interface for the Session class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SESSION_H__81E1423B_EC81_4892_A924_539FCB902699__INCLUDED_)
#define AFX_SESSION_H__81E1423B_EC81_4892_A924_539FCB902699__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MetropolisSampler.h"
#include "Parameters.h"
#include "BinnedSpace.h"
#include "Function.h"

class Session  
{
public:
	Session();

	virtual ~Session();

	void setStatusObject(Status * status);

	void clearStatus();

	int getStatusCode() const;

	void requestCancel();

	int getProgress();

	void getStatus(char * buffer, int maxChars);

	void getMarginalPoint(int point, double & value, double & probability);

	/* constructs marginal distribution */
	void constructMarginal(int marginal,int intervals, bool logScale);

	/* construct marginal distribution over specified interval */
	void constructMarginal(int marginal,int intervals, double minimum, double maximum, bool logScale);

	/* returns number of points in the marginal distribution */
	int getMarginalSize();

	/* returns the extremes on the specified marginal distribution. */
	void getMarginalExtremes(double & min, double & max);

	/* returns the cumulative marginal for specified parameter at x. */
	double getMarginalCumulative(double x);

	/* returns marginal density over given interval of parameter
	 * specified by index.
	 */
	double getMarginalDensity(double x0, double x1);

	/* constructs a binned sample space out of the sample set */
	void constructBinnedSpace(int size, bool logScale);

	/* constructs a binned sample space out of the sample set */
	void constructBinnedSpace(int size, Parameters & minima, Parameters & maxima, bool logScale);

	/* returns the density of given cell in binned sample space */
	double getBinnedDensityA(HyperIndex & hyper);

	double getBinnedParameter(int parameter, int index);

	/* adds a sample to the sample set. */
	void addSampleA(Parameters & params, double probability);

	/* sets the desired number of samples for Monte Carlo */
	void setSampleCount(int count);

	/* returns the number of samples contained in the sampleset */
	int getSampleCount();

	/* returns probability of current sample */
	double getSampleProbability(int index);

	/* returns beta of current sample */
	double getSampleParameter(int parameter, int index);

	/* clears the samples */
	void clearSamples();

	/* clears the data */
	virtual void clearData() = 0;

	virtual int runAnalysis() = 0;

protected:

	/* called when cursor in sample set is changed. */
	void cursorChanged();

	/* moves cursor in sampleset to specified position */
	void moveToSample(int index);

	/* called when sample set is changed */
	void samplesChanged();

	/* returns true if sample set has changed since last call
	 * to clearSamplesChanged()
	 */
	bool haveSamplesChanged();

	/* clears flag indicating whether sample set has changed */
	void clearSamplesChanged();

	int analyze(Function & function);

	SampleSet * getSampleSet() {return &_sampleSet;}

	MetropolisSampler * getSampler() {return _sampler;}

protected:
	StatusPtr _status;
	MetropolisSampler * _sampler;		// sampler algorithm
private:
	bool _samplesChanged;				// flag: sample set has changed
	bool _cursorChanged;				// flag: cursor in sample set has changed
	int _currentIndex;					// cursor position in sample set
	Parameters _currentParams;			// parameters corresponding to cursor
	double _currentProbability;			// probability corresponding to cursor
	SampleSet _sampleSet;				// set of samples
	BinnedSpacePtr _space;				// binned sample space
	Distribution _marginal;				// marginal distribution over parameters
	int _marginalParam;					// parameter to which marginal corresponds
	int _statusCode ;
};

#endif // !defined(AFX_SESSION_H__81E1423B_EC81_4892_A924_539FCB902699__INCLUDED_)
