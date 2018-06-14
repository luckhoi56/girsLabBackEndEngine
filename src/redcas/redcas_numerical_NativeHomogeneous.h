/* DO NOT EDIT THIS FILE - it is machine generated */
#include "jni.h"
/* Header for class redcas_numerical_NativeHomogeneous */

#ifndef _Included_redcas_numerical_NativeHomogeneous
#define _Included_redcas_numerical_NativeHomogeneous
#ifdef __cplusplus
extern "C" {
#endif
#undef redcas_numerical_NativeHomogeneous_PRIOR_BETA
#define redcas_numerical_NativeHomogeneous_PRIOR_BETA 1L
#undef redcas_numerical_NativeHomogeneous_PRIOR_GAMMA
#define redcas_numerical_NativeHomogeneous_PRIOR_GAMMA 2L
#undef redcas_numerical_NativeHomogeneous_PRIOR_NORMAL
#define redcas_numerical_NativeHomogeneous_PRIOR_NORMAL 3L
#undef redcas_numerical_NativeHomogeneous_PRIOR_LOGNORMAL
#define redcas_numerical_NativeHomogeneous_PRIOR_LOGNORMAL 4L
#undef redcas_numerical_NativeHomogeneous_PRIOR_TABULAR
#define redcas_numerical_NativeHomogeneous_PRIOR_TABULAR 5L
#undef redcas_numerical_NativeHomogeneous_LIKELIHOOD_BINOMIAL
#define redcas_numerical_NativeHomogeneous_LIKELIHOOD_BINOMIAL 1L
#undef redcas_numerical_NativeHomogeneous_LIKELIHOOD_POISSON
#define redcas_numerical_NativeHomogeneous_LIKELIHOOD_POISSON 2L
#undef redcas_numerical_NativeHomogeneous_LIKELIHOOD_NORMAL
#define redcas_numerical_NativeHomogeneous_LIKELIHOOD_NORMAL 3L
#undef redcas_numerical_NativeHomogeneous_LIKELIHOOD_LOGNORMAL
#define redcas_numerical_NativeHomogeneous_LIKELIHOOD_LOGNORMAL 4L
#undef redcas_numerical_NativeHomogeneous_RESULT_OK
#define redcas_numerical_NativeHomogeneous_RESULT_OK 0L
#undef redcas_numerical_NativeHomogeneous_NO_RESULT
#define redcas_numerical_NativeHomogeneous_NO_RESULT -99L
#undef redcas_numerical_NativeHomogeneous_BAD_RESULTS
#define redcas_numerical_NativeHomogeneous_BAD_RESULTS -98L
#undef redcas_numerical_NativeHomogeneous_SAMPLING_ERROR
#define redcas_numerical_NativeHomogeneous_SAMPLING_ERROR -30L
/*
 * Class:     redcas_numerical_NativeHomogeneous
 * Method:    parametric
 * Signature: (IDDIDD[D)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeHomogeneous_parametric
  (JNIEnv *, jclass, jint, jdouble, jdouble, jint, jdouble, jdouble, jdoubleArray);

/*
 * Class:     redcas_numerical_NativeHomogeneous
 * Method:    tabular
 * Signature: ([D[DIIDD[D[D)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeHomogeneous_tabular
  (JNIEnv *, jobject, jdoubleArray, jdoubleArray, jint, jint, jdouble, jdouble, jdoubleArray, jdoubleArray);

#ifdef __cplusplus
}
#endif
#endif