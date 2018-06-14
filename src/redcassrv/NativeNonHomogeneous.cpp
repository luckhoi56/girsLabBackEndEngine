#include "stdafx.h"
#include "rdatsrv_NonHomogeneous.h"
#include "fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous.h"

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    createSessionN
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_createSessionN
  (JNIEnv *, jobject)
{
	return CRedcassrvNonHomo::getInstance()->createSession();
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    destroySessionN
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_destroySessionN
  (JNIEnv *, jobject, jint iHandle)
{
	CRedcassrvNonHomo::getInstance()->destroySession(iHandle);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    setModelN
 * Signature: (IIDDDD)V
 */
JNIEXPORT void JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_setModelN
  (JNIEnv *, jobject, jint iHandle, jint iType, jdouble p11, jdouble p12, jdouble p21, jdouble p22)
{
	CRedcassrvNonHomo::getInstance()->setModelN(iHandle, iType, p11, p12, p21, p22);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    runAnalysisN
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_runAnalysisN
  (JNIEnv *, jobject, jint iHandle, jint iSamples)
{
	return CRedcassrvNonHomo::getInstance()->runAnalysis(iHandle, iSamples);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    addDataPoint
 * Signature: (DDDD)V
 */
JNIEXPORT void JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_addDataPoint
  (JNIEnv *, jobject, jint iHandle, jdouble x_star, jdouble tau, jdouble events, jdouble exposure)
{
	CRedcassrvNonHomo::getInstance()->addDataPoint(iHandle, x_star, tau, events, exposure);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    getProgress
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_getProgress
  (JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvNonHomo::getInstance()->getProgress(iHandle);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    getStatusCode
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_getStatusCode
  (JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvNonHomo::getInstance()->getStatusCode(iHandle);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    requestCancel
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_requestCancel
  (JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvNonHomo::getInstance()->requestCancel(iHandle);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    getPopVarMeanMean
 * Signature: (I)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_getPopVarMeanMean
  (JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvNonHomo::getInstance()->getPopVarMeanMean(iHandle);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    getPopVarVarianceMean
 * Signature: (I)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_getPopVarVarianceMean
  (JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvNonHomo::getInstance()->getPopVarVarianceMean(iHandle);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    getPopVarMeanPercentile
 * Signature: (ID)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_getPopVarMeanPercentile
  (JNIEnv *, jobject, jint iHandle, jdouble p)
{
	return CRedcassrvNonHomo::getInstance()->getPopVarMeanPercentile(iHandle, p);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    getPopVarPercentilePercentile
 * Signature: (IDD)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_getPopVarPercentilePercentile
  (JNIEnv *, jobject, jint iHandle, jdouble z, jdouble p)
{
	return CRedcassrvNonHomo::getInstance()->getPopVarPercentilePercentile(iHandle, z, p);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    getSampleCount
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_getSampleCount
  (JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvNonHomo::getInstance()->getSampleCount(iHandle);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    constructBins
 * Signature: (IIZ)Z
 */
JNIEXPORT jboolean JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_constructBins
  (JNIEnv *, jobject, jint iHandle, jint iSize, jboolean logScale)
{
	return CRedcassrvNonHomo::getInstance()->constructBins(iHandle, iSize, logScale);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    getBinnedAlpha
 * Signature: (II)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_getBinnedAlpha
  (JNIEnv *, jobject, jint iHandle, jint index)
{
	return CRedcassrvNonHomo::getInstance()->getBinnedAlpha(iHandle, index);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    getBinnedBeta
 * Signature: (II)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_getBinnedBeta
  (JNIEnv *, jobject, jint iHandle, jint index)
{
	return CRedcassrvNonHomo::getInstance()->getBinnedBeta(iHandle, index);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    getBinnedDensity
 * Signature: (III)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_getBinnedDensity
  (JNIEnv *, jobject, jint iHandle, jint alpha, jint beta)
{
	return CRedcassrvNonHomo::getInstance()->getBinnedDensity(iHandle, alpha, beta);
}

/*
* Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
* Method:    getMarginalSize
* Signature: (I)I
*/
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_getMarginalSize
(JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvNonHomo::getInstance()->getMarginalSize(iHandle);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    getMarginalAlpha
 * Signature: (IIZ)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_getMarginalAlpha
  (JNIEnv *, jobject, jint iHandle, jint iSize, jboolean bLogScale)
{
	return CRedcassrvNonHomo::getInstance()->getMarginalAlpha(iHandle, iSize, bLogScale);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    getMarginalBeta
 * Signature: (IIZ)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_getMarginalBeta
  (JNIEnv *, jobject, jint iHandle, jint iSize, jboolean bLogScale)
{
	return CRedcassrvNonHomo::getInstance()->getMarginalBeta(iHandle, iSize, bLogScale);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    getUpperBound
 * Signature: (I)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_getUpperBound
  (JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvNonHomo::getInstance()->getUpperBound(iHandle);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    getLowerBound
 * Signature: (I)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_getLowerBound
  (JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvNonHomo::getInstance()->getLowerBound(iHandle);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    clearData
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_clearData
  (JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvNonHomo::getInstance()->clearData(iHandle);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    clearSamplesN
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_clearSamplesN
  (JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvNonHomo::getInstance()->clearSamplesN(iHandle);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    getPopVarCDF
 * Signature: (IDD)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_getPopVarCDF
  (JNIEnv *, jobject, jint iHandle, jdouble x, jdouble p)
{
	return CRedcassrvNonHomo::getInstance()->getPopVarCDF(iHandle, x, p);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    getPopVarPDF
 * Signature: (IDD)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_getPopVarPDF
  (JNIEnv *, jobject, jint iHandle, jdouble x, jdouble p)
{
	return CRedcassrvNonHomo::getInstance()->getPopVarPDF(iHandle, x, p);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    getPopVarCDFMean
 * Signature: (ID)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_getPopVarCDFMean
  (JNIEnv *, jobject, jint iHandle, jdouble x)
{
	return CRedcassrvNonHomo::getInstance()->getPopVarCDFMean(iHandle, x);
}

/*
 * Class:     fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous
 * Method:    getPopVarPDFMean
 * Signature: (ID)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_nonhomogeneous_NativeNonHomogeneous_getPopVarPDFMean
  (JNIEnv *, jobject, jint iHandle, jdouble x)
{
	return CRedcassrvNonHomo::getInstance()->getPopVarPDFMean(iHandle, x);
}
