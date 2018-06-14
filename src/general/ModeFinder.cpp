// ModeFinder.cpp: implementation of the ModeFinder class.
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
#include "ModeFinder.h"
#include "MinusFunction.h"
#include "Matrix.h"
#include "NumericalRecipes.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ModeFinder::ModeFinder()
{
	// DO NOTHING
}

ModeFinder::~ModeFinder()
{
	// DO NOTHING
}

bool ModeFinder::findMode(Function & function, Parameters & modeLocation, double & modeValue)
{
	int iterations = 0;
	//double modeValue = 0;
	bool modeFound = false;

	MinusFunction minusFunction(function);

//	modeLocation = minusFunction.getDimension();

	Matrix matrix = Matrix::constructUnitMatrix(minusFunction.getDimension());
	NumericalRecipes::powell(modeLocation, matrix, 1e-8, iterations,
							  modeValue, minusFunction);

	matrix = Matrix::constructUnitMatrix(minusFunction.getDimension());
	modeFound = NumericalRecipes::powell(modeLocation, matrix, 1e-8, iterations,
							  modeValue, minusFunction);

	return modeFound;
}