// MinusFunction.h: interface for the MinusFunction class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINUSFUNCTION_H__E441FBB5_40D0_11D4_9885_002018557056__INCLUDED_)
#define AFX_MINUSFUNCTION_H__E441FBB5_40D0_11D4_9885_002018557056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Function.h"

/**
 * Function wrapper that returns minus the result of underlying
 * function: f(x) = -f'(x)
 */
class MinusFunction : public Function  
{
public:
	double evaluate(const Parameters & parameters) const;
	int getDimension() const;

	MinusFunction(Function & function);
	virtual ~MinusFunction();

private:
	FunctionPtr _function;
};

#endif // !defined(AFX_MINUSFUNCTION_H__E441FBB5_40D0_11D4_9885_002018557056__INCLUDED_)
