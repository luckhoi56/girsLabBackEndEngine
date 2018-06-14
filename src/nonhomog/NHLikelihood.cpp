// NHLikelihood.cpp: implementation of the NHLikelihood class.
//
// $Header: C:/cvsrepo/ptdev/native/redcas/nonhomog/NHLikelihood.cpp,v 1.6 2003/07/10 22:47:28 fgroen Exp $
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NHLikelihood.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NHLikelihood::NHLikelihood()
{
	modeParams = NULL;

}

NHLikelihood::~NHLikelihood()
{
	delete this->modeParams;
}

/**
 * Specifies where the mode of the likelihood is located.
 * This is used when mode is determined externally.
 */
void NHLikelihood::setLogLikeMode(Parameters * params)
{
	this->modeParams = new Parameters(0, 0);
	this->modeParams->setValue(0, params->getValue(0));
	this->modeParams->setValue(1, params->getValue(1));
}