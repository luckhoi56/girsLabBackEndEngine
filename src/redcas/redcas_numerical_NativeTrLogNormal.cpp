// $Header: C:/cvsrepo/ptdev/native/redcas/redcas/redcas_numerical_NativeTrLogNormal.cpp,v 1.2 2001/07/24 21:31:48 fgroen Exp $

#include "redcas_numerical_NativeTrLogNormal.h"
#include "TrLogNormalDistribution.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeTrLogNormal_getPDF
  (JNIEnv *, jclass, jdouble nu, jdouble tau, jdouble x)
{
	double result = 0;
	try {
		result = TrLogNormalDistribution(nu,tau).getPDF(x);
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeTrLogNormal_getCDF
  (JNIEnv *, jclass, jdouble nu, jdouble tau, jdouble x)
{
	double result = 0;
	try {
		result = TrLogNormalDistribution(nu,tau).getCDF(x);
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeTrLogNormal_getQuantile
  (JNIEnv *, jclass, jdouble nu, jdouble tau, jdouble x)
{
	double result = 0;
	try {
		result = TrLogNormalDistribution(nu,tau).getQuantile(x);
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeTrLogNormal_getMean
  (JNIEnv *, jclass, jdouble nu, jdouble tau)
{
	double result = 0;
	try {
		result = TrLogNormalDistribution(nu,tau).getMean();
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeTrLogNormal_getMedian
  (JNIEnv *, jclass, jdouble nu, jdouble tau)
{
	double result = 0;
	try {
		result = TrLogNormalDistribution(nu,tau).getMedian();
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeTrLogNormal_getVariance
  (JNIEnv *, jclass, jdouble nu, jdouble tau)
{
	double result = 0;
	try {
		result = TrLogNormalDistribution(nu,tau).getVariance();
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeTrLogNormal_getSample
  (JNIEnv *, jclass, jdouble nu, jdouble tau)
{
	double result = 0;
	try {
		result = TrLogNormalDistribution(nu,tau).getSample();
	} catch (...) { }
	return result;
}

#ifdef __cplusplus
}
#endif

