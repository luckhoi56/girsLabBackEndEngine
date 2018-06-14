// Redcassrv.cpp : Implementation of CRedcassrv
#include "stdafx.h"
#include "math.h"
#include "RDATSrvObj.h"
#include "Redcassrv.h"
#include "DistributionManager.h"
#include "ParametricDistribution.h"
#include "UniformBinomialDistribution.h"
#include "UniformPoissonDistribution.h"
#include "LogNormalBinomialDistribution.h"
#include "LogNormalPoissonDistribution.h"

/////////////////////////////////////////////////////////////////////////////
// CRedcassrv

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

STDMETHODIMP CRedcassrv::destroy(int iHandle)
{
	ParametricDistribution * dist = DistributionManager::getInstance()->remove(iHandle - 1);
	delete dist;

	return S_OK;
}

STDMETHODIMP CRedcassrv::create(int type, double p1, double p2, double p3, double p4, int *piHandle)
{
	ParametricDistribution * dist = 0;
	
	switch (type) {
		
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
		*piHandle = -1;
	} else {
		*piHandle = retval + 1;
	}

	return S_OK;
}

STDMETHODIMP CRedcassrv::getPDF(int iHandle, double x, double *pdPDF)
{
	double retval = 0;
	ParametricDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getPDF(x);
	}

	*pdPDF = retval;

	return S_OK;
}

STDMETHODIMP CRedcassrv::getCDF(int iHandle, double x, double *pdCDF)
{
	double retval = 0;
	ParametricDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getCDF(x);
	}

	*pdCDF = retval;

	return S_OK;
}

STDMETHODIMP CRedcassrv::getQuantile(int iHandle, double z, double *pdQuantile)
{
	double retval = 0;
	ParametricDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getQuantile(z);
	}

	*pdQuantile = retval;

	return S_OK;
}

STDMETHODIMP CRedcassrv::getMean(int iHandle, double *pdMean)
{
	double retval = 0;
	ParametricDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getMean();
	}

	*pdMean = retval;

	return S_OK;
}

STDMETHODIMP CRedcassrv::getMedian(int iHandle, double *pdMedian)
{
	double retval = 0;
	ParametricDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getMedian();
	}

	*pdMedian = retval;

	return S_OK;
}

STDMETHODIMP CRedcassrv::getVariance(int iHandle, double *pdVariance)
{
	double retval = 0;
	ParametricDistribution * dist = DistributionManager::getInstance()->retrieve(iHandle - 1);
	if (dist != 0) {
		retval = dist->getVariance();
	}

	*pdVariance = retval;

	return S_OK;
}

STDMETHODIMP CRedcassrv::creates(int type, double p1, double p2, int *piHandle)
{
	ParametricDistribution * dist = 0;
	
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
		*piHandle = -1;
	} else {
		*piHandle = retval + 1;
	}

	return S_OK;
}
