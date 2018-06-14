/* DO NOT EDIT THIS FILE - it is machine generated */
#include "jni.h"
/* Header for class redcas_numerical_NativeParametricDistributionFitter */

#ifndef _Included_redcas_numerical_NativeParametricDistributionFitter
#define _Included_redcas_numerical_NativeParametricDistributionFitter
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     redcas_numerical_NativeParametricDistributionFitter
 * Method:    FitGamma
 * Signature: (I[D[D)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeParametricDistributionFitter_FitGamma
  (JNIEnv *, jclass, jint, jdoubleArray, jdoubleArray);

/*
 * Class:     redcas_numerical_NativeParametricDistributionFitter
 * Method:    FitBeta
 * Signature: (I[D[D)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeParametricDistributionFitter_FitBeta
  (JNIEnv *, jclass, jint, jdoubleArray, jdoubleArray);

/*
 * Class:     redcas_numerical_NativeParametricDistributionFitter
 * Method:    FitNormal
 * Signature: (I[D[D[D)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeParametricDistributionFitter_FitNormal
  (JNIEnv *, jclass, jint, jdoubleArray, jdoubleArray, jdoubleArray);

/*
 * Class:     redcas_numerical_NativeParametricDistributionFitter
 * Method:    FitLogNormal
 * Signature: (I[D[D[D)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeParametricDistributionFitter_FitLogNormal
  (JNIEnv *, jclass, jint, jdoubleArray, jdoubleArray, jdoubleArray);

#ifdef __cplusplus
}
#endif
#endif
