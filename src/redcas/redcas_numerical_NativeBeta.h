/* DO NOT EDIT THIS FILE - it is machine generated */
#include "jni.h"
/* Header for class redcas_numerical_NativeBeta */

#ifndef _Included_redcas_numerical_NativeBeta
#define _Included_redcas_numerical_NativeBeta
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     redcas_numerical_NativeBeta
 * Method:    getPDF
 * Signature: (DDD)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeBeta_getPDF
  (JNIEnv *, jclass, jdouble, jdouble, jdouble);

/*
 * Class:     redcas_numerical_NativeBeta
 * Method:    getCDF
 * Signature: (DDD)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeBeta_getCDF
  (JNIEnv *, jclass, jdouble, jdouble, jdouble);

/*
 * Class:     redcas_numerical_NativeBeta
 * Method:    getQuantile
 * Signature: (DDD)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeBeta_getQuantile
  (JNIEnv *, jclass, jdouble, jdouble, jdouble);

/*
 * Class:     redcas_numerical_NativeBeta
 * Method:    getMean
 * Signature: (DD)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeBeta_getMean
  (JNIEnv *, jclass, jdouble, jdouble);

/*
 * Class:     redcas_numerical_NativeBeta
 * Method:    getVariance
 * Signature: (DD)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeBeta_getVariance
  (JNIEnv *, jclass, jdouble, jdouble);

/*
 * Class:     redcas_numerical_NativeBeta
 * Method:    getMedian
 * Signature: (DD)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeBeta_getMedian
  (JNIEnv *, jclass, jdouble, jdouble);

/*
 * Class:     redcas_numerical_NativeBeta
 * Method:    getSample
 * Signature: (DD)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeBeta_getSample
  (JNIEnv *, jclass, jdouble, jdouble);

/*
 * Class:     redcas_numerical_NativeBeta
 * Method:    solve
 * Signature: (DDDD[D)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeBeta_solve
  (JNIEnv *, jclass, jdouble, jdouble, jdouble, jdouble, jdoubleArray);

#ifdef __cplusplus
}
#endif
#endif