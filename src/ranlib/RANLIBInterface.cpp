// $Header: C:/cvsrepo/ptdev/native/redcas/ranlib/RANLIBInterface.cpp,v 1.2 2001/07/24 21:31:46 fgroen Exp $

// RANLIBInterface.cpp: implementation of the RANLIBInterface class.
//
//////////////////////////////////////////////////////////////////////

#include "RANLIBInterface.h"
#include "ranlib.h"
#include "SyncMutex.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SyncMutex RANLIBInterface::mutex("numerical.library.ranlib");

double RANLIBInterface::generateBetaDeviate(double alpha, double beta)
{
	double result;
	mutex.obtain();
	result = genbet(alpha,beta);
	mutex.release();
	return result;
}

double RANLIBInterface::generateGammaDeviate(double scale, double shape)
{
	// shape has interpretation of arrival rate
	double result;
	mutex.obtain();
	result = gengam(scale,shape);
	mutex.release();
	return result;
}

double RANLIBInterface::generateNormalDeviate(double mu, double sigma)
{
	double result;
	mutex.obtain();
	result = gennor(mu,sigma);
	mutex.release();
	return result;
}

double RANLIBInterface::generateUniformDeviate()
{
	double result;
	mutex.obtain();
	result = ignlgi() / 2147483562.0;
	mutex.release();
	return result;
}

void RANLIBInterface::generateBetaDeviates(double alpha, double beta, int n, double *array)
{
	mutex.obtain();
	for (int i = 0 ; i < n ; i++) array[i] = genbet(alpha,beta);
	mutex.release();
}

void RANLIBInterface::generateNormalDeviates(double mu, double sigma, int n, double *array)
{
	mutex.obtain();
	for (int i = 0 ; i < n ; i++) array[i] = gennor(mu,sigma);
	mutex.release();	
}

void RANLIBInterface::generateGammaDeviates(double scale, double shape, int n, double *array)
{
	mutex.obtain();
	for (int i = 0 ; i < n ; i++) array[i] = gengam(scale,shape);
	mutex.release();
}
