// $Header: C:/cvsrepo/ptdev/native/redcas/dcdflib/DCDFLIBInterface.h,v 1.7 2002/06/25 02:46:32 fgroen Exp $

// DCDFLIBInterface.h: interface for the DCDFLIBInterface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCDFLIBINTERFACE_H__2849D446_9B45_46CC_A8E2_9AFEEB262DB5__INCLUDED_)
#define AFX_DCDFLIBINTERFACE_H__2849D446_9B45_46CC_A8E2_9AFEEB262DB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SyncMutex.h"

/**
 * Interface to DCDFLIB library of numerical routines.
 * The interface encapsulates the library in the form of a class
 * with static member methods, and delegates calls to the library 
 * in a thread-safe manner.
 * 
 * Library Info:
 *
 *                              DCDFLIB
 *
 *               Library of C Routines for Cumulative Distribution
 *                Functions, Inverses, and Other Parameters
 *
 *                              (February, 1994)
 *
 *                   Department of Biomathematics, Box 237
 *             The University of Texas, M.D. Anderson Cancer Center
 *                     1515 Holcombe Boulevard
 *                     Houston, TX      77030
 *
 *
 * This work was supported by grant CA-16672 from the National Cancer Institute.
 *
 * NOTE: in this library, the Gamma distribution's scale parameter has the
 * interpretation of failure rate, meaning that the mean of a Gamma distribution
 * with parameters scale and shape is given by shape / scale.
 */

class DCDFLIBInterface  
{
public:
	static double computeInverseErf(double z);
	static double computeErf(double x);
	static double computeErfc(double x);
	static double computeDeltaErf(double lo, double hi);
	static double computeDiGamma(double x);
	static double computeLnGamma(double x);
	static double computeLnBeta(double a, double b);
	static double computeBetaQuantile(double alpha, double beta, double p);
	static double computeBetaCDF(double alpha, double beta, double x);
	static double computeDeltaBetaCDF(double a, double b, double lo, double hi);
	static double computeBetaPDF(double alpha, double beta, double x);
	static double computeGammaQuantile(double shape, double theta, double p);
	static double computeNormalQuantile(double mu, double sigma, double p);
	static double computeNormalCDF(double mu, double sigma, double x);
	static double computeGammaCDF(double shape, double theta, double x);
	static double computeDeltaGammaCDF(double shape, double theta, double lo, double hi);
	static double computeGammaPDF(double shape, double theta, double x);
	//static double computeLogGammaPDF(double shape, double scale, double x);
	static double computeLognormalQuantile(double nu, double tau, double p);
	static double computeLognormalCDF(double nu, double tau, double x);
	static double computeLognormalPDF(double nu, double tau, double x);
	//static double computeLogLognormalPDF(double nu, double tau, double x);

	virtual ~DCDFLIBInterface() {};
private:
	static SyncMutex mutex;

};

#endif // !defined(AFX_DCDFLIBINTERFACE_H__2849D446_9B45_46CC_A8E2_9AFEEB262DB5__INCLUDED_)
