#include "stdafx.h"
#include "fiat_rdatsrv_homogeneous_NativeHomogeneous.h"
#include "rdatsrv_Homogeneous.h"

/*
 * Class:     fiat_rdatsrv_homogeneous_NativeHomogeneous
 * Method:    create
 * Signature: (IIDDIIDD)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_homogeneous_NativeHomogeneous_create
(JNIEnv *, jobject, jint priorType, jint priorInputMode, jdouble pp1, jdouble pp2, 
	jint likelihoodType, jint likelihoodInputMode, jdouble lp1, jdouble lp2) 
{
	return CRedcassrvHomo::getInstance()->create(priorType, priorInputMode, 
		pp1, pp2, likelihoodType, likelihoodInputMode, lp1, lp2);
}

/*
 * Class:     fiat_rdatsrv_homogeneous_NativeHomogeneous
 * Method:    destroy
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_fiat_rdatsrv_homogeneous_NativeHomogeneous_destroy
  (JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvHomo::getInstance()->destory(iHandle);
}

/*
 * Class:     fiat_rdatsrv_homogeneous_NativeHomogeneous
 * Method:    getPDF
 * Signature: (ID)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_homogeneous_NativeHomogeneous_getPDF
  (JNIEnv *, jobject, jint iHandle, jdouble x)
{
	return CRedcassrvHomo::getInstance()->getPDF(iHandle, x);
}

/*
 * Class:     fiat_rdatsrv_homogeneous_NativeHomogeneous
 * Method:    getCDF
 * Signature: (ID)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_homogeneous_NativeHomogeneous_getCDF
  (JNIEnv *, jobject, jint iHandle, jdouble x)
{
	return CRedcassrvHomo::getInstance()->getCDF(iHandle, x);
}

/*
 * Class:     fiat_rdatsrv_homogeneous_NativeHomogeneous
 * Method:    getQuantile
 * Signature: (ID)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_homogeneous_NativeHomogeneous_getQuantile
  (JNIEnv *, jobject, jint iHandle, jdouble z)
{
	return CRedcassrvHomo::getInstance()->getQuantile(iHandle, z);
}

/*
 * Class:     fiat_rdatsrv_homogeneous_NativeHomogeneous
 * Method:    getMean
 * Signature: (I)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_homogeneous_NativeHomogeneous_getMean
  (JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvHomo::getInstance()->getMean(iHandle);
}

/*
 * Class:     fiat_rdatsrv_homogeneous_NativeHomogeneous
 * Method:    getMedian
 * Signature: (I)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_homogeneous_NativeHomogeneous_getMedian
  (JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvHomo::getInstance()->getMedian(iHandle);
}

/*
 * Class:     fiat_rdatsrv_homogeneous_NativeHomogeneous
 * Method:    getVariance
 * Signature: (I)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_homogeneous_NativeHomogeneous_getVariance
  (JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvHomo::getInstance()->getVariance(iHandle);
}
