
#include "fiat_rdatsrv_ccf_NativeCCF.h"
#include "rdatsrv_CCF.h"

/*
* Class:     fiat_rdatsrv_ccf_NativeCCF
* Method:    createCCFSession
* Signature: ()I
*/
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_ccf_NativeCCF_createCCFSession
(JNIEnv *, jobject)
{
	return CRedcassrvCCF::getInstance()->createCCFSession();
}

/*
* Class:     fiat_rdatsrv_ccf_NativeCCF
* Method:    destroyCCFSession
* Signature: (I)V
*/
JNIEXPORT void JNICALL Java_fiat_rdatsrv_ccf_NativeCCF_destroyCCFSession
(JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvCCF::getInstance()->destroyCCFSession(iHandle);
}

/*
* Class:     fiat_rdatsrv_ccf_NativeCCF
* Method:    initialize
* Signature: (III)I
*/
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_ccf_NativeCCF_initialize
(JNIEnv *, jobject, jint iHandle, jint iType, jint iSize)
{
	return CRedcassrvCCF::getInstance()->initialize(iHandle, iType, iSize);
}

/*
* Class:     fiat_rdatsrv_ccf_NativeCCF
* Method:    setPrior
* Signature: (II[D[D)I
*/
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_ccf_NativeCCF_setPrior
(JNIEnv * env, jobject, jint iHandle, jint iInputMode, jdoubleArray para1, jdoubleArray para2)
{
	jdouble * para1n = env->GetDoubleArrayElements(para1, 0);
	jdouble * para2n = env->GetDoubleArrayElements(para2, 0);
	jsize lenPara1 = env->GetArrayLength(para1);
	jsize lenPara2 = env->GetArrayLength(para2);

	if (lenPara1 != lenPara2) {
		env->ReleaseDoubleArrayElements(para1, para1n, 0);
		env->ReleaseDoubleArrayElements(para2, para2n, 0);
		return -1;
	}

	for (int i = 0; i < lenPara1; i++) {
		CRedcassrvCCF::getInstance()->setPrior(iHandle, iInputMode, i+1, para1n[i], para2n[i]);
	}

	env->ReleaseDoubleArrayElements(para1, para1n, 0);
	env->ReleaseDoubleArrayElements(para2, para2n, 0);

	return 0;
}

/*
* Class:     fiat_rdatsrv_ccf_NativeCCF
* Method:    setLikelihood
* Signature: (I[I)I
*/
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_ccf_NativeCCF_setLikelihood
(JNIEnv * env, jobject, jint iHandle, jintArray events)
{
	jint * eventsn = env->GetIntArrayElements(events, 0);
	jsize lenEvents = env->GetArrayLength(events);

	env->ReleaseIntArrayElements(events, eventsn, 0);
}

/*
* Class:     fiat_rdatsrv_ccf_NativeCCF
* Method:    runAnalysis
* Signature: (I)I
*/
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_ccf_NativeCCF_runAnalysis
(JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvCCF::getInstance()->runAnalysis(iHandle);
}

/*
* Class:     fiat_rdatsrv_ccf_NativeCCF
* Method:    getPosteriorDistHandle
* Signature: (II)I
*/
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_ccf_NativeCCF_getPosteriorDistHandle
(JNIEnv *, jobject, jint iHandle, jint iImpactLevel)
{
	return CRedcassrvCCF::getInstance()->getPosteriorDistHandle(iHandle, iImpactLevel);
}
