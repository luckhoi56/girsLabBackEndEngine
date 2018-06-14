#include "redcas_application_license_WinLicenseServiceProvider.h"

#include "../licensecontrol/CopyControl.h"
#include "../licensecontrol/PredictionTechnologyLog.h"

CCopyControl theCtrl;

/*
* Class:     redcas_application_license_WinLicenseServiceProvider
* Method:    SK_getLicenseFileHandle
* Signature: (Ljava/lang/String;)I
*/
JNIEXPORT jint JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1getLicenseFileHandle
(JNIEnv * env, jobject, jstring strDirectory)
{
	jboolean blnIsCopy;
	const char* strCIn = (env)->GetStringUTFChars(strDirectory, &blnIsCopy);
	int iHandle = CLicenseHandleMgr::getInstance()->getLicenseFileHandle(strCIn);
	(env)->ReleaseStringUTFChars(strDirectory, strCIn); // release jstring

	return iHandle;
}


/*
* Class:     redcas_application_license_WinLicenseServiceProvider
* Method:    SK_getSessionID
* Signature: (I)J
*/
JNIEXPORT jlong JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1getSessionID
(JNIEnv *, jobject, jint iHandle)
{
	MODULE_LICENSE_INFO * pLicHandle = CLicenseHandleMgr::getInstance()->getHandle(iHandle);
	if (pLicHandle == NULL) {
		return -1;
	};

	return theCtrl.getSessionID(pLicHandle);
}

/*
* Class:     redcas_application_license_WinLicenseServiceProvider
* Method:    SK_isDemoMode
* Signature: (I)Z
*/
JNIEXPORT jboolean JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1isDemoMode
(JNIEnv *, jobject, jint iHandle)
{
	MODULE_LICENSE_INFO * pLicHandle = CLicenseHandleMgr::getInstance()->getHandle(iHandle);
	if (pLicHandle == NULL) {
		return true;
	};

	return pLicHandle->m_bDemoMode;
}

/*
* Class:     redcas_application_license_WinLicenseServiceProvider
* Method:    SK_getModuleMode
* Signature: (IJ)I
*/
JNIEXPORT jint JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1getModuleMode
(JNIEnv *, jobject, jint iHandle, jlong lModuleCode)
{
	MODULE_LICENSE_INFO * pLicHandle = CLicenseHandleMgr::getInstance()->getHandle(iHandle);
	if (pLicHandle == NULL) {
		return true;
	};

	if (pLicHandle->m_bDemoMode) {
		// The entire platform is running on demo mode
		return true;
	}

	// Check the input module status
	return theCtrl.getModuleMode(lModuleCode, pLicHandle);
}

/*
* Class:     redcas_application_license_WinLicenseServiceProvider
* Method:    SK_isNewLicenseMode
* Signature: (I)Z
*/
JNIEXPORT jboolean JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1isNewLicenseMode
(JNIEnv *, jobject, jint iHandle)
{
	MODULE_LICENSE_INFO * pLicHandle = CLicenseHandleMgr::getInstance()->getHandle(iHandle);
	if (pLicHandle == NULL) {
		return false;
	};

	return pLicHandle->m_bNewLicense;
}

/*
* Class:     redcas_application_license_WinLicenseServiceProvider
* Method:    SK_getMachineID
* Signature: ()J
*/
JNIEXPORT jlong JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1getMachineID
(JNIEnv *, jobject)
{
	return theCtrl.getComputerID();
}

/*
* Class:     redcas_application_license_WinLicenseServiceProvider
* Method:    SK_activateSoftware
* Signature: (IJJJLjava/lang/String;Ljava/lang/String;)Z
*/
JNIEXPORT jboolean JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1activateSoftware
(JNIEnv * env, jobject, jint iHandle, jlong lUserCode1, jlong lUserCode2, jlong lLicenseID, jstring strPassword, jstring strCompanyName)
{
	MODULE_LICENSE_INFO * pLicHandle = CLicenseHandleMgr::getInstance()->getHandle(iHandle);
	if (pLicHandle == NULL)
		return false;

	jboolean blnIsCopyCompanyName, bInIsCopyPassword;
	const char* strCInPassword = (env)->GetStringUTFChars(strPassword, &bInIsCopyPassword);
	const char* strCInCompanyName = (env)->GetStringUTFChars(strCompanyName, &blnIsCopyCompanyName);
	jboolean bResult = theCtrl.ManualActivateSoftware(lUserCode1, lUserCode2, lLicenseID,
		strCInPassword, strCInCompanyName, pLicHandle);
	(env)->ReleaseStringUTFChars(strPassword, strCInPassword); // release jstring
	(env)->ReleaseStringUTFChars(strCompanyName, strCInCompanyName); // release jstring

	return bResult;
}

