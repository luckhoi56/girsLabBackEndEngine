// EPHESInterface.cpp: implementation of the CEPHESInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "EPHESInterface.h"
#include <math.h>

double expn(int n, double x);


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEPHESInterface::CEPHESInterface()
{

}

CEPHESInterface::~CEPHESInterface()
{

}

/**
 * Computes exponential integral En(x) [-> not Ei(x)].
 */
double CEPHESInterface::exponentIntegralEn(int n, double x)
{
	return expn(n,x);
}


