// $Header: C:/cvsrepo/ptdev/native/redcas/redcas/redcas_numerical_NativeParametricDistributionFitter.cpp,v 1.2 2001/08/24 13:03:55 droguett Exp $

#include "redcas_numerical_NativeParametricDistributionFitter.h"
#include "ParametricDistributionFitter.h"


#ifdef __cplusplus
extern "C" {
#endif


void copyNativeToJava(JNIEnv * env, jdoubleArray javaArr, double *nativeArr, int size);
void copyJavaToNative(JNIEnv * env, jdoubleArray javaArr, double *nativeArr, int size);



JNIEXPORT jint JNICALL Java_redcas_numerical_NativeParametricDistributionFitter_FitGamma
  (JNIEnv *env, jclass, jint n, jdoubleArray x, jdoubleArray pout)
{
	double * xNative = new double[n];
	double * poutNative = new double[2];
	
	copyJavaToNative(env,x,xNative,n);
	try {
		poutNative = ParametricDistributionFitter::FitGamma(n, xNative);
	} catch (...) { }
	copyNativeToJava(env,pout, poutNative,2);
	
	delete[] xNative;
	delete[] poutNative;

	return 0;
}


JNIEXPORT jint JNICALL Java_redcas_numerical_NativeParametricDistributionFitter_FitBeta
  (JNIEnv *env, jclass, jint n, jdoubleArray x, jdoubleArray pout)
{
	double * xNative = new double[n];
	double * poutNative = new double[2];
	
	copyJavaToNative(env,x,xNative,n);
	try {
		poutNative = ParametricDistributionFitter::FitBeta(n, xNative);
	} catch (...) { }
	copyNativeToJava(env,pout, poutNative,2);
	
	delete[] xNative;
	delete[] poutNative;

	return 0;
}

JNIEXPORT jint JNICALL Java_redcas_numerical_NativeParametricDistributionFitter_FitNormal
  (JNIEnv *env, jclass, jint n, jdoubleArray x, jdoubleArray y, jdoubleArray pout)
{

	double * xNative = new double[n];
	double * yNative = new double[n];
	double * poutNative = new double[3];
	
	copyJavaToNative(env,x,xNative,n);
	copyJavaToNative(env,y,yNative,n);
	try {
		poutNative = ParametricDistributionFitter::FitNormal(n, xNative, yNative);
	} catch (...) { }
	copyNativeToJava(env,pout, poutNative,3);
	
	delete[] xNative;
	delete[] yNative;
	delete[] poutNative;

	return 0;
}

JNIEXPORT jint JNICALL Java_redcas_numerical_NativeParametricDistributionFitter_FitLogNormal
  (JNIEnv *env, jclass, jint n, jdoubleArray x, jdoubleArray y, jdoubleArray pout)
{
	double * xNative = new double[n];
	double * yNative = new double[n];
	double * poutNative = new double[3];
	
	copyJavaToNative(env,x,xNative,n);
	copyJavaToNative(env,y,yNative,n);
	try {
		poutNative = ParametricDistributionFitter::FitLogNormal(n, xNative, yNative);
	} catch (...) { }
	copyNativeToJava(env,pout, poutNative,3);
	
	delete[] xNative;
	delete[] yNative;
	delete[] poutNative;

	return 0;
}



#ifdef __cplusplus
}
#endif
