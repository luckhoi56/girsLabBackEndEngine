// Function.h: interface for the Function class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUNCTION_H__265CFB30_3E72_11D4_9881_002018557056__INCLUDED_)
#define AFX_FUNCTION_H__265CFB30_3E72_11D4_9881_002018557056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Parameters.h"

/* Super class for functions, i.e. classes capable
 * of evaluating a function of parameters.
 */
class Function  
{
public:
	Function();
	virtual ~Function();

	/* Evaluates the function. */
	virtual double evaluate(const Parameters & params) const;

	virtual double evaluate(double x) const;

	/* Returns the dimension of the parameters required for evaluation. */
	virtual int getDimension() const = 0;

	virtual void setLogLikeMode(Parameters * params) {}
};

typedef Function * FunctionPtr;

#endif // !defined(AFX_FUNCTION_H__265CFB30_3E72_11D4_9881_002018557056__INCLUDED_)
