// $Header: C:/cvsrepo/ptdev/native/redcas/redcas/redcas_numerical_NativeNormal.cpp,v 1.2 2001/07/24 21:31:48 fgroen Exp $

#include "redcas_numerical_NativeNormal.h"
#include "NormalDistribution.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeNormal_getPDF
  (JNIEnv *, jclass, jdouble mu, jdouble sigma, jdouble x)
{
	double result = 0;
	try {
		result = NormalDistribution(mu,sigma).getPDF(x);
	} catch (...) { }
	return result;
}
/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeNormal_getVariance
  (JNIEnv *, jclass, jdouble mu, jdouble sigma)
{
	double result = 0;
	try {
		result = NormalDistribution(mu,sigma).getVariance();
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeNormal_getCDF
  (JNIEnv *, jclass, jdouble mu, jdouble sigma, jdouble x)
{
	double result = 0;
	try {
		result = NormalDistribution(mu,sigma).getCDF(x);
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeNormal_getQuantile
  (JNIEnv *, jclass, jdouble mu, jdouble sigma, jdouble x)
{
	double result = 0;
	try {
		result = NormalDistribution(mu,sigma).getQuantile(x);
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeNormal_getMean
  (JNIEnv *, jclass, jdouble mu, jdouble sigma)
{
	double result = 0;
	try {
		result = NormalDistribution(mu,sigma).getMean();
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeNormal_getMedian
  (JNIEnv *, jclass, jdouble mu, jdouble sigma)
{
	double result = 0;
	try {
		result = NormalDistribution(mu,sigma).getMedian();
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeNormal_getSample
  (JNIEnv *, jclass, jdouble mu, jdouble sigma)
{
	double result = 0;
	try {
		result = NormalDistribution(mu,sigma).getSample();
	} catch (...) { }
	return result;
}

#ifdef __cplusplus
}
#endif