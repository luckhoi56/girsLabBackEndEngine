/**
 * This class is used to define the interface for homogeneous analysis within
 * RDAT service.
 */

#include "rdatsrv_Homogeneous.h"
#include "rdatsrv_Distribution.h"
#include "DistributionManager.h"
#include "GenericDistribution.h"
#include "UniformBinomialDistribution.h"
#include "UniformPoissonDistribution.h"
#include "LogNormalBinomialDistribution.h"
#include "LogNormalPoissonDistribution.h"
#include "GammaDistribution.h"
#include "NormalDistribution.h"
#include "TrLogNormalDistribution.h"
#include "Beta0595InputMode.h"
#include "Gamma0595InputMode.h"
#include "SimpleHomogeneous.h"
#include "TabularDistribution.h"
#include "RdatDefs.h"
#include "math.h"

extern int nRedcassrv_AlphaBeta_Mode;
extern int nRedcassrv_MeanSD_Mode;
extern int nRedcassrv_5th95th_Mode;
extern int nRedcassrv_AlphaTheta_Mode;
extern int nRedcassrv_MedianTau_Mode;
extern int nRedcassrv_MedianEF_Mode;
extern int nRedcassrv_MeanEF_Mode;
extern int nRedcassrv_NuTau_Mode;
extern int nRedcassrv_MinMax_Mode;
extern int nRedcassrv_EventTime_Mode;
extern int nRedcassrv_EventDemands_Mode;

extern REDCASSRV_API int nRedcassrv_Beta_Distribution;
extern REDCASSRV_API int nRedcassrv_Binomial_Distribution;
extern REDCASSRV_API int nRedcassrv_Gamma_Distribution;
extern REDCASSRV_API int nRedcassrv_Normal_Distribution;
extern REDCASSRV_API int nRedcassrv_LogNormal_Distribution;
extern REDCASSRV_API int nRedcassrv_TLogNormal_Distribution;
extern REDCASSRV_API int nRedcassrv_Uniform_Distribution;
extern REDCASSRV_API int nRedcassrv_LogUniform_Distribution;
extern REDCASSRV_API int nRedcassrv_Posisson_Distribution;
extern REDCASSRV_API int nRedcassrv_Table_Distribution;

CRedcassrvHomo CRedcassrvHomo::instance = CRedcassrvHomo();

// This is the constructor of a class that has been exported.
// see redcassrv.h for the class definition
CRedcassrvHomo::CRedcassrvHomo()
{ 
	return; 
}

