/**
 * This class is used to define the interface for all distribution supported
 * in RDAT service.
 */


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
#include "DistributionManager.h"
#include "ParametricDistribution.h"
#include "UniformBinomialDistribution.h"
#include "UniformPoissonDistribution.h"
#include "LogNormalBinomialDistribution.h"
#include "LogNormalPoissonDistribution.h"
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

CRedcassrvDist CRedcassrvDist::instance = CRedcassrvDist();

// This is the constructor of a class that has been exported.
// see redcassrv.h for the class definition
CRedcassrvDist::CRedcassrvDist()
{
	return;
}

/************************************************************************/
/* Create a new distribution and returns handle to it.                  */
/* Return -1 if creation failed. Use this distribution for non-tabular  */
/* distributions only.                                                  */
/************************************************************************/
int CRedcassrvDist::create(int type, int inputmode, double p1, double p2)
{
	double p1Default, p2Default;
	getdefaultInputModeValue(type, inputmode, p1, p2, p1Default, p2Default);

	return create(type, p1Default, p2Default);
}

int CRedcassrvDist::create(int type, double p1, double p2)
{
	GenericDistribution * dist = 0;

	switch (type) {
	case REDCASSRV_BETA_DIST:
		dist = new BetaDistribution(p1, p2);
		break;
	case REDCASSRV_BINOMIAL_DIST:
		break;
	case REDCASSRV_GAMMA_DIST:
		dist = new GammaDistribution(1 / p2, p1);
		break;
	case REDCASSRV_NORMAL_DIST:
		dist = new NormalDistribution(p1, p2);
		break;
	case REDCASSRV_LOGNORMAL_DIST:
		dist = new LogNormalDistribution(p1, p2);
		break;
	case REDCASSRV_TLOGNORMAL_DIST:
		dist = new TrLogNormalDistribution(p1, p2);
		break;
	case REDCASSRV_UNIFORM_DIST:
		{
			double vals[2];
			vals[0] = p1;
			vals[1] = p2;
			double cdfs[2];
			cdfs[0] = 0;
			cdfs[1] = 1;
			dist = new TabularDistribution(vals, cdfs, 2, TABULAR_LIN, p1, p2);
		}
		break;
	case REDCASSRV_LOGUNIFORM_DIST:
		{
			double vals[2];
			vals[0] = p1;
			vals[1] = p2;
			double cdfs[2];
			cdfs[0] = 0;
			cdfs[1] = 1;
			dist = new TabularDistribution(vals, cdfs, 2, TABULAR_LOG, p1, p2);
		}
		break;
	case REDCASSRV_POSISSON_DIST:
		//		dist = new PosissonDistribution(log(p1*p2)/2,log(p2/p1)/3.289711);
		break;
	case REDCASSRV_CHISQUARE_DIST:
		break;
	case REDCASSRV_EXPONENTIAL_DIST:
		break;
	case REDCASSRV_INVERTEDGAMMA_DIST:
		break;
	case REDCASSRV_INVCHISQUARE_DIST:
		break;
	case REDCASSRV_STUDENTT_DIST:
		break;
	case REDCASSRV_WEIBULL_DIST:
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
	}
	else return retval + 1;
}

void CRedcassrvDist::destory(int iHandle)
{
	GenericDistribution * dist = DistributionManager::getInstance()->remove(iHandle - 1);
	dist->releaseMemory();
	delete dist;
}

// Returns x value at given index of distribution with given handle.
double CRedcassrvDist::getX(int iHandle, double x)
{
	double retval = 0;
	GenericDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getX(x);
	}
	return retval;
}

// Returns PDF at x of distribution with given handle.
double CRedcassrvDist::getPDF(int iHandle, double x)
{
	double retval = 0;
	GenericDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getPDF(x);
	}
	return retval;
}

// Returns CDF at x of distribution with given handle.
double CRedcassrvDist::getCDF(int iHandle, double x)
{
	double retval = 0;
	GenericDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getCDF(x);
	}
	return retval;
}

// Returns quantile of distribution with given handle.
double CRedcassrvDist::getQuantile(int iHandle, double z)
{
	double retval = 0;
	GenericDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getQuantile(z);
	}
	return retval;
}

// Returns mean value of distribution with given handle.
double CRedcassrvDist::getMean(int iHandle)
{
	double retval = 0;
	GenericDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getMean();
	}
	return retval;
}

// Returns median value of distribution with given handle.
double CRedcassrvDist::getMedian(int iHandle)
{
	double retval = 0;
	GenericDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getMedian();
	}
	return retval;
}

// Returns variance of distribution with given handle.
double CRedcassrvDist::getVariance(int iHandle)
{
	double retval = 0;
	GenericDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getVariance();
	}
	return retval;
}

