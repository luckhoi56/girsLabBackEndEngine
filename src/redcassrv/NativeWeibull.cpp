
#include "fiat_rdatsrv_weibull_NativeWeibull.h"
#include "rdatsrv_Weibull.h"
#include "brassengine.h"
#include "rdatdefs.h"

#undef brass_algorithmic_BRASSEngineNative_MEASURE_CUMULATIVE
#define brass_algorithmic_BRASSEngineNative_MEASURE_CUMULATIVE 12L
#undef brass_algorithmic_BRASSEngineNative_MEASURE_INTENSITY
#define brass_algorithmic_BRASSEngineNative_MEASURE_INTENSITY 13L
#undef brass_algorithmic_BRASSEngineNative_MEASURE_RELIABILITY
#define brass_algorithmic_BRASSEngineNative_MEASURE_RELIABILITY 11L
#undef brass_algorithmic_BRASSEngineNative_MEASURE_UNRELIABILITY
#define brass_algorithmic_BRASSEngineNative_MEASURE_UNRELIABILITY 20L

/*
 * Class:     fiat_rdatsrv_weibull_NativeWeibull
 * Method:    createWeibullSession
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_weibull_NativeWeibull_createWeibullSession
(JNIEnv *, jobject)
{
	return CRedcassrvWeibull::getInstance()->createWeibullSession();
}

/*
 * Class:     fiat_rdatsrv_weibull_NativeWeibull
 * Method:    destroyWeibullSession
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_fiat_rdatsrv_weibull_NativeWeibull_destroyWeibullSession
(JNIEnv *, jobject, jint iHandle)
{
	CRedcassrvWeibull::getInstance()->destroyWeibullSession(iHandle);
}

/*
 * Class:     fiat_rdatsrv_weibull_NativeWeibull
 * Method:    initialize
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_weibull_NativeWeibull_initialize
(JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvWeibull::getInstance()->initialize(iHandle);
}

/*
 * Class:     fiat_rdatsrv_weibull_NativeWeibull
 * Method:    createTestDataSet
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_weibull_NativeWeibull_createTestDataSet
(JNIEnv *, jobject, jint iHandle, jint record_cnt)
{
	return CRedcassrvWeibull::getInstance()->createTestDataSet(iHandle, record_cnt);
}

/*
 * Class:     fiat_rdatsrv_weibull_NativeWeibull
 * Method:    setTestDataRecord
 * Signature: (IIDDZ)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_weibull_NativeWeibull_setTestDataRecord
(JNIEnv *, jobject, jint iHandle, jint index, jdouble start, jdouble end, jboolean bSuccess)
{
	return CRedcassrvWeibull::getInstance()->setTestDataRecord(iHandle, index, start, end, bSuccess);
}

/*
 * Class:     fiat_rdatsrv_weibull_NativeWeibull
 * Method:    attachTestDataSet
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_weibull_NativeWeibull_attachTestDataSet
(JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvWeibull::getInstance()->attachTestDataSet(iHandle);
}

/*
 * Class:     fiat_rdatsrv_weibull_NativeWeibull
 * Method:    createGroupedDataSet
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_weibull_NativeWeibull_createGroupedDataSet
(JNIEnv *, jobject, jint iHandle, jint record_cnt)
{
	return CRedcassrvWeibull::getInstance()->createGroupedDataSet(iHandle, record_cnt);
}

/*
 * Class:     fiat_rdatsrv_weibull_NativeWeibull
 * Method:    setGroupedDataRecord
 * Signature: (IIDDII)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_weibull_NativeWeibull_setGroupedDataRecord
(JNIEnv *, jobject, jint iHandle, jint index, jdouble start, jdouble end, jint population, jint failures)
{
	return CRedcassrvWeibull::getInstance()->setGroupedDataRecord(iHandle, index, start, end, population, failures);
}

/*
 * Class:     fiat_rdatsrv_weibull_NativeWeibull
 * Method:    attachGroupedDataSet
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_weibull_NativeWeibull_attachGroupedDataSet
(JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvWeibull::getInstance()->attachGroupedDataSet(iHandle);
}

/*
* Class:     fiat_rdatsrv_weibull_NativeWeibull
* Method:    setShapePrior
* Signature: (IIDD)I
*/
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_weibull_NativeWeibull_setShapePrior
(JNIEnv *, jobject, jint iHandle, jint inputMode, jdouble para1, jdouble para2)
{
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

	return CRedcassrvWeibull::getInstance()->setShapePrior(iHandle, median, errfac);
}

