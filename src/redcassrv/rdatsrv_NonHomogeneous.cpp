/**
 * This class is used to define the non-homogeneous analysis interface used in 
 * RDAT service.
 */

#include "stdafx.h"
#include "rdatsrv_NonHomogeneous.h"
#include "rdatsrv_Distribution.h"
#include "rdatdefs.h"
#include "NHSession.h"
#include "SessionManager.h"
#include "DistributionManager.h"
#include "DPD.h"
#include "math.h"

CRedcassrvNonHomo CRedcassrvNonHomo::instance = CRedcassrvNonHomo();

NHSession * getNHSession(int index) 
{
	return static_cast<NHSession *>
		(SessionManager::getInstance()->retrieveSession(index));
}

CRedcassrvNonHomo::CRedcassrvNonHomo()
{ 
}

int CRedcassrvNonHomo::createSession() 
{
	return SessionManager::getInstance()->createSession(NONHOMOG_SESSION);
}

void CRedcassrvNonHomo::destroySession(int iHandle)
{
	SessionManager::getInstance()->destroySession(iHandle);
}

int CRedcassrvNonHomo::runAnalysis(int iHandle, int iSamples)
{
	NHSession * session = getNHSession(iHandle);
	if (session != 0L) {
		session->setSampleCount(iSamples);
		return session->runAnalysis();
	}	

	return redcas_ErrorCode_InvalidHandle;
}

void CRedcassrvNonHomo::setModelN(int sessionID, int model, double mean1, double ef1, double mean2, double ef2) 
{
	double nu1, nu2, tau1, tau2;

	// Convert input from mean and error factor mode to Nu/Tau Mode for lognormal distribution
	CRedcassrvDist::getInstance()->getdefaultInputModeValue(REDCASSRV_LOGNORMAL_DIST, 
		REDCASSRV_MEDIANEF_MODE, 
		mean1, ef1, nu1, tau1);
	CRedcassrvDist::getInstance()->getdefaultInputModeValue(REDCASSRV_LOGNORMAL_DIST, 
		REDCASSRV_MEDIANEF_MODE, 
		mean2, ef2, nu2, tau2);
	
	NHSession * session = getNHSession(sessionID);

	if (session != 0L) {
		// Set model type first
		NHModels newModel;
		switch (model) {
			case redcas_numerical_NativeNonHomo_BETA_BINOMIAL : 
				newModel = betbin; 
				session->setModel(newModel);
				session->setPrior(redcas_PRIOR_MEAN_STD_ERR, nu1,tau1,nu2,tau2);
				break;

			case redcas_numerical_NativeNonHomo_GAMMA_POISSON : 
				newModel = gampois; 
				session->setModel(newModel);
				session->setPrior(redcas_PRIOR_MEAN_STD_ERR, nu1,tau1,nu2,tau2);
				break;

			case redcas_numerical_NativeNonHomo_LOGNORM_POISSON : 
				newModel = logpois; 
				session->setModel(newModel);
				session->setPrior(redcas_PRIOR_MEDIAN_EF, nu1,tau1,nu2,tau2);
				break;

			case redcas_numerical_NativeNonHomo_LOGNORM_BINOMIAL : 
				newModel = logbin; 
				session->setModel(newModel);
				session->setPrior(redcas_PRIOR_MEDIAN_EF, nu1,tau1,nu2,tau2);
				break;
		}
	}
}

void CRedcassrvNonHomo::addDataPoint(int sessionID, double x_star, double tau, double events,
			double exposure)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		session->addDataPoint(x_star, tau, events, exposure);
	}
}

int		CRedcassrvNonHomo::getProgress(int sessionID)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		return session->getProgress();
	}

	return redcas_ErrorCode_InvalidHandle;
}

int		CRedcassrvNonHomo::getStatusCode(int sessionID)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		return session->getStatusCode();
	}

	return redcas_ErrorCode_InvalidHandle;
}

void	CRedcassrvNonHomo::requestCancel(int sessionID)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		session->requestCancel();
	}
}

double	CRedcassrvNonHomo::getPopVarMeanMean(int sessionID)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {

		NHParameters parameter = dflt;

		return session->getMeasureMean(parameter,0,mean);
	}

	return redcas_ErrorCode_InvalidHandle;
}

double	CRedcassrvNonHomo::getPopVarVarianceMean(int sessionID)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {

		NHParameters parameter = dflt;

		return session->getMeasureMean(parameter,0,variance);
	}

	return redcas_ErrorCode_InvalidHandle;
}

double	CRedcassrvNonHomo::getPopVarMeanPercentile(int sessionID, double p)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		return session->findMeanPercentile(p);
	}

	return redcas_ErrorCode_InvalidHandle;
}

double	CRedcassrvNonHomo::getPopVarPercentilePercentile(int sessionID, double z, double p)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {

		NHParameters parameter = dflt;

		return session->getMeasure(parameter,z,quantile,p);
	}

	return redcas_ErrorCode_InvalidHandle;
}

