
#include "redcas_numerical_NativeWeibull2.h"
#include "WeibullSessionManager.h"
#include "BRASSEngine.h"
#include "Weibulldefs.h"

#include <iostream>


#ifdef __cplusplus
extern "C"
{
#endif

BRASSEngine * getWeibullSession(int index)
{
	return static_cast<BRASSEngine *>
		(WeibullSessionManager::getInstance()->retrieveSession(index));
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    createWeibullSession
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_createWeibullSession
	(JNIEnv *, jobject)
{
	return WeibullSessionManager::getInstance()->createSession();
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    destroyWeibullSession
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_redcas_numerical_NativeWeibull2_destroyWeibullSession
(JNIEnv *, jobject, jint iHandle)
{
	WeibullSessionManager::getInstance()->destroySession(iHandle);
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    initialize
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_initialize
(JNIEnv *, jobject, jint iHandle)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->setup(1, 1, 1);
	}

	return redcas_ErrorCode_InvalidHandle;
}

/*
* Class:     redcas_numerical_NativeWeibull2
* Method:    clearStatusN
* Signature: (I)V
*/
JNIEXPORT void JNICALL Java_redcas_numerical_NativeWeibull2_clearStatusN
(JNIEnv *, jclass, jint iHandle)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		session->clear();
	}
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    setSampleCountN
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_setSampleCountN
(JNIEnv *, jclass, jint, jint)
{
	return 0;
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    getSampleCountN
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_getSampleCountN
(JNIEnv *, jclass, jint)
{
	return 0;
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    getAlphaN
 * Signature: (II)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeWeibull2_getAlphaN
(JNIEnv *, jclass, jint, jint)
{
	return 0;
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    getBetaN
 * Signature: (II)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeWeibull2_getBetaN
(JNIEnv *, jclass, jint, jint)
{
	return 0;
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    getProbabilityN
 * Signature: (II)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeWeibull2_getProbabilityN
(JNIEnv *, jclass, jint, jint)
{
	return 0;
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    createTestDataSet
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_createTestDataSet
(JNIEnv *, jobject, jint iHandle, jint record_cnt)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->createTestDataSet(record_cnt);
	}

	return redcas_ErrorCode_InvalidHandle;
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    setTestDataRecord
 * Signature: (IIDDZ)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_setTestDataRecord
(JNIEnv *, jobject, jint iHandle, jint index, jdouble start, jdouble end, jboolean bSuccess)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		if (bSuccess) {
			return session->setTestDataRecord(index, start, end, 0);
		}
		else {
			return session->setTestDataRecord(index, start, end, 1);
		}
	}

	return redcas_ErrorCode_InvalidHandle;
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    attachTestDataSet
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_attachTestDataSet
(JNIEnv *, jobject, jint iHandle)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->attachTestDataSet(0, 1);
	}

	return redcas_ErrorCode_InvalidHandle;
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    createGroupedDataSet
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_createGroupedDataSet
(JNIEnv *, jobject, jint iHandle, jint record_cnt)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->createGroupedDataSet(record_cnt);
	}

	return redcas_ErrorCode_InvalidHandle;
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    setGroupedDataRecord
 * Signature: (IIDDII)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_setGroupedDataRecord
(JNIEnv *, jobject, jint iHandle, jint index, jdouble start, jdouble end, jint population, jint failures)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->setGroupedDataRecord(index, start, end, failures, population);
	}

	return redcas_ErrorCode_InvalidHandle;
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    attachGroupedDataSet
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_attachGroupedDataSet
(JNIEnv *, jobject, jint iHandle)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->attachGroupedDataSet(0, 1);
	}

	return redcas_ErrorCode_InvalidHandle;
}

