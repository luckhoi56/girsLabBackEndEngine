// Sample.cpp: implementation of the Sample class.
//
//////////////////////////////////////////////////////////////////////

/****************************************************
 *
 *  Bayesian Weibull Sampler.
 *
 *	(c) 2000, Frank Groen. All Rights Reserved.
 *
 ****************************************************/

#include "stdafx.h"
#include "Sample.h"

#ifdef _DEBUG
#include <iostream>
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


#ifdef _DEBUG
int Sample::_instances = 0;
#endif 

Sample::Sample() 
{
	_logWeight = 0.0;
#ifdef _DEBUG
	_instances ++;
#endif
}

Sample::Sample(const Parameters & params, double logWeight)
{
	_params = params;
	_logWeight = logWeight;
#ifdef _DEBUG
	_instances ++;
#endif
}

Sample::Sample(const Sample & sample)
{
	_logWeight = sample.getLogWeight();
	_params = sample.getParameters();
#ifdef _DEBUG
	_instances ++;
#endif
}

Sample::~Sample()
{
#ifdef _DEBUG
	_instances --;
//	if (_instances == 0)
//		cout << "ALL SAMPLES DEALLOCATED" << endl;
//	if (_instances == -1)
//		cout << "DEALLOCATION ERROR" << endl;
#endif
}

double Sample::getLogWeight() const
{
	return _logWeight;
}

Parameters Sample::getParameters() const
{
	return Parameters(_params);
}

void Sample::testUnit()
{
#ifdef _DEBUG
	Parameters p(1,1);
	Sample * sample = new Sample(p,1);
	delete sample;
#endif
}