double	CRedcassrvNonHomo::getPopVarCDF(int sessionID, double x, double p)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {

		NHParameters parameter = dflt;

		return session->getMeasure(parameter,x,cdf,p);
	}

	return redcas_ErrorCode_InvalidHandle;
}

double	CRedcassrvNonHomo::getPopVarPDF(int sessionID, double x, double p)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {

		NHParameters parameter = dflt;

		return session->getMeasure(parameter,x,pdf,p);
	}

	return redcas_ErrorCode_InvalidHandle;
}

double	CRedcassrvNonHomo::getPopVarCDFMean(int sessionID, double x)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {

		NHParameters parameter = dflt;

		return session->getMeasureMean(parameter,x,cdf);
	}

	return redcas_ErrorCode_InvalidHandle;
}

double	CRedcassrvNonHomo::getPopVarPDFMean(int sessionID, double x)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {

		NHParameters parameter = dflt;

		return session->getMeasureMean(parameter,x,pdf);
	}

	return redcas_ErrorCode_InvalidHandle;
}

int CRedcassrvNonHomo::getSampleCount(int sessionID)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		return session->getSampleCount();
	}

	return redcas_ErrorCode_InvalidHandle;
}

bool CRedcassrvNonHomo::constructBins(int sessionID, int size, bool logScale)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		session->constructBinnedSpace(size,logScale);
		return true;
	}

	return false;
}

/* returns the density of given cell in binned sample space */
double CRedcassrvNonHomo::getBinnedAlpha(int sessionID, int bin)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		return session->getBinnedAlpha(bin);
	}

	return redcas_ErrorCode_InvalidHandle;
}

double CRedcassrvNonHomo::getBinnedBeta(int sessionID, int bin)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		return session->getBinnedBeta(bin);
	}

	return redcas_ErrorCode_InvalidHandle;
}

double CRedcassrvNonHomo::getBinnedDensity(int sessionID, int alpha, int beta)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		return session->getBinnedDensity(alpha,beta);
	}

	return redcas_ErrorCode_InvalidHandle;
}

int CRedcassrvNonHomo::getMarginalSize(int iHandle)
{
	NHSession * session = getNHSession(iHandle);
	if (session != 0L) {
		int iSize = session->getMarginalSize();
		if (iSize > 0)
			return (iSize - 1);

		return 0;
	}

	return redcas_ErrorCode_InvalidHandle;
}

int CRedcassrvNonHomo::getMarginalAlpha(int sessionID, int size, bool bLogScale)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		session->constructMarginal(0,size,bLogScale);
		int iMargSize = session->getMarginalSize();
		
		DPD * dMargResult = new DPD(iMargSize);
		double value, prob; 
		for (int i=0; i<iMargSize; i++)
		{
			session->getMarginalPoint(i,value,prob);	
			dMargResult->setProbability(i, prob);
			dMargResult->setValue(i, value);
		}
		dMargResult->sort();
		dMargResult->normalize();

		int retval = DistributionManager::getInstance()->add(dMargResult);
		if (retval == -1) {
			dMargResult->releaseMemory();
			delete dMargResult;
			return -1;
		} else return retval + 1;
	}

	return redcas_ErrorCode_InvalidHandle;
}

int CRedcassrvNonHomo::getMarginalBeta(int sessionID, int size, bool bLogScale)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		session->constructMarginal(1,size,bLogScale);

		int iMargSize = session->getMarginalSize();
		
		DPD * dMargResult = new DPD(iMargSize);
		double value, prob; 
		for (int i=0; i<iMargSize; i++)
		{
			session->getMarginalPoint(i,value,prob);	
			dMargResult->setProbability(i, prob);
			dMargResult->setValue(i, value);
		}
		dMargResult->sort();
		dMargResult->normalize();

		int retval = DistributionManager::getInstance()->add(dMargResult);
		if (retval == -1) {
			dMargResult->releaseMemory();
			delete dMargResult;
			return -1;
		} else return retval + 1;
	}

	return redcas_ErrorCode_InvalidHandle;
}

double CRedcassrvNonHomo::getUpperBound(int sessionID)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		return session->getPopVarUpperBound();
	}

	return redcas_ErrorCode_InvalidHandle;
}

double CRedcassrvNonHomo::getLowerBound(int sessionID)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		return session->getPopVarLowerBound();
	}

	return redcas_ErrorCode_InvalidHandle;
}

int CRedcassrvNonHomo::clearData(int sessionID)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		session->clearData();
		return 0;
	}

	return redcas_ErrorCode_InvalidHandle;
}

int CRedcassrvNonHomo::clearSamplesN(int sessionID)
{
	NHSession * session = getNHSession(sessionID);
	if (session != 0L) {
		session->clearSamplesEx();
		return 0;
	}

	return redcas_ErrorCode_InvalidHandle;
}
