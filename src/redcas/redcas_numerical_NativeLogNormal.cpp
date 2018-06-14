// $Header: C:/cvsrepo/ptdev/native/redcas/redcas/redcas_numerical_NativeLogNormal.cpp,v 1.2 2001/07/24 21:31:48 fgroen Exp $

#include "redcas_numerical_NativeLogNormal.h"
#include "LogNormalDistribution.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeLogNormal_getPDF
  (JNIEnv *, jclass, jdouble nu, jdouble tau, jdouble x)
{
	double result = 0;
	try {
		result = LogNormalDistribution(nu,tau).getPDF(x);
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeLogNormal_getCDF
  (JNIEnv *, jclass, jdouble nu, jdouble tau, jdouble x)
{
	double result = 0;
	try {
		result = LogNormalDistribution(nu,tau).getCDF(x);
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeLogNormal_getQuantile
  (JNIEnv *, jclass, jdouble nu, jdouble tau, jdouble x)
{
	double result = 0;
	try {
		result = LogNormalDistribution(nu,tau).getQuantile(x);
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeLogNormal_getMean
  (JNIEnv *, jclass, jdouble nu, jdouble tau)
{
	double result = 0;
	try {
		result = LogNormalDistribution(nu,tau).getMean();
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeLogNormal_getVariance
  (JNIEnv *, jclass, jdouble nu, jdouble tau)
{
	double result = 0;
	try {
		result = LogNormalDistribution(nu,tau).getVariance();
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeLogNormal_getMedian
  (JNIEnv *, jclass, jdouble nu, jdouble tau)
{
	double result = 0;
	try {
		result = LogNormalDistribution(nu,tau).getMedian();
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeLogNormal_getSample
  (JNIEnv *, jclass, jdouble nu, jdouble tau)
{
	double result = 0;
	try {
		result = LogNormalDistribution(nu,tau).getSample();
	} catch (...) { }
	return result;
}

#ifdef __cplusplus
}
#endif

