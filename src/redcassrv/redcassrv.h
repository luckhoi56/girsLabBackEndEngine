
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the REDCASSRV_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// REDCASSRV_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef REDCASSRV_EXPORTS
#define REDCASSRV_API __declspec(dllexport)
#else
#define REDCASSRV_API __declspec(dllimport)
#endif

#include "RdatConstants.h"

// This class is exported from the redcassrv.dll
class CRedcassrv {
public:
	CRedcassrv(void);
	static CRedcassrv * getInstance() {return &instance;}

	// Create a new homogeneous analysis and returns handle to it.
	int create(int type, double p1, double p2, double p3, double p4);

	// Create a new distribution and returns handle to it.
	int create(int type, double p1, double p2);

	// Destroy distribution with given handle.
	void destory(int iHandle);

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

private:
	static CRedcassrv instance;
};

