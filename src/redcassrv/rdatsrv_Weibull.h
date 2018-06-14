#ifndef RDATSRV_WEIBULL_H__
#define RDATSRV_WEIBULL_H__

#ifdef REDCASSRV_EXPORTS
#define REDCASSRV_API __declspec(dllexport)
#else
#define REDCASSRV_API __declspec(dllimport)
#endif

#undef redcas_ErrorCode_InvalidHandle 
#define redcas_ErrorCode_InvalidHandle -1

#include "brassengine.h"

class CRedcassrvWeibull {
public:
	CRedcassrvWeibull(void);
	static CRedcassrvWeibull * getInstance() {return &instance;}
	BRASSEngine * getWeibullSession(int index);

	int createWeibullSession();
	void destroyWeibullSession(int iHandle);
	int initialize(int iHandle);
	int createTestDataSet(int iHandle, int record_cnt);
	int setTestDataRecord(int iHandle, int index, double start,
			double end, bool bSuccess);
	int attachTestDataSet(int iHandle);
	int createGroupedDataSet(int iHandle, int record_cnt);
	int setGroupedDataRecord(int iHandle, int index,
			double start, double end, int population, int failures);
	int attachGroupedDataSet(int iHandle);
	int setShapePrior(int iHandle, double median, double errfac);
	int setLogNormalScalePrior(int iHandle, double median,
			double errfac);
	int setRxCxScalePrior(int iHandle, double time,
			double reliability, double confidence);
	int setI1000ScalePrior(int iHandle, double time,
			double lower, double upper);
	int generateSamples(int iHandle, int samples, int chains,
			int interleave, int burnin, long seed, bool clear);
	int getStatus(int iHandle);
	int getProgress(int iHandle);
	int estimateJointParameterDensity(int iHandle,
			bool log_scale, int draw_points);
	double computeWeibullMTTF(int iHandle, double dShape, double dScale);

private:
	static CRedcassrvWeibull instance;
};

#endif