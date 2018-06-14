/* DO NOT EDIT THIS FILE - it is machine generated */
#include "jni.h""
/* Header for class redcas_numerical_NativeWeibull2 */

#ifndef _Included_redcas_numerical_NativeWeibull2
#define _Included_redcas_numerical_NativeWeibull2
#ifdef __cplusplus
extern "C" {
#endif
#undef redcas_numerical_NativeWeibull2_DEFAULT_SAMPLE_SIZE
#define redcas_numerical_NativeWeibull2_DEFAULT_SAMPLE_SIZE 10000L
#undef redcas_numerical_NativeWeibull2_DPDMODE_LINEAR_INTERVAL
#define redcas_numerical_NativeWeibull2_DPDMODE_LINEAR_INTERVAL 1L
#undef redcas_numerical_NativeWeibull2_DPDMODE_LOGARITHMIC_INTERVAL
#define redcas_numerical_NativeWeibull2_DPDMODE_LOGARITHMIC_INTERVAL 2L
#undef redcas_numerical_NativeWeibull2_MARGINALMODE_LINEAR_INTERVAL
#define redcas_numerical_NativeWeibull2_MARGINALMODE_LINEAR_INTERVAL 1L
#undef redcas_numerical_NativeWeibull2_MARGINALMODE_LOGARITHMIC_INTERVAL
#define redcas_numerical_NativeWeibull2_MARGINALMODE_LOGARITHMIC_INTERVAL 2L
#undef redcas_numerical_NativeWeibull2_MARGINAL_ALPHA
#define redcas_numerical_NativeWeibull2_MARGINAL_ALPHA 0L
#undef redcas_numerical_NativeWeibull2_MARGINAL_BETA
#define redcas_numerical_NativeWeibull2_MARGINAL_BETA 1L
#undef redcas_numerical_NativeWeibull2_ANALYSIS_EXCEPTION
#define redcas_numerical_NativeWeibull2_ANALYSIS_EXCEPTION 99978L
/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    createWeibullSession
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_createWeibullSession
  (JNIEnv *, jobject);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    destroyWeibullSession
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_redcas_numerical_NativeWeibull2_destroyWeibullSession
  (JNIEnv *, jobject, jint);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    initialize
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_initialize
  (JNIEnv *, jobject, jint);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    clearStatusN
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_redcas_numerical_NativeWeibull2_clearStatusN
  (JNIEnv *, jclass, jint);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    setSampleCountN
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_setSampleCountN
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    getSampleCountN
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_getSampleCountN
  (JNIEnv *, jclass, jint);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    getAlphaN
 * Signature: (II)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeWeibull2_getAlphaN
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    getBetaN
 * Signature: (II)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeWeibull2_getBetaN
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    getProbabilityN
 * Signature: (II)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeWeibull2_getProbabilityN
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    createTestDataSet
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_createTestDataSet
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    setTestDataRecord
 * Signature: (IIDDZ)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_setTestDataRecord
  (JNIEnv *, jobject, jint, jint, jdouble, jdouble, jboolean);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    attachTestDataSet
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_attachTestDataSet
  (JNIEnv *, jobject, jint);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    createGroupedDataSet
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_createGroupedDataSet
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    setGroupedDataRecord
 * Signature: (IIDDII)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_setGroupedDataRecord
  (JNIEnv *, jobject, jint, jint, jdouble, jdouble, jint, jint);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    attachGroupedDataSet
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_attachGroupedDataSet
  (JNIEnv *, jobject, jint);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    setShapePriorN
 * Signature: (IIDD)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_setShapePriorN
  (JNIEnv *, jobject, jint, jint, jdouble, jdouble);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    setLogNormalScalePriorN
 * Signature: (IIDD)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_setLogNormalScalePriorN
  (JNIEnv *, jobject, jint, jint, jdouble, jdouble);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    setRxCxScalePriorN
 * Signature: (IDDD)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_setRxCxScalePriorN
  (JNIEnv *, jobject, jint, jdouble, jdouble, jdouble);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    setI1000ScalePriorN
 * Signature: (IDDD)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_setI1000ScalePriorN
  (JNIEnv *, jobject, jint, jdouble, jdouble, jdouble);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    generateSamples
 * Signature: (IIIIIJZ)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_generateSamples
  (JNIEnv *, jobject, jint, jint, jint, jint, jint, jlong, jboolean);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    getStatusN
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_getStatusN
  (JNIEnv *, jobject, jint);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    getProgressN
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_getProgressN
  (JNIEnv *, jobject, jint);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    estimateJointParameterDensity
 * Signature: (IZI)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_estimateJointParameterDensity
  (JNIEnv *, jobject, jint, jboolean, jint);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    getJointDensities
 * Signature: (II[D[D[D)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_getJointDensities
  (JNIEnv *, jobject, jint, jint, jdoubleArray, jdoubleArray, jdoubleArray);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    computeParameter
 * Signature: (II[D[D[D[D[D[D[D[D)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_computeParameter
  (JNIEnv *, jobject, jint, jint, jdoubleArray, jdoubleArray, jdoubleArray, jdoubleArray, jdoubleArray, jdoubleArray, jdoubleArray, jdoubleArray);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    computeMeasures
 * Signature: (II[D[D[D[D[D[D[D[D[D)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_computeMeasures
  (JNIEnv *, jobject, jint, jint, jdoubleArray, jdoubleArray, jdoubleArray, jdoubleArray, jdoubleArray, jdoubleArray, jdoubleArray, jdoubleArray, jdoubleArray);

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    computeWeibullMTTF
 * Signature: (IDD)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeWeibull2_computeWeibullMTTF
  (JNIEnv *, jobject, jint, jdouble, jdouble);

#ifdef __cplusplus
}
#endif
#endif
