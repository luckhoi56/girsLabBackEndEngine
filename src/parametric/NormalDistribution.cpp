// $Header: C:/cvsrepo/ptdev/native/redcas/parametric/NormalDistribution.cpp,v 1.2 2001/07/24 21:31:42 fgroen Exp $

// NormalDistribution.cpp: implementation of the NormalDistribution class.
//
//////////////////////////////////////////////////////////////////////

#include "NormalDistribution.h"
#include "../dcdf/DCDFLIBInterface.h"
#include "../ran/RANLIBInterface.h"
#include "../global/Error.h"
#include <math.h>

#define PI 3.14159

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

NormalDistribution::NormalDistribution(double mu, double sigma)
{
	if (checkParameters(mu,sigma)) {
		this->mu = mu;
		this->sigma = sigma;
	} else throw INVALID_ARGUMENT;
}

double NormalDistribution::getSample() const
{
	return RANLIBInterface::generateNormalDeviate(mu,sigma);
}

double NormalDistribution::getQuantile(double x) const
{
	if (x <= 0 || x >= 1) throw INVALID_ARGUMENT;
	return DCDFLIBInterface::computeNormalQuantile(mu,sigma,x);
}

double NormalDistribution::getMean() const
{
	return mu;
}

double NormalDistribution::getVariance() const
{
	return sigma * sigma;
}

double NormalDistribution::getCDF(double x) const
{
	return DCDFLIBInterface::computeNormalCDF(mu,sigma,x);
}

double NormalDistribution::getLogPDF(double x) const
{
	double y = (x - mu) / sigma;
	return (-0.5 * y * y) + log(0.3989422804 / sigma);
}

bool NormalDistribution::checkParameters(double mu, double sigma)
{
	return sigma > 0;
}

