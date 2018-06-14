// $Header: C:/cvsrepo/ptdev/native/redcas/parametric/BetaDistribution.cpp,v 1.2 2001/05/24 13:57:26 fgroen Exp $

// BetaDistribution.cpp: implementation of the BetaDistribution class.
//
//////////////////////////////////////////////////////////////////////

#include "BetaDistribution.h"
#include "Error.h"
#include "DCDFLIBInterface.h"
#include "RANLIBInterface.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BetaDistribution::BetaDistribution(double alpha, double beta)
{
	if (checkParameters(alpha,beta)) {
		this->alpha = alpha;
		this->beta = beta;
		this->lnbetafunc = DCDFLIBInterface::computeLnBeta(alpha,beta);
	} else throw INVALID_ARGUMENT;
}

bool BetaDistribution::checkParameters(double alpha, double beta)
{
	return alpha > 0 && beta > 0;
}

double BetaDistribution::getSample() const
{
	return RANLIBInterface::generateBetaDeviate(alpha , beta);
}

double BetaDistribution::getMean() const
{
	return alpha / (alpha + beta);
}

double BetaDistribution::getQuantile(double percentage) const
{
	if( percentage < 0 || percentage > 1) throw INVALID_ARGUMENT;
	return DCDFLIBInterface::computeBetaQuantile(alpha,beta,percentage);
}

double BetaDistribution::getCDF(double x) const
{
	if (x < 0.0 || x > 1.0) throw INVALID_ARGUMENT;
	return DCDFLIBInterface::computeBetaCDF(alpha,beta,x);
}

double BetaDistribution::getLogPDF(double x) const
{
	if (x < 0.0|| x > 1.0) throw INVALID_ARGUMENT;

	if (x == 0 || x == 1) return -3000 - lnbetafunc;

	double lnx = log(x);
	double lnomx;

	if (lnx < -25)
		lnomx = -x;
	else
		lnomx = log(1 - x);

	return (alpha - 1) * lnx + (beta - 1) * lnomx - lnbetafunc;
}

double BetaDistribution::getVariance() const
{
	return (alpha * beta) / ((alpha + beta) * (alpha + beta) * (alpha + beta + 1));
}