/*
* Class:     fiat_rdatsrv_weibull_NativeWeibull
* Method:    setLogNormalScalePrior
* Signature: (IIDD)I
*/
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_weibull_NativeWeibull_setLogNormalScalePrior
(JNIEnv *, jobject, jint iHandle, jint inputMode, jdouble para1, jdouble para2)
{
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

	return CRedcassrvWeibull::getInstance()->setLogNormalScalePrior(iHandle, median, errfac);
}

/*
 * Class:     fiat_rdatsrv_weibull_NativeWeibull
 * Method:    setRxCxScalePrior
 * Signature: (IDDD)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_weibull_NativeWeibull_setRxCxScalePrior
(JNIEnv *, jobject, jint iHandle, jdouble time, jdouble reliability, jdouble confidence)
{
	return CRedcassrvWeibull::getInstance()->setRxCxScalePrior(iHandle, time, reliability, confidence);
}

/*
 * Class:     fiat_rdatsrv_weibull_NativeWeibull
 * Method:    setI1000ScalePrior
 * Signature: (IDDD)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_weibull_NativeWeibull_setI1000ScalePrior
(JNIEnv *, jobject, jint iHandle, jdouble time, jdouble lower, jdouble upper)
{
	return CRedcassrvWeibull::getInstance()->setI1000ScalePrior(iHandle, time, lower, upper);
}

/*
 * Class:     fiat_rdatsrv_weibull_NativeWeibull
 * Method:    generateSamples
 * Signature: (IIIIIJZ)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_weibull_NativeWeibull_generateSamples
(JNIEnv *, jobject, jint iHandle, jint samples, jint chains, jint interleave, jint burnin, jlong seed, jboolean clear)
{
	return CRedcassrvWeibull::getInstance()->generateSamples(iHandle, samples, chains,
		interleave, burnin, seed, clear);
}

/*
 * Class:     fiat_rdatsrv_weibull_NativeWeibull
 * Method:    getStatus
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_weibull_NativeWeibull_getStatus
(JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvWeibull::getInstance()->getStatus(iHandle);
}

/*
 * Class:     fiat_rdatsrv_weibull_NativeWeibull
 * Method:    getProgress
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_weibull_NativeWeibull_getProgress
(JNIEnv *, jobject, jint iHandle)
{
	return CRedcassrvWeibull::getInstance()->getProgress(iHandle);
}

/*
 * Class:     fiat_rdatsrv_weibull_NativeWeibull
 * Method:    estimateJointParameterDensity
 * Signature: (IZI)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_weibull_NativeWeibull_estimateJointParameterDensity
(JNIEnv *, jobject, jint iHandle, jboolean log_scale, jint draw_points)
{
	return CRedcassrvWeibull::getInstance()->estimateJointParameterDensity(iHandle, log_scale, draw_points);
}

/*
 * Class:     fiat_rdatsrv_weibull_NativeWeibull
 * Method:    getJointDensities
 * Signature: (II[D[D[D)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_weibull_NativeWeibull_getJointDensities
(JNIEnv * env, jobject, jint iHandle, jint iPoints, jdoubleArray x, jdoubleArray y, jdoubleArray density)
{
	jdouble * xn = env->GetDoubleArrayElements(x, 0);
	jdouble * yn = env->GetDoubleArrayElements(y, 0);
	jdouble * densityn = env->GetDoubleArrayElements(density, 0);

	BRASSEngine * engine = CRedcassrvWeibull::getInstance()->getWeibullSession(iHandle);

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
 * Class:     fiat_rdatsrv_weibull_NativeWeibull
 * Method:    computeParameter
 * Signature: (II[D[D[D[D[D[D[D[D)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_weibull_NativeWeibull_computeParameter
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

	BRASSEngine * engine = CRedcassrvWeibull::getInstance()->getWeibullSession(iHandle);

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
 * Class:     fiat_rdatsrv_weibull_NativeWeibull
 * Method:    computeMeasures
 * Signature: (II[D[D[D[D[D[D[D[D[D)I
 */
JNIEXPORT jint JNICALL Java_fiat_rdatsrv_weibull_NativeWeibull_computeMeasures
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

	BRASSEngine * engine = CRedcassrvWeibull::getInstance()->getWeibullSession(iHandle);

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
 * Class:     fiat_rdatsrv_weibull_NativeWeibull
 * Method:    computeWeibullMTTF
 * Signature: (IDD)D
 */
JNIEXPORT jdouble JNICALL Java_fiat_rdatsrv_weibull_NativeWeibull_computeWeibullMTTF
(JNIEnv *, jobject, jint iHandle, jdouble dShape, jdouble dScale)
{
	return CRedcassrvWeibull::getInstance()->computeWeibullMTTF(iHandle, dShape, dScale);
}