/*
* Class:     redcas_application_license_WinLicenseServiceProvider
* Method:    SK_getErrorMessage
* Signature: ()Ljava/lang/String;
*/
JNIEXPORT jstring JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1getErrorMessage
(JNIEnv * env, jobject)
{
	jstring jstrOutput;
	string strErrorMessage = theCtrl.getErrorMessage();
	jstrOutput = (env)->NewStringUTF(strErrorMessage.c_str()); // convert char array to jstring
	return jstrOutput;
}

/*
* Class:     redcas_application_license_WinLicenseServiceProvider
* Method:    SK_getDemoDaysLeft
* Signature: (I)I
*/
JNIEXPORT jint JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1getDemoDaysLeft
(JNIEnv *, jobject, jint iHandle)
{
	MODULE_LICENSE_INFO * pLicHandle = CLicenseHandleMgr::getInstance()->getHandle(iHandle);
	if (pLicHandle == NULL)
		return 30;

	return theCtrl.getDemoDaysLeft(pLicHandle);
}

/*
* Class:     redcas_application_license_WinLicenseServiceProvider
* Method:    SK_statusChanged
* Signature: (I)Z
*/
JNIEXPORT jboolean JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1statusChanged
(JNIEnv *, jobject, jint iHandle)
{
	MODULE_LICENSE_INFO * pLicHandle = CLicenseHandleMgr::getInstance()->getHandle(iHandle);
	if (pLicHandle == NULL)
		return false;

	return theCtrl.statusChanged(pLicHandle);
}

/*
* Class:     redcas_application_license_WinLicenseServiceProvider
* Method:    SK_setAppPath
* Signature: (Ljava/lang/String;)V
*/
JNIEXPORT void JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1setAppPath
(JNIEnv * env, jobject, jstring strPath)
{
	jboolean blnIsCopy;
	const char* strCIn = (env)->GetStringUTFChars(strPath, &blnIsCopy);
	theCtrl.setAppPath(strCIn);
	(env)->ReleaseStringUTFChars(strPath, strCIn); // release jstring
}

/*
* Class:     redcas_application_license_WinLicenseServiceProvider
* Method:    SK_getTrialDaysLeft
* Signature: (I)I
*/
JNIEXPORT jint JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1getTrialDaysLeft
(JNIEnv *, jobject, jint iHandle)
{
	MODULE_LICENSE_INFO * pLicHandle = CLicenseHandleMgr::getInstance()->getHandle(iHandle);
	if (pLicHandle == NULL)
		return 0;

	return theCtrl.getTrialDaysLeft(pLicHandle);
}

/*
* Class:     redcas_application_license_WinLicenseServiceProvider
* Method:    SK_activeSoftware
* Signature: (ILjava/lang/String;Ljava/lang/String;)Z
*/
JNIEXPORT jboolean JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1activeSoftware
(JNIEnv * env, jobject, jint iHandle, jstring strLicenseID, jstring strPassword)
{
	MODULE_LICENSE_INFO * pLicHandle = CLicenseHandleMgr::getInstance()->getHandle(iHandle);
	if (pLicHandle == NULL)
		return false;

	jboolean blnIsCopy;
	const char* strCInID = (env)->GetStringUTFChars(strLicenseID, &blnIsCopy);
	const char* strCInPassword = (env)->GetStringUTFChars(strPassword, &blnIsCopy);
	jboolean bResult = theCtrl.onlineActivation(strCInID, strCInPassword, pLicHandle);
	(env)->ReleaseStringUTFChars(strLicenseID, strCInID); // release jstring
	(env)->ReleaseStringUTFChars(strPassword, strCInPassword); // release jstring

	return bResult;
}

