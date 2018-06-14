// InterpretationSet.cpp: implementation of the InterpretationSet class.
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
#include "InterpretationSet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

InterpretationSet::InterpretationSet()
{
	_cursor = 0;
}

InterpretationSet::~InterpretationSet()
{
	clear();
}

double InterpretationSet::getLogLikelihood(const Parameters &params)
{
	double logLike = 0.0;
	for (int index = 0; index < _cursor; index++) {
		logLike += _interpretations[index]->getLogLikelihood(params);
	}
	return logLike;
}

void InterpretationSet::addInterpretation(InterpretationPtr interpretation)
{
	if (!isFull()) {
	   _interpretations[_cursor] = interpretation;
	   _cursor++;
	}
}

void InterpretationSet::clear()
{
	for (int index = 0; index < _cursor; index++) {
		delete _interpretations[index];
		_interpretations[index] = 0L;
	}
	_cursor = 0;
}

bool InterpretationSet::isFull()
{
	return (_cursor >= MAX_INTERPRETATIONS);
}