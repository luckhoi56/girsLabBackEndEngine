// $Header: C:/cvsrepo/ptdev/native/redcas/redcas/redcas_numerical_NativeBeta.cpp,v 1.11 2002/06/25 02:47:43 fgroen Exp $

#include "redcas_numerical_NativeBeta.h"
#include "BetaDistribution.h"
#include "Beta0595InputMode.h"
#include <iostream>


#include "redcas_numerical_NativeDistributionMgr.h"
#include "DistributionManager.h"
#include "ParametricDistribution.h"
#include "UniformBinomialDistribution.h"
#include "UniformPoissonDistribution.h"
#include "LogNormalBinomialDistribution.h"
#include "LogNormalPoissonDistribution.h"

#include "TabularDistribution.h"

#include <iostream>

#ifdef __cplusplus
extern "C" {
#endif
void copyNativeToJava(JNIEnv * env, jdoubleArray javaArr, double *nativeArr, int size);

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeBeta_getPDF
  (JNIEnv *, jclass, jdouble alpha, jdouble beta, jdouble x)
{
	double result = 0;
	try {
		result = BetaDistribution(alpha,beta).getPDF(x);
	} catch (...) {	}

	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeBeta_getCDF
  (JNIEnv *, jclass, jdouble alpha, jdouble beta, jdouble x)
{
	double result = 0;
	try {
		result = BetaDistribution(alpha,beta).getCDF(x);
	} catch (...) { }
	return result;
}


/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeBeta_getQuantile
  (JNIEnv *, jclass, jdouble alpha, jdouble beta, jdouble x)
{
	double result = 0;
	try {
		result = BetaDistribution(alpha,beta).getQuantile(x);
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeBeta_getMean
  (JNIEnv *, jclass, jdouble alpha, jdouble beta)
{
	double result = 0;
	try {
		result = BetaDistribution(alpha,beta).getMean();
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeBeta_getMedian
  (JNIEnv *, jclass, jdouble alpha, jdouble beta)
{
	double result = 0;
	try {
		result = BetaDistribution(alpha,beta).getMedian();
	} catch (...) { }
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeBeta_getVariance
  (JNIEnv *, jclass, jdouble alpha, jdouble beta)
{
	double result = 0;
	try {
		result = BetaDistribution(alpha,beta).getVariance();
	} catch (...) { }
	return result;
}

/*
 * TEST : none
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeBeta_solve
  (JNIEnv * env, jclass, jdouble zlo, jdouble qlo, jdouble zhi, jdouble qhi, jdoubleArray out)
{

//	cout << "Fitting Beta parameters for " << qlo << "," << qhi << endl;

	// currently only supports zlo = 0.05 and zhi = 0.95
	Beta0595InputMode mode(zlo,zhi);
	double native_arr[2];
	int result;
	try {
	   result = mode.convert(qlo,qhi,native_arr[0],native_arr[1]);

//	   cout << "Returning " << native_arr[0] << " " << native_arr[1] << " : " << result << endl;

	   copyNativeToJava(env,out,native_arr,2);	
	} catch (...) {
//		cout << "Exception occurred during Beta percentile fit" << endl;
		result = 1;
	}
	return result;
}

/*
 * TEST : Covered by AlgorithmTester.cpp
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeBeta_getSample
  (JNIEnv *, jclass, jdouble alpha, jdouble beta)
{
	double result = 0;
	try {
		result = BetaDistribution(alpha,beta).getSample();
	} catch (...) { }
	return result;
}


/////////////////////////////////////////////////////////////////////////////


/*
 * Class:     redcas_numerical_NativeDistributionMgr
 * Method:    create
 * Signature: (IDDDD)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeDistributionMgr_create
  (JNIEnv *, jclass, jint type, jdouble p1, jdouble p2, jdouble p3, jdouble p4)
{
	
	ParametricDistribution * dist = 0;

	switch (type) {

	case redcas_numerical_NativeDistributionMgr_UNIFORM_BINOMIAL:
		dist = new UniformBinomialDistribution(p1,p2,p3,p4,false);
		break;
	case redcas_numerical_NativeDistributionMgr_UNIFORM_POISSON:
		dist = new UniformPoissonDistribution(p1,p2,p3,p4,false);
		break;
	case redcas_numerical_NativeDistributionMgr_LOGUNIFORM_BINOMIAL:
		dist = new UniformBinomialDistribution(p1,p2,p3,p4,true);
		break;
	case redcas_numerical_NativeDistributionMgr_LOGUNIFORM_POISSON:
		dist = new UniformPoissonDistribution(p1,p2,p3,p4,true);
		break;
	case redcas_numerical_NativeDistributionMgr_LOGNORMAL_BINOMIAL:
		dist = new LogNormalBinomialDistribution(p1,p2,p3,p4);
		break;
	case redcas_numerical_NativeDistributionMgr_LOGNORMAL_POISSON:
		dist = new LogNormalPoissonDistribution(p1,p2,p3,p4);
		break;
	default: 
		dist = 0;
		break;
	}

	if (dist == 0) return -1;

	int retval = DistributionManager::getInstance()->add(dist);
	if (retval == -1) {
		delete dist;
		return -1;
	} else return retval + 1;
}


/*
 * Class:     redcas_numerical_NativeDistributionMgr
 * Method:    destroy
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_redcas_numerical_NativeDistributionMgr_destroy
  (JNIEnv *, jclass, jint handle)
{
	ParametricDistribution * dist = DistributionManager::getInstance()->remove(handle - 1);
	delete dist;
}

/*
 * Class:     redcas_numerical_NativeDistributionMgr
 * Method:    getPDF
 * Signature: (ID)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeDistributionMgr_getPDF
  (JNIEnv *, jclass, jint handle, jdouble x)
{
	double retval = 0;
	ParametricDistribution * dist = DistributionManager::getInstance()->retrieve(handle - 1);
	if (dist != 0) {
		retval = dist->getPDF(x);
	}
	return retval;
}

/*
 * Class:     redcas_numerical_NativeDistributionMgr
 * Method:    getCDF
 * Signature: (ID)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeDistributionMgr_getCDF
  (JNIEnv *, jclass, jint handle, jdouble x)
{
	double retval = 0;
	ParametricDistribution * dist = DistributionManager::getInstance()->retrieve(handle - 1);
	if (dist != 0) {
		retval = dist->getCDF(x);
	}
	return retval;
}


/*
 * Class:     redcas_numerical_NativeDistributionMgr
 * Method:    getCDF
 * Signature: (ID)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeDistributionMgr_getTabularCDF
  (JNIEnv *, jclass, jint handle, jint index)
{
	double retval = 0;
	ParametricDistribution * dist = DistributionManager::getInstance()->retrieve(handle - 1);
	if (dist != 0) {
		// assume that the calls are made correctly... somewhat unsafe downcast operation
		TabularDistribution * tab = static_cast<TabularDistribution*>(dist);
		retval = (tab != 0) ? tab->getCDF(index) : 0;
	}
	return retval;
}


/*
 * Class:     redcas_numerical_NativeDistributionMgr
 * Method:    getQuantile
 * Signature: (ID)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeDistributionMgr_getQuantile
  (JNIEnv *, jclass, jint handle, jdouble z)
{
	double retval = 0;
	ParametricDistribution * dist = DistributionManager::getInstance()->retrieve(handle - 1);
	if (dist != 0) {
		retval = dist->getQuantile(z);
	}
	return retval;
}

/*
 * Class:     redcas_numerical_NativeDistributionMgr
 * Method:    getMean
 * Signature: (I)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeDistributionMgr_getMean
  (JNIEnv *, jclass, jint handle)
{
	double retval = 0;
	ParametricDistribution * dist = DistributionManager::getInstance()->retrieve(handle - 1);
	if (dist != 0) {
		retval = dist->getMean();
	}
	return retval;
}

/*
 * Class:     redcas_numerical_NativeDistributionMgr
 * Method:    getMedian
 * Signature: (I)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeDistributionMgr_getMedian
  (JNIEnv *, jclass, jint handle)
{
	double retval = 0;
	ParametricDistribution * dist = DistributionManager::getInstance()->retrieve(handle - 1);
	if (dist != 0) {
		retval = dist->getMedian();
	}
	return retval;
}

/*
 * Class:     redcas_numerical_NativeDistributionMgr
 * Method:    getVariance
 * Signature: (I)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeDistributionMgr_getVariance
  (JNIEnv *, jclass, jint handle)
{
	double retval = 0;
	ParametricDistribution * dist = DistributionManager::getInstance()->retrieve(handle - 1);
	if (dist != 0) {
		retval = dist->getVariance();
	}
	return retval;
}

/*
 * Class:     redcas_numerical_NativeDistributionMgr
 * Method:    getSample
 * Signature: (I)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeDistributionMgr_getSample
  (JNIEnv *, jclass, jint handle)
{
	double retval = 0;
	ParametricDistribution * dist = DistributionManager::getInstance()->retrieve(handle - 1);
	if (dist != 0) {
		retval = dist->getSample();
	}
	return retval;
}

/*
 * Class:     redcas_numerical_NativeDistributionMgr
 * Method:    createTabular
 * Signature: (I[D[DDD)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeDistributionMgr_createTabular
  (JNIEnv * env, jclass, jint jtype, jdoubleArray jvals, jdoubleArray jcdfs, jdouble jp1, jdouble jp2)
{

	ParametricDistribution * dist = 0;

	int len = env->GetArrayLength(jvals);

	double * nvals = env->GetDoubleArrayElements(jvals,NULL);
	double * ncdfs = env->GetDoubleArrayElements(jcdfs,NULL);

	switch (jtype) {

	case redcas_numerical_NativeDistributionMgr_LOGTAB:
		dist = new TabularDistribution(nvals, ncdfs, len, TABULAR_LOG);
		break;
	case redcas_numerical_NativeDistributionMgr_LINTAB:
		dist = new TabularDistribution(nvals, ncdfs, len, TABULAR_LIN);
		break;
	case redcas_numerical_NativeDistributionMgr_LINTAB_POISSON:
		dist = new TabularDistribution(nvals, ncdfs, len, TABULAR_LIN_POISSON,jp1,jp2);
		break;
	case redcas_numerical_NativeDistributionMgr_LOGTAB_POISSON:
		dist = new TabularDistribution(nvals, ncdfs, len, TABULAR_LOG_POISSON,jp1,jp2);
		break;
	case redcas_numerical_NativeDistributionMgr_LINTAB_BINOMIAL:
		dist = new TabularDistribution(nvals, ncdfs, len, TABULAR_LIN_BINOMIAL,jp1,jp2);
		break;
	case redcas_numerical_NativeDistributionMgr_LOGTAB_BINOMIAL:
		dist = new TabularDistribution(nvals, ncdfs, len, TABULAR_LOG_BINOMIAL,jp1,jp2);
		break;
	case redcas_numerical_NativeDistributionMgr_LINTAB_LOGNORMAL:
		dist = new TabularDistribution(nvals, ncdfs, len, TABULAR_LIN_LOGNORMAL,jp1,jp2);
		break;
	case redcas_numerical_NativeDistributionMgr_LOGTAB_LOGNORMAL:
		dist = new TabularDistribution(nvals, ncdfs, len, TABULAR_LOG_LOGNORMAL,jp1,jp2);
		break;
	case redcas_numerical_NativeDistributionMgr_LINTAB_NORMAL:
		dist = new TabularDistribution(nvals, ncdfs, len, TABULAR_LIN_NORMAL,jp1,jp2);
		break;

	default: 
		dist = 0;
		break;
	}

//		cout << "ADDING LOGTAB RANGING BETWEEN " << nvals[0] << " AND " << nvals[len-1] << ", LENGTH IS " << len << endl;
//		cout << "MEAN    : " << dist->getMean() << endl;
//		cout << "VARIANCE: " << dist->getVariance() << endl;
//		cout << "p05     : " << dist->getQuantile(0.05) << endl;
//		cout << "p50     : " << dist->getQuantile(0.50) << endl;
//		cout << "p95     : " << dist->getQuantile(0.95) << endl;

	env->ReleaseDoubleArrayElements(jcdfs,ncdfs,0);
	env->ReleaseDoubleArrayElements(jvals,nvals,0);

	if (dist == 0) return -1;

	int retval = DistributionManager::getInstance()->add(dist);

//	cout << "ASSIGNED HANDLE " << retval << endl;

	if (retval == -1) {
		delete dist;
		return -1;
	} else return retval + 1;
}


JNIEXPORT jint JNICALL Java_redcas_numerical_NativeDistributionMgr_createTabular4
  (JNIEnv * env, jclass, jint jtype, jdoubleArray jvals, jdoubleArray jcdfs, jdouble jp1, jdouble jp2, jdouble jp3, jdouble jp4)
{

	ParametricDistribution * dist = 0;

	int len = env->GetArrayLength(jvals);

	double * nvals = env->GetDoubleArrayElements(jvals,NULL);
	double * ncdfs = env->GetDoubleArrayElements(jcdfs,NULL);

	switch (jtype) {

	case redcas_numerical_NativeDistributionMgr_LINTAB_LOGNORMAL_POISSON:
		dist = new TabularDistribution(nvals, ncdfs, len, TABULAR_LIN_LOGNORMAL_POISSON,jp1,jp2,jp3,jp4);
		break;
	case redcas_numerical_NativeDistributionMgr_LOGTAB_LOGNORMAL_POISSON:
		dist = new TabularDistribution(nvals, ncdfs, len, TABULAR_LOG_LOGNORMAL_POISSON,jp1,jp2,jp3,jp4);
		break;
	case redcas_numerical_NativeDistributionMgr_LINTAB_LOGNORMAL_BINOMIAL:
		dist = new TabularDistribution(nvals, ncdfs, len, TABULAR_LIN_LOGNORMAL_BINOMIAL,jp1,jp2,jp3,jp4);
		break;
	case redcas_numerical_NativeDistributionMgr_LOGTAB_LOGNORMAL_BINOMIAL:
		dist = new TabularDistribution(nvals, ncdfs, len, TABULAR_LOG_LOGNORMAL_BINOMIAL,jp1,jp2,jp3,jp4);
		break;

	default: 
		dist = 0;
		break;
	}

//		cout << "ADDING LOGTAB RANGING BETWEEN " << nvals[0] << " AND " << nvals[len-1] << ", LENGTH IS " << len << endl;
//		cout << "MEAN    : " << dist->getMean() << endl;
//		cout << "VARIANCE: " << dist->getVariance() << endl;
//		cout << "p05     : " << dist->getQuantile(0.05) << endl;
//		cout << "p50     : " << dist->getQuantile(0.50) << endl;
//		cout << "p95     : " << dist->getQuantile(0.95) << endl;

	env->ReleaseDoubleArrayElements(jcdfs,ncdfs,0);
	env->ReleaseDoubleArrayElements(jvals,nvals,0);

	if (dist == 0) return -1;

	int retval = DistributionManager::getInstance()->add(dist);

//	cout << "ASSIGNED HANDLE " << retval << endl;

	if (retval == -1) {
		delete dist;
		return -1;
	} else return retval + 1;
}


#ifdef __cplusplus
}
#endif

