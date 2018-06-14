// $Header: C:/cvsrepo/ptdev/native/redcas/redcas/redcas_numerical_NativeDCDFLIB.cpp,v 1.1 2001/05/04 22:26:54 droguett Exp $

#include "redcas_numerical_NativeDCDFLIB.h"
#include "DCDFLIBInterface.h"




#ifdef __cplusplus
extern "C" {
#endif


JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeDCDFLIB_getGammaLn
  (JNIEnv *, jclass, jdouble x)
{
	double result = 0;
	try {
		result = DCDFLIBInterface::computeLnGamma(x);
	} catch (...) { }
	return result;
}


JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeDCDFLIB_getDiGamma
  (JNIEnv *, jclass, jdouble x)
{
	double result = 0;
	try {
		result = DCDFLIBInterface::computeDiGamma(x);
	} catch (...) { }
	return result;
}



#ifdef __cplusplus
}
#endif