/*
* Class:     redcas_application_license_WinLicenseServiceProvider
* Method:    SK_getRegCompanyName
* Signature: (I)Ljava/lang/String;
*/
JNIEXPORT jstring JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1getRegCompanyName
(JNIEnv * env, jobject, jint iHandle)
{
	MODULE_LICENSE_INFO * pLicHandle = CLicenseHandleMgr::getInstance()->getHandle(iHandle);
	jstring jstrOutput;

	string strRegCompanyName = "";
	if (pLicHandle != NULL)
	{
		strRegCompanyName = theCtrl.getRegCompanyName(pLicHandle);
	}
	jstrOutput = (env)->NewStringUTF(strRegCompanyName.c_str()); // convert char array to jstring
	return jstrOutput;
}

/*
* Class:     redcas_application_license_WinLicenseServiceProvider
* Method:    SK_getMaintenanceExpirationDate
* Signature: (I)I
*/
JNIEXPORT jint JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1getMaintenanceExpirationDate
(JNIEnv *, jobject, jint iHandle)
{
	MODULE_LICENSE_INFO * pLicHandle = CLicenseHandleMgr::getInstance()->getHandle(iHandle);

	if (pLicHandle == NULL)
		return 0;

	return theCtrl.GetMaintenanceExpirationDate(pLicHandle);
}

/*
* Class:     redcas_application_license_WinLicenseServiceProvider
* Method:    SK_checkProductUpdate
* Signature: (IJJJ)Z
*/
JNIEXPORT jboolean JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1checkProductUpdate
(JNIEnv * env, jobject, jint iHandle, jlong lAppMajor, jlong lAppMinor, jlong lAppRelease)
{
	MODULE_LICENSE_INFO * pLicHandle = CLicenseHandleMgr::getInstance()->getHandle(iHandle);
	if (pLicHandle == NULL)
		return false;

	return theCtrl.checkProductUpdate(pLicHandle, lAppMajor, lAppMinor, lAppRelease);
}

/*
* Class:     redcas_application_license_WinLicenseServiceProvider
* Method:    SK_getReleaseInformation
* Signature: ()Ljava/lang/String;
*/
JNIEXPORT jstring JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1getReleaseInformation
(JNIEnv * env, jobject)
{
	jstring jstrOutput;

	string strLatestRelease = "";
	strLatestRelease = theCtrl.getLatestReleaseInformation();

	jstrOutput = (env)->NewStringUTF(strLatestRelease.c_str()); // convert char array to jstring
	return jstrOutput;
}

/*
* Class:     redcas_application_license_WinLicenseServiceProvider
* Method:    SK_invalidLicenseDetected
* Signature: (I)Z
*/
JNIEXPORT jboolean JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1invalidLicenseDetected
(JNIEnv *, jobject, jint iHandle)
{
	MODULE_LICENSE_INFO * pLicHandle = CLicenseHandleMgr::getInstance()->getHandle(iHandle);
	if (pLicHandle == NULL) {
		return false;
	};

	return pLicHandle->m_bInvalidLicenseDetected;
}

/*
* Class:     redcas_application_license_WinLicenseServiceProvider
* Method:    SK_getPostLicenseParameterString
* Signature: (I)Ljava/lang/String;
*/
JNIEXPORT jstring JNICALL Java_redcas_application_license_WinLicenseServiceProvider_SK_1getPostLicenseParameterString
(JNIEnv * env, jobject, jint iHandle)
{
	MODULE_LICENSE_INFO * pLicHandle = CLicenseHandleMgr::getInstance()->getHandle(iHandle);
	if (pLicHandle == NULL) {
		return false;
	}

	CPredictionTechnologyLog predLog;
	string strValue = predLog.getPostLicenseParameterString(pLicHandle);

	jstring jstrOutput;
	jstrOutput = (env)->NewStringUTF(strValue.c_str()); // convert char array to jstring
	return jstrOutput;
}
