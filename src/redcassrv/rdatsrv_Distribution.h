#ifndef RDATSRV_DISTRIBUTION_H__
#define RDATSRV_DISTRIBUTION_H__

#ifdef REDCASSRV_EXPORTS
#define REDCASSRV_API __declspec(dllexport)
#else
#define REDCASSRV_API __declspec(dllimport)
#endif

// The followings are the dll functions used for standard parametric distributions supported within 
// RDAT
extern "C" REDCASSRV_API int	REDCASSRV_DIST_create(int distType, int inputMode, double p1, double p2);
extern "C" REDCASSRV_API void	REDCASSRV_DIST_destory(int iHandle);
extern "C" REDCASSRV_API int	REDCASSRV_DIST_validateInput(int distType, int inputMode, double parameter1, double parameter2);
extern "C" REDCASSRV_API double REDCASSRV_DIST_getPDF(int iHandle, double x);
extern "C" REDCASSRV_API double REDCASSRV_DIST_getCDF(int iHandle, double x);
extern "C" REDCASSRV_API double REDCASSRV_DIST_getQuantile(int iHandle, double z);
extern "C" REDCASSRV_API double REDCASSRV_DIST_getMean(int iHandle);
extern "C" REDCASSRV_API double REDCASSRV_DIST_getMedian(int iHandle);
extern "C" REDCASSRV_API double REDCASSRV_DIST_getVariance(int iHandle);

/**
 * This class defines the interface functions used for distribution supported in 
 * RDAT Service DLL.
 */
class CRedcassrvDist {
public:
	CRedcassrvDist(void);
	static CRedcassrvDist * getInstance() {return &instance;}

	// Create a new distribution using parameters with input mode and returns handle to it.
	// This function should be used when the distribution contains only two parameters
	int create(int type, int inputmode, double p1, double p2);

	int create(int type, double p1, double p2);

	// Create a new distribution using default parameters and returns handle to it.
	// This function is the generic interface used all distributions with any number
	// of parameters. 
	// The parameters pointer defines the list of parameters 
	// ParameterCount defines the number of the parameters in the pointer.
	int create(int type, double * parameters, int parameterCount);

	// This parameter is another form to get the default parameter value based on the input 
	// parameter value in the input mode; the index defines the return parameter index
	double getDefaultParameter(int inputMode, int index, double parameter1, double parameter2);

	// Destroy distribution with given handle.
	void destory(int iHandle);

	// Return x value of the discrete distribution with given handle
	double getX(int iHandle, double x);

	// Returns PDF at x of distribution with given handle.
	double getPDF(int iHandle, double x);

	// Returns CDF at x of distribution with given handle.
	double getCDF(int iHandle, double x);

	// Returns quantile of distribution with given handle.
	double getQuantile(int iHandle, double z);

	// Returns mean value of distribution with given handle.
	double getMean(int iHandle);

	// Returns median value of distribution with given handle.
	double getMedian(int iHandle);

	// Returns variance of distribution with given handle.
	double getVariance(int iHandle);

	void getdefaultInputModeValue(int distType, int inputMode, double p1, double p2, double &p1Default, double &p2Default);

private:
	static CRedcassrvDist instance;
};

#endif