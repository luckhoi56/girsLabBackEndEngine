// $Header: C:/cvsrepo/ptdev/native/redcas/redcas/redcas_numerical_NativeGamma.cpp,v 1.3 2002/02/11 16:37:04 fgroen Exp $

#include "redcas_numerical_NativeGamma.h"
#include "GammaDistribution.h"
#include "Gamma0595InputMode.h"

#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif

void copyNativeToJava(JNIEnv * env, jdoubleArray javaArr, double *nativeArr, int size);


/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeGamma_getPDF
  (JNIEnv *, jclass, jdouble beta, jdouble alpha, jdouble x)
{
	double result = 0;
	try {
		result = GammaDistribution(beta,alpha).getPDF(x);
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeGamma_getCDF
  (JNIEnv *, jclass, jdouble beta, jdouble alpha, jdouble x)
{
	double result = 0;
	try {
		result = GammaDistribution(beta,alpha).getCDF(x);
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeGamma_getQuantile
  (JNIEnv *, jclass, jdouble beta, jdouble alpha, jdouble x)
{
	double result = 0;
	try {
		result = GammaDistribution(beta,alpha).getQuantile(x);
	} catch (...) { }
	return result;
}


/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeGamma_getMean
  (JNIEnv *, jclass, jdouble beta, jdouble alpha)
{
	double result = 0;
	try {
		result = GammaDistribution(beta,alpha).getMean();
	} catch (...) { }
	return result;
}


/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeGamma_getMedian
  (JNIEnv *, jclass, jdouble beta, jdouble alpha)
{
	double result = 0;
	try {
		result = GammaDistribution(beta,alpha).getMedian();
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeGamma_getVariance
  (JNIEnv *, jclass, jdouble beta, jdouble alpha)
{
	double result = 0;
	try {
		result = GammaDistribution(beta,alpha).getVariance();
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeGamma_getSample
  (JNIEnv *, jclass, jdouble beta, jdouble alpha)
{
	double result = 0;
	try {
		result = GammaDistribution(beta,alpha).getSample();
	} catch (...) { }
	return result;
}


/*
 * TEST : none
 * Stores beta in p[0], alpha in p[1]. Returns 0 if successful.
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeGamma_solve
  (JNIEnv * env, jclass, jdouble zlo, jdouble qlo, jdouble zhi, jdouble qhi, jdoubleArray out)
{
//	cout << "Fitting Gamma parameters for " << qlo << "," << qhi << endl;
	// currently only supports zlo = 0.05 and zhi = 0.95
//	Gamma0595InputMode mode(zlo,zhi);
	Gamma0595InputMode mode;
	double native_arr[2];
	int result;
	try {
	   result = mode.convert(qlo,qhi,native_arr[1],native_arr[0]);
//	   cout << "Returning " << native_arr[0] << " " << native_arr[1] << " : " << result << endl;
	   copyNativeToJava(env,out,native_arr,2);	
	} catch (...) {
//		cout << "Exception occurred during Gamma percentile fit" << endl;
		result = 1;
	}
	return result;
}

#ifdef __cplusplus
}
#endif
