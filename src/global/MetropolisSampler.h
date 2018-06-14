// MetropolisSampler.h: interface for the MetropolisSampler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_METROPOLISSAMPLER_H__22BAB621_46DF_11D4_8735_000086586C9E__INCLUDED_)
#define AFX_METROPOLISSAMPLER_H__22BAB621_46DF_11D4_8735_000086586C9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Function.h"
#include "Parameters.h"
#include "Status.h"
#include "SampleSet.h"
#include "Sampler.h"
#include "MetropolisGenerator.h"
#include "MetropolisGammaGenerator.h"

/* Metropolis sampling algorithm. Implementation of Metropolis sampler 
 * described in:
 *
 * Chib and Greenberg, 'Understanding the Metropolis-Hastings Algorithm'
 * The American Statistician v49 n4 p327 (1995) 
 *
 * Uses implementations of MetropolisGenerator for generation of 
 * candidates. By default, MetropolisGammaGenerator is used. Use
 * setGenerator() to change.
 */
class MetropolisSampler : public Sampler
{
public:

	MetropolisSampler(SampleSet & set);

	virtual ~MetropolisSampler();

	/* Sets the generator. Note that the sampler takes responsibility
	 * for deleting the generator, once it is no longer needed. 
	 * Generators must therefore be dynamically allocated.
	 */
	void setGenerator(MetropolisGenerator * generator);

	/* Returns the ratio of rejected samples and accepted samples.
	 */
	double getRejectionRate() const;

	/* Executes the sampling routine. Progress messages are sent
	 * to status.
	 */
	int sample(Function & function, StatusPtr status);
	int sample(Function & function, Parameters & start);
	int sample(Function & function, Parameters & start, StatusPtr status);

	/* Executes the sampling routine. */
	int sample(Function & function);

	/* Specifies size of sample set. */
	void setSampleSize(int size);

	/* Returns number of generated samples.
	 * OBSOLETE: call SampleSet.size() directly.
	 */
	int getSampleSize();

private:

//	Parameters initCandidateGenerator(int dimension, Function & function, MetropolisGenerator * generator, StatusPtr status);
	Parameters initCandidateGenerator(int dimension, Function & function, MetropolisGenerator * generator, Parameters & start, StatusPtr status);

	double generateUniformSample();

	double computeLogDensity(const Parameters & x, const Parameters & y);

	Parameters generateCandidate(const Parameters & sample);

	int _sampleSize;		// specified number of samples
	int _rejected;
	MetropolisGenerator * _generator;
	MetropolisGammaGenerator _defaultGenerator;
};

#endif // !defined(AFX_METROPOLISSAMPLER_H__22BAB621_46DF_11D4_8735_000086586C9E__INCLUDED_)
