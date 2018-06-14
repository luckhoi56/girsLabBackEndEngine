#include "stdafx.h"
#include <stdlib.h>
#include "redcas_numerical_NativeNonHomog2.h"
#include "SessionManager.h"
#include "jni.h"
#include "Status.h"
#include "NHSession.h"
#include <math.h>
                             

#include <iostream>

            
#ifdef __cplusplus
extern "C"
{
#endif

/****************************************************************/


NHSession * getNHSession(int index) 
{
	return static_cast<NHSession *>
		(SessionManager::getInstance()->retrieveSession(index));
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    createSessionN
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeNonHomog2_createSessionN
  (JNIEnv *, jclass)
{
	return SessionManager::getInstance()->createSession(NONHOMOG_SESSION);
}
/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    destroySessionN
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_redcas_numerical_NativeNonHomog2_destroySessionN
  (JNIEnv *, jclass, jint session)
{
	SessionManager::getInstance()->destroySession(session);
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    clearDataN
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeNonHomog2_clearDataN
  (JNIEnv *, jclass, jint sessionID)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		session->clearData();		
	}
	return 0;
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    addDataPointN
 * Signature: (IDD)V
 */
JNIEXPORT void JNICALL Java_redcas_numerical_NativeNonHomog2_addDataPointN
  (JNIEnv *, jclass, jint sessionID, jdouble v1, jdouble v2, jdouble v3, jdouble v4)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		session->addDataPoint(v1,v2,v3,v4);
#ifdef _DEBUG
		cout << "DLL added datapoint " << v1 << " " << v2 << endl;
#endif

	}
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    setModel
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_redcas_numerical_NativeNonHomog2_setModel
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    clearSamplesN
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeNonHomog2_clearSamplesN
  (JNIEnv *, jclass, jint sessionID)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		session->clearSamplesEx();		
	}
	return 0;
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    addSampleN
 * Signature: (IDDD)V
 */
JNIEXPORT void JNICALL Java_redcas_numerical_NativeNonHomog2_addSampleN
  (JNIEnv *, jclass, jint sessionID, jdouble alpha, jdouble beta, jdouble prob)
{
	NHSession * session = getNHSession(sessionID);

	if (session != 0L) {
		session->addSample(alpha,beta,prob);
	}
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    getMeasureN
 * Signature: (IIDID)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeNonHomog2_getMeasureN
  (JNIEnv *, jclass, jint sessionID, jint param, jdouble value, jint measure, jdouble q)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		// NHMeasures msr;
		NHParameters parameter = dflt;
		double val;
		switch (measure) {
		case redcas_numerical_NativeNonHomog2_MEASURE_CDF :
			val = session->getMeasure(parameter,value,cdf,q);
			break;
		case redcas_numerical_NativeNonHomog2_MEASURE_PDF :
			val = session->getMeasure(parameter,value,pdf,q);
			break;
		case redcas_numerical_NativeNonHomog2_MEASURE_MEAN :
			// special case: return the percentile of the mean distribution
			// not the mean itself.
			val = session->findMeanPercentile(q);
			break;
		case redcas_numerical_NativeNonHomog2_MEASURE_VARIANCE :
			val = session->getMeasure(parameter,value,variance,q);
			break;
		case redcas_numerical_NativeNonHomog2_MEASURE_QUANTILE:
			val = session->getMeasure(parameter,value,quantile,q);
			break;
		}
		
		return val; 
	}
	return 0;
}


/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    getMeasureMeanN
 * Signature: (IIDI)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeNonHomog2_getMeasureMeanN
  (JNIEnv *, jclass, jint sessionID, jint param, jdouble value, jint measure)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {

		NHParameters parameter = dflt;
		double mn;

		switch (measure) {
		case redcas_numerical_NativeNonHomog2_MEASURE_CDF :
			mn = session->getMeasureMean(parameter,value,cdf);
			break;
		case redcas_numerical_NativeNonHomog2_MEASURE_PDF :
			mn = session->getMeasureMean(parameter,value,pdf);
			break;
		case redcas_numerical_NativeNonHomog2_MEASURE_MEAN :
			mn = session->getMeasureMean(parameter,value,mean);
			break;
		case redcas_numerical_NativeNonHomog2_MEASURE_VARIANCE :
			mn = session->getMeasureMean(parameter,value,variance);
			break;
		case redcas_numerical_NativeNonHomog2_MEASURE_QUANTILE:
			mn = session->getMeasureMean(parameter,value,quantile);
			break;
		}
	
		return mn;
	}
	return 0;
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    getUpperBoundN
 * Signature: (II)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeNonHomog2_getUpperBoundN
  (JNIEnv *, jclass, jint sessionID, jint param)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		return session->getPopVarUpperBound();
	}
	return 0;
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    getLowerBoundN
 * Signature: (II)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeNonHomog2_getLowerBoundN
  (JNIEnv *, jclass, jint sessionID, jint param)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		return session->getPopVarLowerBound();
	}
	return 0;
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    setSampleCountN
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeNonHomog2_setSampleCountN
  (JNIEnv *, jclass, jint sessionID, jint samples)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		session->setSampleCount(samples);
	}
	return 0;
}


