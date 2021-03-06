/* DO NOT EDIT THIS FILE - it is machine generated */
#include "jni.h"
/* Header for class redcas_application_license_WinLicenseServiceProvider */

#ifndef _Included_redcas_application_license_WinLicenseServiceProvider
#define _Included_redcas_application_license_WinLicenseServiceProvider
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     redcas_application_license_WinLicenseServiceProvider
 * Method:    SK_getLicenseFileHandle
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1getLicenseFileHandle
  (JNIEnv *, jobject, jstring);

/*
 * Class:     redcas_application_license_WinLicenseServiceProvider
 * Method:    SK_getSessionID
 * Signature: (I)J
 */
JNIEXPORT jlong JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1getSessionID
  (JNIEnv *, jobject, jint);

/*
 * Class:     redcas_application_license_WinLicenseServiceProvider
 * Method:    SK_isDemoMode
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1isDemoMode
  (JNIEnv *, jobject, jint);

/*
 * Class:     redcas_application_license_WinLicenseServiceProvider
 * Method:    SK_getModuleMode
 * Signature: (IJ)I
 */
JNIEXPORT jint JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1getModuleMode
  (JNIEnv *, jobject, jint, jlong);

/*
 * Class:     redcas_application_license_WinLicenseServiceProvider
 * Method:    SK_isNewLicenseMode
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1isNewLicenseMode
  (JNIEnv *, jobject, jint);

/*
 * Class:     redcas_application_license_WinLicenseServiceProvider
 * Method:    SK_getMachineID
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1getMachineID
  (JNIEnv *, jobject);

/*
 * Class:     redcas_application_license_WinLicenseServiceProvider
 * Method:    SK_activeSoftware
 * Signature: (ILjava/lang/String;Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1activeSoftware
  (JNIEnv *, jobject, jint, jstring, jstring);

/*
 * Class:     redcas_application_license_WinLicenseServiceProvider
 * Method:    SK_getErrorMessage
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1getErrorMessage
  (JNIEnv *, jobject);

/*
 * Class:     redcas_application_license_WinLicenseServiceProvider
 * Method:    SK_getDemoDaysLeft
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1getDemoDaysLeft
  (JNIEnv *, jobject, jint);

/*
 * Class:     redcas_application_license_WinLicenseServiceProvider
 * Method:    SK_statusChanged
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1statusChanged
  (JNIEnv *, jobject, jint);

/*
 * Class:     redcas_application_license_WinLicenseServiceProvider
 * Method:    SK_setAppPath
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1setAppPath
  (JNIEnv *, jobject, jstring);

/*
 * Class:     redcas_application_license_WinLicenseServiceProvider
 * Method:    SK_getTrialDaysLeft
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1getTrialDaysLeft
  (JNIEnv *, jobject, jint);

/*
 * Class:     redcas_application_license_WinLicenseServiceProvider
 * Method:    SK_activateSoftware
 * Signature: (IJJJLjava/lang/String;Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1activateSoftware
  (JNIEnv *, jobject, jint, jlong, jlong, jlong, jstring, jstring);

/*
 * Class:     redcas_application_license_WinLicenseServiceProvider
 * Method:    SK_getRegCompanyName
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1getRegCompanyName
  (JNIEnv *, jobject, jint);

/*
 * Class:     redcas_application_license_WinLicenseServiceProvider
 * Method:    SK_getMaintenanceExpirationDate
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1getMaintenanceExpirationDate
  (JNIEnv *, jobject, jint);

/*
 * Class:     redcas_application_license_WinLicenseServiceProvider
 * Method:    SK_checkProductUpdate
 * Signature: (IJJJ)Z
 */
JNIEXPORT jboolean JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1checkProductUpdate
  (JNIEnv *, jobject, jint, jlong, jlong, jlong);

/*
 * Class:     redcas_application_license_WinLicenseServiceProvider
 * Method:    SK_getReleaseInformation
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1getReleaseInformation
  (JNIEnv *, jobject);

/*
 * Class:     redcas_application_license_WinLicenseServiceProvider
 * Method:    SK_invalidLicenseDetected
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1invalidLicenseDetected
  (JNIEnv *, jobject, jint);

/*
 * Class:     redcas_application_license_WinLicenseServiceProvider
 * Method:    SK_getPostLicenseParameterString
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1getPostLicenseParameterString
  (JNIEnv *, jobject, jint);

#ifdef __cplusplus
}
#endif
#endif