/*
* Class:     redcas_numerical_NativeWeibull2
* Method:    setShapePriorN
* Signature: (IIDD)I
*/
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_setShapePriorN
(JNIEnv *, jobject, jint iHandle, jint inputMode, jdouble para1, jdouble para2)
{	
	BRASSEngine * session = getWeibullSession(iHandle);

	double median, errfac, nu, tau;
	double z95 = 1.6448536295514722;

	if (inputMode == REDCASSRV_MEDIANEF_MODE) {
		median = para1;
		errfac = para2;
	}
	else if (inputMode == REDCASSRV_MEANEF_MODE) {
		tau = log(para2) / z95;
		nu = log(para1) - tau * tau / 2;
		median = exp(nu);
		errfac = exp(z95 * tau);
	}
	else if (inputMode == REDCASSRV_5TH95TH_MODE) {
		nu = log(para1 * para2) / 2;
		tau = log(para2 / para1) / 3.28971;
		median = exp(nu);
		errfac = exp(z95 * tau);
	}
	else if (inputMode == REDCASSRV_MEDIANTAU_MODE) {
		median = para1;
		errfac = exp(z95 * para2);
	}
	else if (inputMode == REDCASSRV_NUTAU_MODE) {
		median = exp(para1);
		errfac = exp(z95 * para2);
	}

	if (session != 0L) {
		return session->addShapePrior(1, median, errfac);
	}

	return redcas_ErrorCode_InvalidHandle;
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    setLogNormalScalePrior
 * Signature: (IIDD)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_setLogNormalScalePriorN
(JNIEnv *, jobject, jint iHandle, jint inputMode, jdouble para1, jdouble para2)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		double median, errfac, nu, tau;
		double z95 = 1.6448536295514722;

		if (inputMode == REDCASSRV_MEDIANEF_MODE) {
			median = para1;
			errfac = para2;
		}
		else if (inputMode == REDCASSRV_MEANEF_MODE) {
			tau = log(para2) / z95;
			nu = log(para1) - tau * tau / 2;
			median = exp(nu);
			errfac = exp(z95 * tau);
		}
		else if (inputMode == REDCASSRV_5TH95TH_MODE) {
			nu = log(para1 * para2) / 2;
			tau = log(para2 / para1) / 3.28971;
			median = exp(nu);
			errfac = exp(z95 * tau);
		}
		else if (inputMode == REDCASSRV_MEDIANTAU_MODE) {
			median = para1;
			errfac = exp(z95 * para2);
		}
		else if (inputMode == REDCASSRV_NUTAU_MODE) {
			median = exp(para1);
			errfac = exp(z95 * para2);
		}

		return session->addScalePrior(0, 1, median, errfac);
	}

	return redcas_ErrorCode_InvalidHandle;
}

/*
* Class:     redcas_numerical_NativeWeibull2
* Method:    setRxCxScalePriorN
* Signature: (IDDD)I
*/
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_setRxCxScalePriorN
(JNIEnv *, jobject, jint iHandle, jdouble time, jdouble reliability, jdouble confidence)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		session->createCustomPrior(1, BRASSEngine::MEASURE_RELIABILITY);
		session->setCustomPriorRecord(0, time, reliability, confidence, 0.9);
		return session->attachCustomPrior(0, 1);
	}

	return redcas_ErrorCode_InvalidHandle;
}

/*
* Class:     redcas_numerical_NativeWeibull2
* Method:    setI1000ScalePriorN
* Signature: (IDDD)I
*/
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_setI1000ScalePriorN
(JNIEnv *, jobject, jint iHandle, jdouble time, jdouble lower, jdouble upper)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		session->createCustomPrior(1, BRASSEngine::MEASURE_IPER1000);
		session->setCustomPriorRecord(0, time, lower, upper, 0.9);
		return session->attachCustomPrior(0, 1);
	}

	return redcas_ErrorCode_InvalidHandle;
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    setI1000ScalePrior
 * Signature: (IDDD)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_setI1000ScalePrior
(JNIEnv *, jobject, jint iHandle, jdouble time, jdouble lower, jdouble upper)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		session->createCustomPrior(1, BRASSEngine::MEASURE_IPER1000);
		session->setCustomPriorRecord(0, time, lower, upper, 0.9);
		return session->attachCustomPrior(0, 1);
	}

	return redcas_ErrorCode_InvalidHandle;
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    generateSamples
 * Signature: (IIIIIJZ)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_generateSamples
