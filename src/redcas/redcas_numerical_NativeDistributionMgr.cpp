
#include "redcas_numerical_NativeDistributionMgr.h"
#include "DistributionManager.h"
#include "ParametricDistribution.h"
#include "UniformBinomialDistribution.h"
#include "UniformPoissonDistribution.h"

#ifndef _Included_redcas_numerical_NativeDistributionMgr
#define _Included_redcas_numerical_NativeDistributionMgr
#ifdef __cplusplus
extern "C" {
#endif

//
//  SOURCE TEMPORARILY PLACED IN NativeBeta BECAUSE OF VS6 PROBLEMS
//

/*

JNIEXPORT jint JNICALL Java_redcas_numerical_NativeDistributionMgr_create
  (JNIEnv *, jclass, jint type, jdouble p1, jdouble p2, jdouble p3, jdouble p4)
{
	
	ParametricDistribution * dist = 0;

	switch (type) {
	case redcas_numerical_NativeDistributionMgr_LOGUNIFORM_BINOMIAL:
		dist = new UniformBinomialDistribution(p1,p2,p3,p4,true);
		break;
	case redcas_numerical_NativeDistributionMgr_LOGUNIFORM_POISSON:
		dist = new UniformPoissonDistribution(p1,p2,p3,p4,true);
		break;
	default: 
		dist = 0;
		break;
	}

	if (dist == 0) return -1;

	int retval = DistributionManager::getInstance()->add(dist);
	if (retval == -1) {
		delete dist;
	}
	return retval;
}



JNIEXPORT void JNICALL Java_redcas_numerical_NativeDistributionMgr_destroy
  (JNIEnv *, jclass, jint handle)
{
	ParametricDistribution dist = DistributionManager::getInstance()->remove(handle);
	delete dist;
}


JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeDistributionMgr_getPDF
  (JNIEnv *, jclass, jint handle, jdouble x)
{
	double retval = 0;
	ParametricDistribution dist = DistributionManager::getInstance()->retrieve(handle);
	if (dist != 0) {
		retval = dist->getPDF(x);
	}
	return retval;
}


JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeDistributionMgr_getCDF
  (JNIEnv *, jclass, jint handle, jdouble x)
{
	double retval = 0;
	ParametricDistribution dist = DistributionManager::getInstance()->retrieve(handle);
	if (dist != 0) {
		retval = dist->getCDF(x);
	}
	return retval;
}


JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeDistributionMgr_getQuantile
  (JNIEnv *, jclass, jint handle, jdouble z)
{
	double retval = 0;
	ParametricDistribution dist = DistributionManager::getInstance()->retrieve(handle);
	if (dist != 0) {
		retval = dist->getQuantile(z);
	}
	return retval;
}


JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeDistributionMgr_getMean
  (JNIEnv *, jclass, jint)
{
	double retval = 0;
	ParametricDistribution dist = DistributionManager::getInstance()->retrieve(handle);
	if (dist != 0) {
		retval = dist->getMean();
	}
	return retval;
}


JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeDistributionMgr_getMedian
  (JNIEnv *, jclass, jint)
{
	double retval = 0;
	ParametricDistribution dist = DistributionManager::getInstance()->retrieve(handle);
	if (dist != 0) {
		retval = dist->getMedian();
	}
	return retval;
}


JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeDistributionMgr_getVariance
  (JNIEnv *, jclass, jint)
{
	double retval = 0;
	ParametricDistribution dist = DistributionManager::getInstance()->retrieve(handle);
	if (dist != 0) {
		retval = dist->getVariance();
	}
	return retval;
}


JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeDistributionMgr_getSample
  (JNIEnv *, jclass, jint)
{
	double retval = 0;
	ParametricDistribution dist = DistributionManager::getInstance()->retrieve(handle);
	if (dist != 0) {
		retval = dist->getSample();
	}
	return retval;
}

*/

#ifdef __cplusplus
}
#endif
#endif
