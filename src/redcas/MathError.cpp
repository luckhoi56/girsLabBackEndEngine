// $Header: C:/cvsrepo/ptdev/native/redcas/redcas/MathError.cpp,v 1.2 2001/07/24 21:31:48 fgroen Exp $

#include <math.h>
#include "error.h"

/* Handle math errors 
 */
int _matherr( struct _exception *except )
{
	switch (except->type) {
		case _DOMAIN: throw MATH_DOMAIN_ERROR;
		case _SING: throw MATH_SINGULARITY_ERROR;
		case _OVERFLOW: throw MATH_OVERFLOW_ERROR;
		case _TLOSS: throw MATH_TLOSS_ERROR;
		case _UNDERFLOW: return MATH_UNDERFLOW_ERROR;
		default : throw MATH_UNEXPECTED_ERROR;
	}
}