(JNIEnv *, jobject, jint iHandle, jint samples, jint chains, jint interleave, jint burnin, jlong seed, jboolean clear)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->generateSamples(samples, chains, interleave, burnin, seed, clear);
	}

	return redcas_ErrorCode_InvalidHandle;
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    getStatusN
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_getStatusN
(JNIEnv *, jobject, jint iHandle)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->getStatus();
	}

	return redcas_ErrorCode_InvalidHandle;
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    getProgressN
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_getProgressN
(JNIEnv *, jobject, jint iHandle)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->getProgress();
	}

	return redcas_ErrorCode_InvalidHandle;
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    estimateJointParameterDensity
 * Signature: (IZI)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_estimateJointParameterDensity
(JNIEnv *, jobject, jint iHandle, jboolean log_scale, jint draw_points)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->estimateJointParameterDensity(0, 1, log_scale, draw_points);
	}

	return redcas_ErrorCode_InvalidHandle;
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    getJointDensities
 * Signature: (II[D[D[D)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_getJointDensities
(JNIEnv * env, jobject, jint iHandle, jint iPoints, jdoubleArray x, jdoubleArray y, jdoubleArray density)
{
	jdouble * xn = env->GetDoubleArrayElements(x, 0);
	jdouble * yn = env->GetDoubleArrayElements(y, 0);
	jdouble * densityn = env->GetDoubleArrayElements(density, 0);

	BRASSEngine * engine = getWeibullSession(iHandle);

	int k = 0;
	for (int i = 0; i < iPoints; i++) {
		for (int j = 0; j < iPoints; j++) {
			engine->getJointDensity(i, j, xn[i], yn[j], densityn[k++]);
		}
	}

	env->ReleaseDoubleArrayElements(x, xn, 0);
	env->ReleaseDoubleArrayElements(y, yn, 0);
	env->ReleaseDoubleArrayElements(density, densityn, 0);

	return 0;
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    computeParameter
 * Signature: (II[D[D[D[D[D[D[D[D)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_computeParameter
(JNIEnv * env, jobject, jint iHandle, jint param, jdoubleArray mean, jdoubleArray median,
jdoubleArray p01, jdoubleArray p05, jdoubleArray p10, jdoubleArray p90, jdoubleArray p95, jdoubleArray p99)
{
	jdouble * meann = env->GetDoubleArrayElements(mean, 0);
	jdouble * mediann = env->GetDoubleArrayElements(median, 0);
	jdouble * p01n = env->GetDoubleArrayElements(p01, 0);
	jdouble * p05n = env->GetDoubleArrayElements(p05, 0);
	jdouble * p10n = env->GetDoubleArrayElements(p10, 0);
	jdouble * p90n = env->GetDoubleArrayElements(p90, 0);
	jdouble * p95n = env->GetDoubleArrayElements(p95, 0);
	jdouble * p99n = env->GetDoubleArrayElements(p99, 0);

	double mn, md, q01, q05, q10, q90, q95, q99;

	BRASSEngine * engine = getWeibullSession(iHandle);

	int ret = engine->computeParameter(param, 0, 1, mn, md, q01, q05, q10, q90, q95, q99);

	meann[0] = mn;
	mediann[0] = md;
	p01n[0] = q01;
	p05n[0] = q05;
	p10n[0] = q10;
	p90n[0] = q90;
	p95n[0] = q95;
	p99n[0] = q99;

	env->ReleaseDoubleArrayElements(mean, meann, 0);
	env->ReleaseDoubleArrayElements(median, mediann, 0);
	env->ReleaseDoubleArrayElements(p01, p01n, 0);
	env->ReleaseDoubleArrayElements(p05, p05n, 0);
	env->ReleaseDoubleArrayElements(p10, p10n, 0);
	env->ReleaseDoubleArrayElements(p90, p90n, 0);
	env->ReleaseDoubleArrayElements(p95, p95n, 0);
	env->ReleaseDoubleArrayElements(p99, p99n, 0);

	return 0;
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    computeMeasures
 * Signature: (II[D[D[D[D[D[D[D[D[D)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeWeibull2_computeMeasures
(JNIEnv * env, jobject, jint iHandle, jint measure, jdoubleArray times,
jdoubleArray mean, jdoubleArray median, jdoubleArray p01, jdoubleArray p05, jdoubleArray p10,
jdoubleArray p90, jdoubleArray p95, jdoubleArray p99)
{
	jdouble * timesn = env->GetDoubleArrayElements(times, 0);
	jsize lenTimes = env->GetArrayLength(times);

	jdouble * meann = env->GetDoubleArrayElements(mean, 0);
	jdouble * mediann = env->GetDoubleArrayElements(median, 0);
	jdouble * p01n = env->GetDoubleArrayElements(p01, 0);
	jdouble * p05n = env->GetDoubleArrayElements(p05, 0);
	jdouble * p10n = env->GetDoubleArrayElements(p10, 0);
	jdouble * p90n = env->GetDoubleArrayElements(p90, 0);
	jdouble * p95n = env->GetDoubleArrayElements(p95, 0);
	jdouble * p99n = env->GetDoubleArrayElements(p99, 0);

	double time, mn, md, q01, q05, q10, q90, q95, q99;

	cout << "COMPUTE INPUT MEASURE " << measure << " NODE " << 0 << " MODE " << 1 << endl;

	BRASSEngine * engine = getWeibullSession(iHandle);

	for (int i = 0; i < lenTimes; i++) {
		time = timesn[i];

		int native_measure;

		switch (measure)
		{
		case brass_algorithmic_BRASSEngineNative_MEASURE_CUMULATIVE:
			native_measure = engine->MEASURE_IPER1000;
			break;
		case brass_algorithmic_BRASSEngineNative_MEASURE_RELIABILITY:
			native_measure = engine->MEASURE_RELIABILITY;
			break;
		case brass_algorithmic_BRASSEngineNative_MEASURE_UNRELIABILITY:
			native_measure = engine->MEASURE_RELIABILITY;
			break;
		case brass_algorithmic_BRASSEngineNative_MEASURE_INTENSITY:
			native_measure = engine->MEASURE_INTENSITY;
			break;
		}

		engine->computeMeasure(native_measure, 0, 1, time, mn, md, q01, q05, q10, q90, q95, q99);

		if (measure != brass_algorithmic_BRASSEngineNative_MEASURE_UNRELIABILITY) {
			meann[i] = mn;
			mediann[i] = md;
			p01n[i] = q01;
			p05n[i] = q05;
			p10n[i] = q10;
			p90n[i] = q90;
			p95n[i] = q95;
			p99n[i] = q99;
		}
		else {
			meann[i] = 1.0 - mn;
			mediann[i] = 1.0 - md;
			p01n[i] = 1.0 - q01;
			p05n[i] = 1.0 - q05;
			p10n[i] = 1.0 - q10;
			p90n[i] = 1.0 - q90;
			p95n[i] = 1.0 - q95;
			p99n[i] = 1.0 - q99;
		}
	}

	env->ReleaseDoubleArrayElements(times, timesn, 0);
	env->ReleaseDoubleArrayElements(mean, meann, 0);
	env->ReleaseDoubleArrayElements(median, mediann, 0);
	env->ReleaseDoubleArrayElements(p01, p01n, 0);
	env->ReleaseDoubleArrayElements(p05, p05n, 0);
	env->ReleaseDoubleArrayElements(p10, p10n, 0);
	env->ReleaseDoubleArrayElements(p90, p90n, 0);
	env->ReleaseDoubleArrayElements(p95, p95n, 0);
	env->ReleaseDoubleArrayElements(p99, p99n, 0);

	return 0;
}

/*
 * Class:     redcas_numerical_NativeWeibull2
 * Method:    computeWeibullMTTF
 * Signature: (IDD)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeWeibull2_computeWeibullMTTF
(JNIEnv *, jobject, jint iHandle, jdouble dShape, jdouble dScale)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->computeWeibullMTTF(dShape, dScale);
	}

	return redcas_ErrorCode_InvalidHandle;
}

#ifdef __cplusplus
}
#endif