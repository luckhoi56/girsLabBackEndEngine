#include "redcas_license_LicenseController.h"

#include "../licensing/LicenseController.h"

// #include <afxwin.h>
#include <windows.h>
#include <rpc.h>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

#ifdef _DEBUG
#include <iostream>
#endif

#ifdef __cplusplus
extern "C" {
#endif

//BOOL GetNicAddress(LPSTR pszNicAddress, UINT nBufSize);
//BOOL GetCDriveSerialNumber(LPDWORD pdwSerialNumber);
//void ConstructSystemID(char * buf, int len);

LicenseController lic_control;

/*
 * Class:     redcas_license_LicenseController
 * Method:    getSysIDN
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_redcas_application_license_LicenseController_getSysIDN
  (JNIEnv * env, jclass)
{
	char buf[10];
	lic_control.ConstructSystemID(buf,10);
	return (env)->NewStringUTF(buf);
}

JNIEXPORT jlong JNICALL Java_redcas_application_license_LicenseController_getStartTimeN(JNIEnv *, jclass) {

	__int64 time = lic_control.retrieveStartTime();
	return time;

//	__int64 ntime = 0;
//	long * ltime = (long *)&ntime;
//	int version;
//	ifstream timefile("redspprt.inf");
//	if (timefile == 0) return 0;
//	timefile >> version >> ltime[0] >> ltime[1];
//	timefile.close();
//	return ntime;

}

JNIEXPORT jboolean JNICALL Java_redcas_application_license_LicenseController_setStartTimeN(JNIEnv *, jclass, jlong time)  
{
	bool retval = lic_control.storeStartTime(time);
	return retval ? TRUE : FALSE;

//	__int64 ntime = time;
//	long * ltime = (long *)&ntime;
//	ofstream timefile("redspprt.inf");
//	timefile.clear();
//	timefile << "1 " << ltime[0] << " " << ltime[1];
//	timefile.close();
}


JNIEXPORT jstring JNICALL Java_redcas_application_license_LicenseController_getCodeN(JNIEnv * env, jclass, jstring cnfg) 
{
	// adapted from page 25 in Liang
	char code [512];
	const char *config = env->GetStringUTFChars(cnfg,NULL);
	if (config == NULL) return NULL;
	lic_control.retrieveLicenseCode(config,code,255);
	env->ReleaseStringUTFChars(cnfg,config);

	if (code != 0) {	
		return env->NewStringUTF(code);
	} else {
		return NULL;
	}
}

JNIEXPORT jboolean JNICALL Java_redcas_application_license_LicenseController_setCodeN(JNIEnv * env, jclass, jstring cnfg, jstring code)
{
	// adapted from page 25 in Liang
	const char *config;
	const char *license;

	config = env->GetStringUTFChars(cnfg,NULL);
	if (config == NULL) {
		return NULL;
	}

	license = env->GetStringUTFChars(code,NULL);
	if (license == NULL) {
		env->ReleaseStringUTFChars(cnfg,config);
		return NULL;
	}

	bool retval = lic_control.storeLicenseCode(config,license);

	env->ReleaseStringUTFChars(code,license);
	env->ReleaseStringUTFChars(cnfg,config);

	return retval ? TRUE : FALSE;
}

#ifdef __cplusplus
}
#endif
