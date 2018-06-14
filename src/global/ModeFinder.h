// ModeFinder.h: interface for the ModeFinder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODEFINDER_H__265CFB2C_3E72_11D4_9881_002018557056__INCLUDED_)
#define AFX_MODEFINDER_H__265CFB2C_3E72_11D4_9881_002018557056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Parameters.h"
#include "Function.h"

class ModeFinder  
{
public:
	ModeFinder();
	virtual ~ModeFinder();

	/* Returns location of the mode of the function.
	 * This method works well for unimodal functions.
	 * 
	 * Dimension of modeLocation must correspond to the 
	 * number of parameters required by the function. 
	 * The values of the parameters are used as an initial 
	 * guess.
	 */
	static bool findMode(Function & function, Parameters & modeLocation, double & modeValue);
};

#endif // !defined(AFX_MODEFINDER_H__265CFB2C_3E72_11D4_9881_002018557056__INCLUDED_)
