
#include "fiat_rdatsrv_distribution_NativeDistribution.h"
#include "rdatsrv_Distribution.h"

/*
 * Class:     fiat_rdatsrv_distribution_NativeDistribution
 * Method:    create
 * Signature: (IIDD)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_distribution_NativeDistribution_create
  (JNIEnv *, jobject, jint distType, jint inputMode, jdouble p1, jdouble p2)
{
	return CRedcassrvDist::getInstance()->create(distType, inputMode, p1, p2);
}


/*
 * Class:     fiat_rdatsrv_distribution_NativeDistribution
 * Method:    destroy
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_fiat_rdatsrv_distribution_NativeDistribution_destroy
  (JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvDist::getInstance()->destory(iHandle);
}

/*
* Class:     fiat_rdatsrv_distribution_NativeDistribution
* Method:    getX
* Signature: (ID)D
*/
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_distribution_NativeDistribution_getX
(JNIEnv *, jobject, jint iHandle, jdouble index)
{
	return CRedcassrvDist::getInstance()->getX(iHandle, index);
}

/*
 * Class:     fiat_rdatsrv_distribution_NativeDistribution
 * Method:    getPDF
 * Signature: (ID)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_distribution_NativeDistribution_getPDF
  (JNIEnv *, jobject, jint iHandle, jdouble x)
{
	return CRedcassrvDist::getInstance()->getPDF(iHandle, x);
}

/*
 * Class:     fiat_rdatsrv_distribution_NativeDistribution
 * Method:    getCDF
 * Signature: (ID)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_distribution_NativeDistribution_getCDF
  (JNIEnv *, jobject, jint iHandle, jdouble x)
{
	return CRedcassrvDist::getInstance()->getCDF(iHandle, x);
}

/*
 * Class:     fiat_rdatsrv_distribution_NativeDistribution
 * Method:    getQuantile
 * Signature: (ID)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_distribution_NativeDistribution_getQuantile
  (JNIEnv *, jobject, jint iHandle, jdouble z)
{
	return CRedcassrvDist::getInstance()->getQuantile(iHandle, z);
}

/*
 * Class:     fiat_rdatsrv_distribution_NativeDistribution
 * Method:    getMean
 * Signature: (I)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_distribution_NativeDistribution_getMean
  (JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvDist::getInstance()->getMean(iHandle);
}

/*
 * Class:     fiat_rdatsrv_distribution_NativeDistribution
 * Method:    getMedian
 * Signature: (I)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_distribution_NativeDistribution_getMedian
  (JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvDist::getInstance()->getMedian(iHandle);
}

/*
 * Class:     fiat_rdatsrv_distribution_NativeDistribution
 * Method:    getVariance
 * Signature: (I)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_distribution_NativeDistribution_getVariance
  (JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvDist::getInstance()->getVariance(iHandle);
}