/*
 * Class:     redcas_numerical_NativeBeta
 * Method:    getCDF
 * Signature: (DDD)D
 */
JNIEXPORT void JNICALL Java_redcas_numerical_NativeNonHomog2_setModelN
  (JNIEnv *, jclass, jint sessionID, jint model)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		NHModels newModel;
		switch (model) {
			case redcas_numerical_NativeNonHomog2_BETA_BINOMIAL : newModel = betbin; break;
			case redcas_numerical_NativeNonHomog2_GAMMA_POISSON : newModel = gampois; break;
			case redcas_numerical_NativeNonHomog2_LOGNORM_POISSON : newModel = logpois; break;
			case redcas_numerical_NativeNonHomog2_LOGNORM_BINOMIAL : newModel = logbin; break;
			
			// following types are deprecated
			case redcas_numerical_NativeNonHomog2_LOGNORM_LOGNORM : newModel = logpois; break;
			case redcas_numerical_NativeNonHomog2_NORMAL_NORMAL : newModel = normnorm; break;
		}
		session->setModel(newModel);
	}
//	return 0;
}


/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    runAnalysisN
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeNonHomog2_runAnalysisN
  (JNIEnv *, jclass, jint sessionID, jint samples)
{

#ifdef _DEBUG
		cout << "DLL running analysis (session  " << sessionID << endl;
#endif

	NHSession * session = getNHSession(sessionID);

	if (session != 0L) {

		try {
			session->setSampleCount(samples);
#ifdef _DEBUG
			cout << "DLL starting analysis. Samples: " << samples << endl;
#endif
			int result = session->runAnalysis();
#ifdef _DEBUG
			cout << "DLL analysis completed: " << result << endl;
#endif
			return result;
		} catch (exception e) {
			return STATUS_ERROR;
		}
	} else {
#ifdef _DEBUG
		cout << "DLL: session not found"<< endl;
#endif
	}
	return -1;
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    getSampleCountN
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeNonHomog2_getSampleCountN
  (JNIEnv *, jclass, jint sessionID)
{
	NHSession * session = getNHSession(sessionID);

	if (session != 0L) {
		return session->getSampleCount();
	}
	return 0;
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    getAlphaN
 * Signature: (II)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeNonHomog2_getAlphaN
  (JNIEnv *, jclass, jint sessionID, jint index)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		return session->getSampleAlpha(index);
	}
	return 0;
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    getBetaN
 * Signature: (II)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeNonHomog2_getBetaN
  (JNIEnv *, jclass, jint sessionID, jint index)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		return session->getSampleBeta(index);
	}
	return 0;
}  

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    getProbabilityN
 * Signature: (II)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeNonHomog2_getProbabilityN
  (JNIEnv *, jclass, jint sessionID, jint index)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		return session->getSampleProbability(index);
	}
	return 0;
}  


/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    constructBinsN
 * Signature: (IIZ)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeNonHomog2_constructBinsN
  (JNIEnv *, jclass, jint sessionID, jint size, jboolean logScale)
{
#ifdef _DEBUG
	cout << "Constructing bins" << endl;
#endif
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		session->constructBinnedSpace(size,logScale);
#ifdef _DEBUG
		cout << "Constructing bins done" << endl;
#endif
	}
	return 0;
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    constructBinsExN
 * Signature: (IIZDDDD)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeNonHomog2_constructBinsExN
	(JNIEnv *, jclass, jint sessionID, jint size, jboolean logScale, jdouble, jdouble, jdouble, jdouble) 
{
#ifdef _DEBUG
	cout << "Constructing bins" << endl;
#endif
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		session->constructBinnedSpace(size,logScale);
#ifdef _DEBUG
		cout << "Constructing bins done" << endl;
#endif
	}
	return 0;
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    getBinSizeN
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeNonHomog2_getBinSizeN
  (JNIEnv *, jclass, jint)

