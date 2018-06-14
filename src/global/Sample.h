// Sample.h: interface for the Sample class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAMPLE_H__7B40FBC8_3FB7_11D4_9882_002018557056__INCLUDED_)
#define AFX_SAMPLE_H__7B40FBC8_3FB7_11D4_9882_002018557056__INCLUDED_

#include "Parameters.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/**
 * Represents a single sample, comprised of a Parameter plus
 * associated weight.
 * This class is intended for use by SampleSet only.
 */
class Sample  
{
public:
	Sample();
	Sample(const Sample & sample);
	Sample(const Parameters & params, double logWeight);
	virtual ~Sample();
	double getLogWeight() const;
	Parameters getParameters() const;

	static void testUnit();

#ifdef _DEBUG
	static int _instances;
#endif

private:
	Parameters _params;
	double _logWeight;
	int _dimension;
};

#endif // !defined(AFX_SAMPLE_H__7B40FBC8_3FB7_11D4_9882_002018557056__INCLUDED_)
