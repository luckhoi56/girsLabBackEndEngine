/**
 * This class defines the interface used for weibull analysis within RDAT Plus
 * service.
 */

#include "stdafx.h"
#include "rdatsrv_Weibull.h"
#include "WeibullSessionManager.h"
#include "BRASSEngine.h"

CRedcassrvWeibull CRedcassrvWeibull::instance = CRedcassrvWeibull();

BRASSEngine * CRedcassrvWeibull::getWeibullSession(int index)
{
	return static_cast<BRASSEngine *>
		(WeibullSessionManager::getInstance()->retrieveSession(index));
}

CRedcassrvWeibull::CRedcassrvWeibull()
{
}

int CRedcassrvWeibull::createWeibullSession()
{
	return WeibullSessionManager::getInstance()->createSession();
}

void CRedcassrvWeibull::destroyWeibullSession(int iHandle)
{
	WeibullSessionManager::getInstance()->destroySession(iHandle);
}

int CRedcassrvWeibull::initialize(int iHandle)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->setup(1, 1, 1);
	}

	return redcas_ErrorCode_InvalidHandle;
}

int CRedcassrvWeibull::createTestDataSet(int iHandle, int record_cnt)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->createTestDataSet(record_cnt);
	}

	return redcas_ErrorCode_InvalidHandle;
}

int CRedcassrvWeibull::setTestDataRecord(int iHandle, int index, double start,
	double end, bool bSuccess)
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

int CRedcassrvWeibull::attachTestDataSet(int iHandle)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->attachTestDataSet(0, 1);
	}

	return redcas_ErrorCode_InvalidHandle;
}

int CRedcassrvWeibull::createGroupedDataSet(int iHandle, int record_cnt)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->createGroupedDataSet(record_cnt);
	}

	return redcas_ErrorCode_InvalidHandle;
}

int CRedcassrvWeibull::setGroupedDataRecord(int iHandle, int index,
	double start, double end, int population, int failures)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->setGroupedDataRecord(index, start, end, failures, population);
	}

	return redcas_ErrorCode_InvalidHandle;
}

int CRedcassrvWeibull::attachGroupedDataSet(int iHandle)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->attachGroupedDataSet(0, 1);
	}

	return redcas_ErrorCode_InvalidHandle;
}

int CRedcassrvWeibull::setShapePrior(int iHandle, double median, double errfac)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->addShapePrior(1, median, errfac);
	}

	return redcas_ErrorCode_InvalidHandle;
}

int CRedcassrvWeibull::setLogNormalScalePrior(int iHandle, double median,
	double errfac)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->addScalePrior(0, 1, median, errfac);
	}

	return redcas_ErrorCode_InvalidHandle;
}

int CRedcassrvWeibull::setRxCxScalePrior(int iHandle, double time,
	double reliability, double confidence)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		session->createCustomPrior(1, BRASSEngine::MEASURE_RELIABILITY);
		session->setCustomPriorRecord(0, time, reliability, confidence, 0.9);
		return session->attachCustomPrior(0, 1);
	}

	return redcas_ErrorCode_InvalidHandle;
}

int CRedcassrvWeibull::setI1000ScalePrior(int iHandle, double time,
	double lower, double upper)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		session->createCustomPrior(1, BRASSEngine::MEASURE_IPER1000);
		session->setCustomPriorRecord(0, time, lower, upper, 0.9);
		return session->attachCustomPrior(0, 1);
	}

	return redcas_ErrorCode_InvalidHandle;
}

int CRedcassrvWeibull::generateSamples(int iHandle, int samples, int chains,
	int interleave, int burnin, long seed, bool clear)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->generateSamples(samples, chains, interleave, burnin, seed, clear);
	}

	return redcas_ErrorCode_InvalidHandle;
}

int CRedcassrvWeibull::getStatus(int iHandle)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->getStatus();
	}

	return redcas_ErrorCode_InvalidHandle;
}

int CRedcassrvWeibull::getProgress(int iHandle)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->getProgress();
	}

	return redcas_ErrorCode_InvalidHandle;
}

int CRedcassrvWeibull::estimateJointParameterDensity(int iHandle,
	bool log_scale, int draw_points)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->estimateJointParameterDensity(0, 1, log_scale, draw_points);
	}

	return redcas_ErrorCode_InvalidHandle;
}

double CRedcassrvWeibull::computeWeibullMTTF(int iHandle, double dShape, double dScale)
{
	BRASSEngine * session = getWeibullSession(iHandle);

	if (session != 0L) {
		return session->computeWeibullMTTF(dShape, dScale);
	}

	return redcas_ErrorCode_InvalidHandle;
}
