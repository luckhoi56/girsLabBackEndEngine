//
// File:      $Id: NHFunction.cpp 152 2005-01-24 13:04:33Z saulo $
// Revision:  $Revision: 152 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//


#include "NHFunction.h"

/**
 * Constructor.
 * Function does not assume ownership of prior and likelihood.
 */
NHFunction::NHFunction(NHPrior * prior, Interpretation * interpretation)
{
	this->prior = prior;
	this->interpretation = interpretation;
}

NHFunction::~NHFunction()
{

}

/**
 * Returns natural log of prior / likelihood product.
 * Prior and likelihood are evaluated for specified parameters.
 */
double NHFunction::evaluate(const Parameters & params) const
{
	double retval = interpretation->getLogLikelihood(params);
	if (prior) retval += prior->getLogPrior(params);
	return retval;
}

/**
 * Specifies location of the mode.
 */
void NHFunction::setLogLikeMode(Parameters * params)
{
	interpretation->setLogLikeMode(params);
}
