// Interpretation.h: interface for the Interpretation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERPRETATION_H__265CFB23_3E72_11D4_9881_002018557056__INCLUDED_)
#define AFX_INTERPRETATION_H__265CFB23_3E72_11D4_9881_002018557056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Parameters.h"

/* Superclass for interpretations. Interpretations are
 * container classes that associate observations with
 * events defined in the likelihood models, such as weibull.
 * 
 * The only function defined for an interpretation is 
 * getLogLikelihood(), which returns the natural log of the
 * likelihood function for the interpretation.
 */
class Interpretation  
{
public:
	Interpretation();
	virtual ~Interpretation();

	/* Returns natural log of likelihood function. */
	virtual double getLogLikelihood(const Parameters &params) = 0;

	virtual void setLogLikeMode(Parameters * params) {};
};

typedef Interpretation * InterpretationPtr;

#endif // !defined(AFX_INTERPRETATION_H__265CFB23_3E72_11D4_9881_002018557056__INCLUDED_)
