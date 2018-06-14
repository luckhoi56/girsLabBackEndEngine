// redcassrv.cpp : Defines the entry point for the DLL application.
//


#include "math.h"
#include "redcassrv.h"
#include "DistributionManager.h"
#include "GenericDistribution.h"
#include "UniformBinomialDistribution.h"
#include "UniformPoissonDistribution.h"
#include "LogNormalBinomialDistribution.h"
#include "LogNormalPoissonDistribution.h"
#include "RdatDefs.h"
#include "rdatsrv_Homogeneous.h"


#undef REDCASSRV_UNIFORM_BINOMIAL
#define REDCASSRV_UNIFORM_BINOMIAL 200L
#undef REDCASSRV_UNIFORM_POISSON
#define REDCASSRV_UNIFORM_POISSON 201L
#undef REDCASSRV_LOGUNIFORM_BINOMIAL
#define REDCASSRV_LOGUNIFORM_BINOMIAL 202L
#undef REDCASSRV_LOGUNIFORM_POISSON
#define REDCASSRV_LOGUNIFORM_POISSON 203L
#undef REDCASSRV_LOGNORMAL_BINOMIAL
#define REDCASSRV_LOGNORMAL_BINOMIAL 204L
#undef REDCASSRV_LOGNORMAL_POISSON
#define REDCASSRV_LOGNORMAL_POISSON 205L
#undef REDCASSRV_LOGTAB
#define REDCASSRV_LOGTAB 206L
#undef REDCASSRV_LINTAB
#define REDCASSRV_LINTAB 207L
#undef REDCASSRV_LINTAB_BINOMIAL
#define REDCASSRV_LINTAB_BINOMIAL 208L
#undef REDCASSRV_LOGTAB_BINOMIAL
#define REDCASSRV_LOGTAB_BINOMIAL 209L
#undef REDCASSRV_LINTAB_POISSON
#define REDCASSRV_LINTAB_POISSON 210L
#undef REDCASSRV_LOGTAB_POISSON
#define REDCASSRV_LOGTAB_POISSON 211L
#undef REDCASSRV_LINTAB_LOGNORMAL
#define REDCASSRV_LINTAB_LOGNORMAL 212L
#undef REDCASSRV_LOGTAB_LOGNORMAL
#define REDCASSRV_LOGTAB_LOGNORMAL 213L
#undef REDCASSRV_LINTAB_NORMAL
#define REDCASSRV_LINTAB_NORMAL 214L
#undef REDCASSRV_LOGTAB_NORMAL
#define REDCASSRV_LOGTAB_NORMAL 215L
#undef REDCASSRV_LINTAB_LOGNORMAL_POISSON
#define REDCASSRV_LINTAB_LOGNORMAL_POISSON 216L
#undef REDCASSRV_LOGTAB_LOGNORMAL_POISSON
#define REDCASSRV_LOGTAB_LOGNORMAL_POISSON 217L
#undef REDCASSRV_LINTAB_LOGNORMAL_BINOMIAL
#define REDCASSRV_LINTAB_LOGNORMAL_BINOMIAL 218L
#undef REDCASSRV_LOGTAB_LOGNORMAL_BINOMIAL
#define REDCASSRV_LOGTAB_LOGNORMAL_BINOMIAL 219L

#undef REDCASSRV_LOGNORMAL_5_95_POISSON
#define	REDCASSRV_LOGNORMAL_5_95_POISSON	220L
#undef REDCASSRV_LOGNORMAL_5_95
#define	REDCASSRV_LOGNORMAL_5_95	221L
#undef REDCASSRV_LOGNORMAL
#define	REDCASSRV_LOGNORMAL	222L

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

