#ifndef RDATSRV_HOMOGENEOUS_H__
#define RDATSRV_HOMOGENEOUS_H__

#ifdef REDCASSRV_EXPORTS
#define REDCASSRV_API __declspec(dllexport)
#else
#define REDCASSRV_API __declspec(dllimport)
#endif

// The followings are the dll functions used for homogeneous analysis module
extern "C" REDCASSRV_API int	REDCASSRV_HOMO_create(int priorType, double p1, double p2, 
												   int likelihoodType, double p3, double p4);
extern "C" REDCASSRV_API int	REDCASSRV_HOMO_createF(int priorType, int priorInputMode, double p1, double p2, 
													  int likelihoodType, int likelihoodInputMode, double p3, double p4);
extern "C" REDCASSRV_API void	REDCASSRV_HOMO_destory(int iHandle);
extern "C" REDCASSRV_API int	REDCASSRV_HOMO_getPosteriorType(int iHandle);
extern "C" REDCASSRV_API double REDCASSRV_HOMO_getPosteriorParameter(int iHandle, int index);
extern "C" REDCASSRV_API double REDCASSRV_HOMO_getPDF(int iHandle, double x);
extern "C" REDCASSRV_API void REDCASSRV_HOMO_getPDFA(int iHandle, double * x, double * y, int iCount);
extern "C" REDCASSRV_API double REDCASSRV_HOMO_getCDF(int iHandle, double x);
extern "C" REDCASSRV_API void REDCASSRV_HOMO_getCDFA(int iHandle, double * x, double * y, int iCount);
extern "C" REDCASSRV_API double REDCASSRV_HOMO_getQuantile(int iHandle, double z);
extern "C" REDCASSRV_API double REDCASSRV_HOMO_getMean(int iHandle);
extern "C" REDCASSRV_API double REDCASSRV_HOMO_getMedian(int iHandle);
extern "C" REDCASSRV_API double REDCASSRV_HOMO_getVariance(int iHandle);

/**
 * This class defines the interface functions used for homogeneous analysis 
 * module.
 */
class CRedcassrvHomo {
public:
	CRedcassrvHomo(void);

	// Get the static instance of the homogeneous analysis service.
	static CRedcassrvHomo * getInstance() {return &instance;}

	// Create a new homogeneous analysis and returns handle to it.
	// The input parameter value must be the value of default parameter type for the distribution
	int create(int priorType, double p1, double p2, int likelihoodType, double p3, double p4);

	// Create a new homogeneous analysis and returns handle to it.
	// The input parameter value must be the value of default parameter type for the distribution
	int create(int priorType, int priorInputMode, double p1, double p2, int likelihoodType, int likelihoodInputMode, double p3, double p4);

	// Destroy homogeneous analysis with given handle.
	void destory(int iHandle);

	// Returns PDF at x of posterior distribution of the homogeneous analysis with given handle.
	double getPDF(int iHandle, double x);

	void getPDF(int iHandle, double * x, double * y, int iCount);

	// Returns CDF at x of posterior distribution of the homogeneous analysis with given handle.
	double getCDF(int iHandle, double x);

	void getCDF(int iHandle, double * x, double * y, int iCount);

	// Returns quantile of posterior distribution of the homogeneous analysis with given handle.
	double getQuantile(int iHandle, double z);

	// Returns mean value of posterior distribution of the homogeneous analysis with given handle.
	double getMean(int iHandle);

	// Returns median value of posterior distribution of the homogeneous analysis with given handle.
	double getMedian(int iHandle);

	// Returns variance of posterior distribution of the homogeneous analysis with given handle.
	double getVariance(int iHandle);

	// Get posterial type; the output type must be one of the distribution
	// constants defined
	int getPosteriorType(int iHandle);

	// Get the default parameter for posterior distribution; the posterior
	// distribution must be a known distribution, not tabular distribution
	double getPosteriorParameter(int iHandle, int index);

private:
	static CRedcassrvHomo instance;

	// This function is used to allow user get parameter value for default input mode for all distributions
	void getdefaultInputModeValue(int distType, int inputMode, double p1, double p2, double &p1Default, double &p2Default);
};


#endif