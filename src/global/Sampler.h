// Sampler.h: interface for the Sampler class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAMPLER_H__F50348C1_48E9_11D4_8735_000102479105__INCLUDED_)
#define AFX_SAMPLER_H__F50348C1_48E9_11D4_8735_000102479105__INCLUDED_

#include "Parameters.h"	// Added by ClassView
#include "SampleSet.h"	// Added by ClassView
#include "Function.h"
#include "Status.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Sampler  
{
public:

	virtual ~Sampler();

	virtual void setSampleSize(int size) = 0;
	virtual int sample(Function & function) = 0;
	virtual int sample(Function & function, StatusPtr status) = 0;
	virtual int sample(Function & function, Parameters & start) = 0;
protected:
	SampleSetPtr _sampleSet;
};

#endif // !defined(AFX_SAMPLER_H__F50348C1_48E9_11D4_8735_000102479105__INCLUDED_)