// Constants for Prior-likelihood relationship
REDCASSRV_API int nRedcassrv_Uniform_Binomial_Type		=	REDCASSRV_UNIFORM_BINOMIAL;
REDCASSRV_API int nRedcassrv_Uniform_Poisson_Typee		=	REDCASSRV_UNIFORM_POISSON;
REDCASSRV_API int nRedcassrv_LogUniform_Binomial_Type	=	REDCASSRV_LOGUNIFORM_BINOMIAL;
REDCASSRV_API int nRedcassrv_LogUniform_Poisson_Type	=	REDCASSRV_LOGUNIFORM_POISSON;
REDCASSRV_API int nRedcassrv_LogNormal_Binomial_Type	=	REDCASSRV_LOGNORMAL_BINOMIAL;
REDCASSRV_API int nRedcassrv_LogNormal_Poisson_Type		=	REDCASSRV_LOGNORMAL_POISSON;
REDCASSRV_API int nRedcassrv_LogNormal_5_95_Poisson_Type=	REDCASSRV_LOGNORMAL_5_95_POISSON;
REDCASSRV_API int nRedcassrv_LogNormal_Type				=	REDCASSRV_LOGNORMAL;
REDCASSRV_API int nRedcassrv_LogNormal_5_95_Type		=	REDCASSRV_LOGNORMAL_5_95;

// Constants for input mode definition

CRedcassrv CRedcassrv::instance = CRedcassrv();

// This is the constructor of a class that has been exported.
// see redcassrv.h for the class definition
CRedcassrv::CRedcassrv()
{ 
	return; 
}

/************************************************************************/
/* Create a new distribution and returns handle to it.                  */
/* Return -1 if creation failed. Use this distribution for non-tabular  */
/* distributions only.                                                  */
/************************************************************************/
int CRedcassrv::create(int type, double p1, double p2, double p3, double p4)
{
	GenericDistribution * dist = 0;
	
	switch (type) {
		
	case REDCASSRV_UNIFORM_BINOMIAL:
		dist = new UniformBinomialDistribution(p1,p2,p3,p4,false);
		break;
	case REDCASSRV_UNIFORM_POISSON:
		dist = new UniformPoissonDistribution(p1,p2,p3,p4,false);
		break;
	case REDCASSRV_LOGUNIFORM_BINOMIAL:
		dist = new UniformBinomialDistribution(p1,p2,p3,p4,true);
		break;
	case REDCASSRV_LOGUNIFORM_POISSON:
		dist = new UniformPoissonDistribution(p1,p2,p3,p4,true);
		break;
	case REDCASSRV_LOGNORMAL_BINOMIAL:
		dist = new LogNormalBinomialDistribution(p1,p2,p3,p4);
		break;
	case REDCASSRV_LOGNORMAL_5_95_POISSON:
		dist = new LogNormalPoissonDistribution(log(p1*p2)/2,log(p2/p1)/3.28971,p3,p4);
		break;
	case REDCASSRV_LOGNORMAL_POISSON:
		dist = new LogNormalPoissonDistribution(log(p1),p2,p3,p4);
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

int CRedcassrv::create(int type, double p1, double p2)
{
	GenericDistribution * dist = 0;
	
	switch (type) {
		
	case REDCASSRV_LOGNORMAL:
		dist = new LogNormalDistribution(log(p1),p2);
		break;
	case REDCASSRV_LOGNORMAL_5_95:
		dist = new LogNormalDistribution(log(p1*p2)/2,log(p2/p1)/3.289711);
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

void CRedcassrv::destory(int iHandle)
{
	GenericDistribution * dist = DistributionManager::getInstance()->remove(iHandle - 1);
	delete dist;
}

// Returns PDF at x of distribution with given handle.
double CRedcassrv::getPDF(int iHandle, double x)
{
	double retval = 0;
	GenericDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getPDF(x);
	}
	return retval;
}

// Returns CDF at x of distribution with given handle.
double CRedcassrv::getCDF(int iHandle, double x)
{
	double retval = 0;
	GenericDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getCDF(x);
	}
	return retval;
}

// Returns quantile of distribution with given handle.
double CRedcassrv::getQuantile(int iHandle, double z)
{
	double retval = 0;
	GenericDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getQuantile(z);
	}
	return retval;
}

// Returns mean value of distribution with given handle.
double CRedcassrv::getMean(int iHandle)
{
	double retval = 0;
	GenericDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getMean();
	}
	return retval;
}

// Returns median value of distribution with given handle.
double CRedcassrv::getMedian(int iHandle)
{
	double retval = 0;
	GenericDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getMedian();
	}
	return retval;
}

// Returns variance of distribution with given handle.
double CRedcassrv::getVariance(int iHandle)
{
	double retval = 0;
	GenericDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getVariance();
	}
	return retval;
}