{
	return 0;
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    getBinnedAlphaN
 * Signature: (II)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeNonHomog2_getBinnedAlphaN
  (JNIEnv *, jclass, jint sessionID, jint bin)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		return session->getBinnedAlpha(bin);
	}
	return 0;
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    getBinnedBetaN
 * Signature: (II)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeNonHomog2_getBinnedBetaN
  (JNIEnv *, jclass, jint sessionID, jint bin)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		return session->getBinnedBeta(bin);
	}
	return 0;
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    getBinnedDensityN
 * Signature: (III)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeNonHomog2_getBinnedDensityN
   (JNIEnv *, jclass, jint sessionID, jint alpha, jint beta)
{
	NHSession * session = getNHSession(sessionID);

	if (session != 0L) {
		return session->getBinnedDensity(alpha,beta);
	}
	return 0;
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    constructMarginalN
 * Signature: (IIII)V
 */
JNIEXPORT void JNICALL Java_redcas_numerical_NativeNonHomog2_constructMarginalN
	(JNIEnv *, jclass, jint sessionID, jint marginal, jint size, jint mode)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		bool logScale = (mode == redcas_numerical_NativeNonHomog2_DPDMODE_LOGARITHMIC_INTERVAL);
		session->constructMarginal(marginal,size,logScale);
	}
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    getMarginalSizeN
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeNonHomog2_getMarginalSizeN
	(JNIEnv *, jclass, jint sessionID)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		return session->getMarginalSize();
	}
	return 0;
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    getMarginalProbabilityN
 * Signature: (II)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeNonHomog2_getMarginalProbabilityN
  (JNIEnv *, jclass, jint sessionID, jint index) 
{
	NHSession * session = getNHSession(sessionID);

	if (session != 0L) {
		double prob = 0;
		double value = 0;
		session->getMarginalPoint(index,value,prob);	
		return prob;
	}
	return 0;
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    getMarginalValueN
 * Signature: (II)D
 */
JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeNonHomog2_getMarginalValueN
  (JNIEnv *, jclass, jint sessionID, jint index)
{
	NHSession * session = getNHSession(sessionID);

	if (session != 0L) {
		double prob = 0;
		double value = 0;
		session->getMarginalPoint(index,value,prob);	
		return value;
	}
	return 0;
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    getStatusN
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_redcas_numerical_NativeNonHomog2_getStatusN
  (JNIEnv * env, jclass, jint sessionID)
{
	jstring result;
	
	NHSession * session = getNHSession(sessionID);

	if (session != 0L) {
		char buffer[128];
		session->getStatus(buffer,127);
		result = env->NewStringUTF(buffer);
	} else {
		result = env->NewStringUTF("");
	}

	return result;
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    getProgressN
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeNonHomog2_getProgressN
  (JNIEnv *, jclass, jint sessionID)
{
	NHSession * session = getNHSession(sessionID);

	if (session != 0L) {
		return session->getProgress();
	}
	return 0;
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    getStatusCodeN
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_redcas_numerical_NativeNonHomog2_getStatusCodeN
	(JNIEnv *, jclass, jint sessionID)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		return session->getStatusCode();
	}
	return STATUS_ERROR;
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    requestCancelN
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_redcas_numerical_NativeNonHomog2_requestCancelN
  (JNIEnv *, jclass, jint sessionID)
{
	NHSession * session = getNHSession(sessionID);

	if (session != 0L) {
		session->requestCancel();
	}
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    isCancelRequestedN
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_redcas_numerical_NativeNonHomog2_isCancelRequestedN
  (JNIEnv *, jclass, jint)
{
	return false;
}

/*
 * Class:     redcas_numerical_NativeNonHomog2
 * Method:    clearStatusN
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_redcas_numerical_NativeNonHomog2_clearStatusN
  (JNIEnv *, jclass, jint sessionID)
{
	NHSession * session = getNHSession(sessionID);

	if (session != 0L) {
		session->clearStatus();
	}
}

JNIEXPORT void JNICALL Java_redcas_numerical_NativeNonHomog2_setLogNormalPriorN
  (JNIEnv *, jclass, jint sessionID, jint mode, jdouble nu1, jdouble tau1, jdouble nu2, jdouble tau2)
{
	NHSession * session = getNHSession(sessionID);

	if (session != 0L) {
		session->setPrior(mode,nu1,tau1,nu2,tau2);
	}
}


JNIEXPORT jdouble JNICALL Java_redcas_numerical_NativeNonHomog2_getLnLikelihoodN
  (JNIEnv *, jclass, jint sessionID, jdouble p1, jdouble p2)
{
	NHSession * session = getNHSession(sessionID);

	if (session != 0L) {
		return session->computeLnLikelihood(p1,p2);
	}

	return 0;
}

#ifdef __cplusplus
}
#endif

void testJNIInterface() {
#ifdef _DEBUG
	

#endif
}