void CRedcassrvDist::getdefaultInputModeValue(int distType, int inputMode, double p1, double p2, double &p1Default, double &p2Default)
{
	p1Default = p1;
	p2Default = p2;

	if (distType == REDCASSRV_LOGNORMAL_DIST)
	{
		// Default input mode is Median/Tau
		if (inputMode == REDCASSRV_MEDIANEF_MODE) {
			p1Default = log(p1);
			p2Default = log(p2) / 1.6448536269514722;
		}
		else if (inputMode == REDCASSRV_MEANEF_MODE) {
			p2Default = log(p2) / 1.6448536269514722;
			p1Default = log(p1) - p2Default*p2Default / 2;
		}
		else if (inputMode == REDCASSRV_5TH95TH_MODE) {
			p1Default = log(p1*p2) / 2;
			p2Default = log(p2 / p1) / 3.28971;
		}
	}
	else if (distType == REDCASSRV_BETA_DIST)	{
		// Default Input Mode is Alpha/Beta
		if (inputMode == REDCASSRV_MEANSD_MODE) {
			p1Default = -p1 * (-p1 + p1 * p1 + p2*p2) / (p2*p2);
			p2Default = (p1 - 2 * p1*p1 + p1*p1*p1 - p2*p2 + p1*p2*p2) / (p2*p2);
		}
		else if (inputMode == REDCASSRV_5TH95TH_MODE) {
			Beta0595InputMode mode(p1, p2);
			double native_arr[2];
			mode.convert(p1, p2, native_arr[0], native_arr[1]);
			p1Default = native_arr[0];
			p2Default = native_arr[1];
		}
	}
	else if (distType == REDCASSRV_GAMMA_DIST) {
		// Default Input Mode is Alpha/Theta
		if (inputMode == REDCASSRV_MEANSD_MODE) {
			p1Default = p1*p1 / (p2*p2);
			p2Default = p1 / (p2*p2);
		}
		else if (inputMode == REDCASSRV_5TH95TH_MODE) {
			Gamma0595InputMode mode(p1, p2);
			double native_arr[2];
			mode.convert(p1, p2, native_arr[0], native_arr[1]);
			p1Default = native_arr[0];
			p2Default = native_arr[1];
		}
		else if (inputMode == REDCASSRV_ALPHABETA_MODE) {
			p1Default = p1;
			p2Default = 1 / p2;
		}
	}
	else if (distType == REDCASSRV_TLOGNORMAL_DIST) {
		// Default Input Mode is Median/Tau
		if (inputMode == REDCASSRV_MEDIANEF_MODE) {
			p1Default = log(p1);
			p2Default = log(p2) / 1.6448536269514722;
		}
		else if (inputMode == REDCASSRV_MEDIANTAU_MODE) {
			p1Default = log(p1);
			p2Default = p2;
		}
	}
	else if (distType == REDCASSRV_NORMAL_DIST) {
		// Default Input Mode is Mean/SD
		if (inputMode == REDCASSRV_5TH95TH_MODE) {
			p1Default = (p1 + p2) / 2;
			p2Default = (p2 - p1) / 3.28971;
		}
	}
}

REDCASSRV_API int	REDCASSRV_DIST_create(int distType, int inputMode, double p1, double p2)
{
	return CRedcassrvDist::getInstance()->create(distType, inputMode, p1, p2);
}

REDCASSRV_API void	REDCASSRV_DIST_destory(int iHandle)
{
	return CRedcassrvDist::getInstance()->destory(iHandle);
}

REDCASSRV_API int	REDCASSRV_DIST_validateInput(int distType, int inputMode, double parameter1, double parameter2)
{
	return 1;
}

REDCASSRV_API double REDCASSRV_DIST_getDefaultParameter(int distType, int inputMode,
	double parameter1, double parameter2, int index)
{
	return 1;
}

REDCASSRV_API double REDCASSRV_DIST_getPDF(int iHandle, double x)
{
	return CRedcassrvDist::getInstance()->getPDF(iHandle, x);
}

REDCASSRV_API double REDCASSRV_DIST_getCDF(int iHandle, double x)
{
	return CRedcassrvDist::getInstance()->getCDF(iHandle, x);
}

REDCASSRV_API double REDCASSRV_DIST_getQuantile(int iHandle, double z)
{
	return CRedcassrvDist::getInstance()->getQuantile(iHandle, z);
}

REDCASSRV_API double REDCASSRV_DIST_getMean(int iHandle)
{
	return CRedcassrvDist::getInstance()->getMean(iHandle);
}

REDCASSRV_API double REDCASSRV_DIST_getMedian(int iHandle)
{
	return CRedcassrvDist::getInstance()->getMedian(iHandle);
}

REDCASSRV_API double REDCASSRV_DIST_getVariance(int iHandle)
{
	return CRedcassrvDist::getInstance()->getVariance(iHandle);
}