/************************************************************************/
/* Create a new distribution and returns handle to it.                  */
/* Return -1 if creation failed. Use this distribution for non-tabular  */
/* distributions only.                                                  */
/************************************************************************/
int CRedcassrvHomo::create(int priorType, double p1, double p2, int likelihoodType, double p3, double p4)
{
	GenericDistribution * dist = 0;
	double pos[2];
	

	if (priorType == REDCASSRV_LOGNORMAL_DIST) 
	{
		if (likelihoodType == REDCASSRV_POSISSON_DIST)
		{
			dist = new LogNormalPoissonDistribution(p1,p2,p3,p4);
		} 
		else if (likelihoodType == REDCASSRV_LOGNORMAL_DIST)
		{
			SimpleHomogeneous::updateLogNormalLogNormal(exp(p1),p2,exp(p3),p4,pos[0],pos[1]);
			dist = new LogNormalDistribution(log(pos[0]), pos[1]);
		} 
		else if (likelihoodType == REDCASSRV_BINOMIAL_DIST) 
		{
			dist = new LogNormalBinomialDistribution(p1, p2, p3,p4);
		}
	} 
	else if (priorType == REDCASSRV_BETA_DIST)
	{
		if (likelihoodType == REDCASSRV_LOGNORMAL_DIST)
		{
			// update lognormal distribution
			p2 = p1 + p2 -1;
			dist = new LogNormalBinomialDistribution(p3, p4, p1,p2);
		} 
		else if (likelihoodType == REDCASSRV_BINOMIAL_DIST)
		{
			SimpleHomogeneous::updateBetaBinomial(p1,p2,p3,p4,pos[0],pos[1]);
			dist = new BetaDistribution(pos[0], pos[1]);
		} 
	}
	else if (priorType == REDCASSRV_GAMMA_DIST)
	{
		if (likelihoodType == REDCASSRV_LOGNORMAL_DIST)
		{
			dist = new LogNormalPoissonDistribution(p3, p4, p1,	p2);
		} 
		else if (likelihoodType == REDCASSRV_POSISSON_DIST)
		{
			SimpleHomogeneous::updateGammaPoisson(p1,p2,p3,p4,pos[0],pos[1]);
			dist = new GammaDistribution(1/pos[1], pos[0]);
		} 
	}
	else if (priorType == REDCASSRV_NORMAL_DIST)
	{
		if (likelihoodType == REDCASSRV_NORMAL_DIST)
		{
			SimpleHomogeneous::updateNormalNormal(p1,p2,p3, p4,pos[0],pos[1]);
			dist = new NormalDistribution(pos[0], pos[1]);
		} 
	}
	else if (priorType == REDCASSRV_TLOGNORMAL_DIST) 
	{
		if (likelihoodType == REDCASSRV_LOGNORMAL_DIST)
		{
			int returnCode = SimpleHomogeneous::updateLogNormalLogNormal(exp(p1),p2,exp(p3),p4,pos[0],pos[1]);
			if (returnCode == 0) {
				dist = new TrLogNormalDistribution(log(pos[0]), pos[1]);
			}
		} 
		else if (likelihoodType == REDCASSRV_BINOMIAL_DIST) 
		{
			dist = new LogNormalBinomialDistribution(p1, p2, p3, p4);
		}
	}
	else if (priorType == REDCASSRV_UNIFORM_DIST) 
	{
		double vals[2];
		vals[0] = p1;
		vals[1] = p2;
		double cdfs[2];
		cdfs[0]= 0;
		cdfs[1]= 1;
		if (likelihoodType == REDCASSRV_NORMAL_DIST)
		{
			dist = new TabularDistribution(vals, cdfs, 2, TABULAR_LIN_NORMAL,p3,p4);
		} 
		else if (likelihoodType == REDCASSRV_LOGNORMAL_DIST)
		{
			dist = new TabularDistribution(vals, cdfs, 2, TABULAR_LIN_LOGNORMAL,p3,p4);
		} 
		else if (likelihoodType == REDCASSRV_BINOMIAL_DIST) 
		{
			dist = new TabularDistribution(vals, cdfs, 2, TABULAR_LIN_BINOMIAL,p3,p4);
		}
		else if (likelihoodType == REDCASSRV_POSISSON_DIST)
		{
			dist = new UniformPoissonDistribution(p1,p2,p3,p4, false);
		} 
	}
	else if (priorType == REDCASSRV_LOGUNIFORM_DIST) 
	{
		double vals[2];
		vals[0] = p1;
		vals[1] = p2;
		double cdfs[2];
		cdfs[0]= 0;
		cdfs[1]= 1;
		if (likelihoodType == REDCASSRV_LOGNORMAL_DIST)
		{
			dist = new TabularDistribution(vals, cdfs, 2, TABULAR_LOG_LOGNORMAL,p3,p4);
		} 
		else if (likelihoodType == REDCASSRV_BINOMIAL_DIST) 
		{
			dist = new TabularDistribution(vals, cdfs, 2, TABULAR_LOG_BINOMIAL,p3,p4);
		}
		else if (likelihoodType == REDCASSRV_POSISSON_DIST)
		{
			dist = new UniformPoissonDistribution(p1,p2,p3,p4, true);
		} 
	}

	if (dist == 0) return -1;

	int retval = DistributionManager::getInstance()->add(dist);
	if (retval == -1) {
		delete dist;
		return -1;
	} else return retval + 1;
}

int CRedcassrvHomo::create(int priorType, int priorInputMode, double p1, double p2, int likelihoodType, int likelihoodInputMode, double p3, double p4)
{
	double p1Default, p2Default, p3Default, p4Default;
	CRedcassrvDist::getInstance()->getdefaultInputModeValue(priorType, priorInputMode, p1, p2, p1Default, p2Default);
	CRedcassrvDist::getInstance()->getdefaultInputModeValue(likelihoodType, likelihoodInputMode, p3, p4, p3Default, p4Default);

	return create(priorType, p1Default, p2Default, likelihoodType, p3Default, p4Default);
}


void CRedcassrvHomo::destory(int iHandle)
{
	GenericDistribution * dist = DistributionManager::getInstance()->remove(iHandle - 1);
	delete dist;
}

