// $Header: C:/cvsrepo/ptdev/native/redcas/redcas/redcas_numerical_NativeHomogeneous.cpp,v 1.2 2001/06/06 00:40:39 fgroen Exp $

/** 
 * The functions in this file provide the interfacing between
 * the native routines in redcas.numerical.NativeHomogeneous
 * and the native implementations of the updating procedures.
 */

#include "jni.h"
#include "SimpleHomogeneous.h"
#include <math.h>
#include <iostream>

using namespace std;

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

void copyNativeToJava(JNIEnv * env, jdoubleArray javaArr, double *nativeArr, int size);
void copyJavaToNative(JNIEnv * env, jdoubleArray javaArr, double *nativeArr, int size);

/*
 * TEST : Covered by AlgorithmTester.cpp. The test routine does not call
 * this method, but the updateXXXYYY() methods instead.
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeHomogeneous_parametric
  (JNIEnv * env, jclass, jint ptype, jdouble p1, jdouble p2, jint ltype, jdouble l1, jdouble l2, jdoubleArray posterior)
{
	int result = -1;
	int samples = 20000;

	double pos[2];

	switch(ptype) {

	case redcas_numerical_NativeHomogeneous_PRIOR_BETA:
		if (ltype == redcas_numerical_NativeHomogeneous_LIKELIHOOD_BINOMIAL) {
			// l1: demands, l2: events
			SimpleHomogeneous::updateBetaBinomial(p1,p2,l2,l1,pos[0],pos[1]);
			result = 0;
		}
		break;

	case redcas_numerical_NativeHomogeneous_PRIOR_GAMMA:
		if (ltype == redcas_numerical_NativeHomogeneous_LIKELIHOOD_POISSON) {
			SimpleHomogeneous::updateGammaPoisson(p1,p2,l1,l2,pos[0],pos[1]);
			result = 0;
		}
		break;

	case redcas_numerical_NativeHomogeneous_PRIOR_NORMAL:
		if (ltype == redcas_numerical_NativeHomogeneous_LIKELIHOOD_NORMAL) {
			SimpleHomogeneous::updateNormalNormal(p1,p2,l1,l2,pos[0],pos[1]);
			result = 0;
		}
		break;

	case redcas_numerical_NativeHomogeneous_PRIOR_LOGNORMAL:
		if (ltype == redcas_numerical_NativeHomogeneous_LIKELIHOOD_LOGNORMAL) {
			SimpleHomogeneous::updateLogNormalLogNormal(log(p1),p2,log(l1),l2,pos[0],pos[1]);
			pos[0] = exp(pos[0]);
			result = 0;
		}
		break;
	}

	if (result == 0) copyNativeToJava(env,posterior,pos,2);

	return result;
}

/*
 * TEST: preliminary test in SimpleHomogeneous.cpp
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeHomogeneous_logBinomial
  (JNIEnv * env, jobject, jdouble nu, jdouble tau, jdouble k, jdouble n, jdoubleArray vout, jdoubleArray pout, jint size)
{
	double * values = new double[size];
	double * probs = new double[size];

	int samples = 40000;
	int retval = 0;
	double integral,mean,var;

	try {
		retval = SimpleHomogeneous::updateLogNormalBinomial(nu,tau,k,n,samples,values,probs,size,integral,mean,var);
		copyNativeToJava(env,vout,values,size);
		copyNativeToJava(env,pout,probs,size);
	} catch (int e) {
		retval = e;
	}

	delete [] values;
	delete [] probs;

	return retval;
}

/*
 * TEST: preliminary test in SimpleHomogeneous.cpp
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeHomogeneous_logBinomialEx
  (JNIEnv * env, jobject, jdouble nu, jdouble tau, jdouble k, jdouble n, jdoubleArray vout, jdoubleArray pout, jint size, jdoubleArray norm)
{
	double * values = new double[size];
	double * probs = new double[size];

	int samples = 40000;
	int retval = 0;
	double rslt[3];

	try {
		retval = SimpleHomogeneous::updateLogNormalBinomial(nu,tau,k,n,samples,values,probs,size,rslt[0],rslt[1],rslt[2]);
		copyNativeToJava(env,vout,values,size);
		copyNativeToJava(env,pout,probs,size);
		copyNativeToJava(env,norm,rslt,3);
	} catch (int e) {
		retval = e;
	} 

	delete [] values;
	delete [] probs;

	return retval;
}

/*
 * TEST: preliminary test in SimpleHomogeneous.cpp
 * DO NOT USE THIS VERSION! USE Java_redcas_numerical_NativeHomogeneous_logPoissonEx instead
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeHomogeneous_logPoisson
  (JNIEnv * env, jobject, jdouble nu, jdouble tau, jdouble k, jdouble t, jdoubleArray vout, jdoubleArray pout, jint size)
{
	double * values = new double[size];
	double * probs = new double[size];

	int samples = 40000;
	int retval = 0;
	double integral,mean,var;

	try {
		retval = SimpleHomogeneous::updateLogNormalPoisson(nu,tau,k,t,samples,values,probs,size,integral,mean,var);
		copyNativeToJava(env,vout,values,size);
		copyNativeToJava(env,pout,probs,size);
	} catch (int e) {
		retval = e;
	}

	delete [] values;
	delete [] probs;

	return retval;
}

/*
 * TEST: preliminary test in SimpleHomogeneous.cpp
 * 
 * INPUTS:
 *  nu : log(median) of the lognormal distribution
 *  tau: sd of the underlying normal
 *  k: number of events
 *  t: time
 *  vout : array in which x values are stored
 *  pout : array in which cdf values are stored
 *  size : size of the array
 *  norm : array in which integral is stored 
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeHomogeneous_logPoissonEx
  (JNIEnv * env, jobject, jdouble nu, jdouble tau, jdouble k, jdouble t, jdoubleArray vout, jdoubleArray pout, jint size, jdoubleArray norm)
{
	double * values = new double[size];
	double * probs = new double[size];

	int samples = 40000;
	int retval = 0;
	double rslt[3];

	try {
		cout << "Starting LP update" << endl;
		retval = SimpleHomogeneous::updateLogNormalPoisson(nu,tau,k,t,samples,values,probs,size,rslt[0],rslt[1],rslt[2]);
		cout << "Update complete" << endl;
		copyNativeToJava(env,vout,values,size);
		cout << "Copied values" << endl;
		copyNativeToJava(env,pout,probs,size);
		cout << "Copied cdfs" << endl;
		copyNativeToJava(env,norm,rslt,3);
		cout << "Copied stats" << endl;
	} catch (int e) {
		retval = e;
	}

	delete [] values;
	delete [] probs;

	return retval;
}

/*
 * TEST : Covered by AlgorithmTester.cpp. The test routine does not call
 * this method, but the updateTabularYYY() methods instead.
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeHomogeneous_tabular
  (JNIEnv * env, jobject, jdoubleArray vin, jdoubleArray pin, jint size, jint like, jdouble p1, jdouble p2, jdoubleArray vout, jdoubleArray pout)
{
	double * vinNative = new double[size];
	double * pinNative = new double[size];
	double * voutNative = new double[size];
	double * poutNative = new double[size];

	int samples = 20000;
	int retval = 0;

	copyJavaToNative(env,vin,vinNative,size);
	copyJavaToNative(env,pin,pinNative,size);
	
	switch(like) {
	case redcas_numerical_NativeHomogeneous_LIKELIHOOD_POISSON:
		retval = SimpleHomogeneous::updateTabularPoisson(vinNative,pinNative,size,p1,p2,samples,voutNative,poutNative,size);
		break;
	case redcas_numerical_NativeHomogeneous_LIKELIHOOD_BINOMIAL:
		retval = SimpleHomogeneous::updateTabularBinomial(vinNative,pinNative,size,p1,p2,samples,voutNative,poutNative,size);
		break;
	case redcas_numerical_NativeHomogeneous_LIKELIHOOD_NORMAL:
		retval = SimpleHomogeneous::updateTabularNormal(vinNative,pinNative,size,p1,p2,samples,voutNative,poutNative,size);
		break;
	case redcas_numerical_NativeHomogeneous_LIKELIHOOD_LOGNORMAL:
		retval = SimpleHomogeneous::updateTabularLogNormal(vinNative,pinNative,size,p1,p2,samples,voutNative,poutNative,size);
		break;
	}	

	if (retval != 0) {
		for (int i = 0 ; i < size ; i++) {
			poutNative[i] = i * 1. / (size - 1);
			voutNative[i] = vinNative[0] + (vinNative[size - 1] - vinNative[0]) / (size - 1);
		}
	};

	copyNativeToJava(env,vout,voutNative,size);
	copyNativeToJava(env,pout,poutNative,size);

	delete [] poutNative;
	delete [] voutNative;
	delete [] pinNative;
	delete [] vinNative;

	return 0;
}

void copyNativeToJava(JNIEnv * env, jdoubleArray javaArr, double *nativeArr, int size) {
	jdouble *body = (env)->GetDoubleArrayElements(javaArr, 0);
	for (int i = 0 ; i < size ; i++) body[i] = nativeArr[i];
	(env)->ReleaseDoubleArrayElements(javaArr,body,0);
}

void copyJavaToNative(JNIEnv * env, jdoubleArray javaArr, double *nativeArr, int size) {
	jdouble *body = (env)->GetDoubleArrayElements(javaArr, 0);
	for (int i = 0 ; i < size ; i++) nativeArr[i] = body[i];
	(env)->ReleaseDoubleArrayElements(javaArr,body,0);
}

#ifdef __cplusplus
}
#endif
#endif