// Returns PDF at x of distribution with given handle.
double CRedcassrvHomo::getPDF(int iHandle, double x)
{
	double retval = 0;
	GenericDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getPDF(x);
	}
	return retval;
}

// Returns PDF at x of distribution with given handle.
void CRedcassrvHomo::getPDF(int iHandle, double * x, double * y, int iCount)
{
	GenericDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		for (int i=0 ; i<iCount; i++)
			y[i] = dist->getPDF(x[i]);
	}
}


// Returns CDF at x of distribution with given handle.
void CRedcassrvHomo::getCDF(int iHandle, double * x, double * y, int iCount)
{
	GenericDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		for (int i=0 ; i<iCount; i++)
			y[i] = dist->getCDF(x[i]);
	}
}

// Returns CDF at x of distribution with given handle.
double CRedcassrvHomo::getCDF(int iHandle, double x)
{
	double retval = 0;
	GenericDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getCDF(x);
	}
	return retval;
}

// Returns quantile of distribution with given handle.
double CRedcassrvHomo::getQuantile(int iHandle, double z)
{
	double retval = 0;
	GenericDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getQuantile(z);
	}
	return retval;
}

// Returns mean value of distribution with given handle.
double CRedcassrvHomo::getMean(int iHandle)
{
	double retval = 0;
	GenericDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getMean();
	}
	return retval;
}

// Returns median value of distribution with given handle.
double CRedcassrvHomo::getMedian(int iHandle)
{
	double retval = 0;
	GenericDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getMedian();
	}
	return retval;
}

// Returns variance of distribution with given handle.
double CRedcassrvHomo::getVariance(int iHandle)
{
	double retval = 0;
	GenericDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getVariance();
	}
	return retval;
}

REDCASSRV_API int	REDCASSRV_HOMO_create(int priorType, double p1, double p2, 
										  int likelihoodType, double p3, double p4)
{
	return CRedcassrvHomo::getInstance()->create(priorType, p1, p2, likelihoodType, p3, p4);
}

REDCASSRV_API int	REDCASSRV_HOMO_createF(int priorType, int priorInputMode, double p1, double p2, 
										   int likelihoodType, int likelihoodInputMode, double p3, double p4)
{
	return CRedcassrvHomo::getInstance()->create(priorType, priorInputMode, p1, p2, likelihoodType, likelihoodInputMode, p3, p4);
}

REDCASSRV_API void	REDCASSRV_HOMO_destory(int iHandle)
{
	return CRedcassrvHomo::getInstance()->destory(iHandle);
}

REDCASSRV_API int	REDCASSRV_HOMO_validateInput(int distType, int inputMode, double parameter1, double parameter2)
{
	return 1;
}

REDCASSRV_API double REDCASSRV_HOMO_getDefaultParameter(int distType, int inputMode, 
														double parameter1, double parameter2 ,  int index)
{
	return 1;
}

REDCASSRV_API double REDCASSRV_HOMO_getPDF(int iHandle, double x)
{
	return CRedcassrvHomo::getInstance()->getPDF(iHandle, x);
}

REDCASSRV_API void REDCASSRV_HOMO_getPDFA(int iHandle, double * x, double * y, int iCount)
{
	CRedcassrvHomo::getInstance()->getPDF(iHandle, x, y, iCount);
}

REDCASSRV_API double REDCASSRV_HOMO_getCDF(int iHandle, double x)
{
	return CRedcassrvHomo::getInstance()->getCDF(iHandle, x);
}

REDCASSRV_API void REDCASSRV_HOMO_getCDFA(int iHandle, double * x, double * y, int iCount)
{
	CRedcassrvHomo::getInstance()->getCDF(iHandle, x, y, iCount);
}

REDCASSRV_API double REDCASSRV_HOMO_getQuantile(int iHandle, double z)
{
	return CRedcassrvHomo::getInstance()->getQuantile(iHandle, z);
}

REDCASSRV_API double REDCASSRV_HOMO_getMean(int iHandle)
{
	return CRedcassrvHomo::getInstance()->getMean(iHandle);
}

REDCASSRV_API double REDCASSRV_HOMO_getMedian(int iHandle)
{
	return CRedcassrvHomo::getInstance()->getMedian(iHandle);
}

REDCASSRV_API double REDCASSRV_HOMO_getVariance(int iHandle)
{
	return CRedcassrvHomo::getInstance()->getVariance(iHandle);
}