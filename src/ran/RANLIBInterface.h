// $Header: C:/cvsrepo/ptdev/native/redcas/ranlib/RANLIBInterface.h,v 1.2 2001/07/24 21:31:46 fgroen Exp $

// RANLIBInterface.h: interface for the RANLIBInterface class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANLIBINTERFACE_H__C3951477_644D_4493_B4BB_549045F5EA64__INCLUDED_)
#define AFX_RANLIBINTERFACE_H__C3951477_644D_4493_B4BB_549045F5EA64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * Interface to RANLIB library: generation of random deviates.
 * The interface encapsulates the library in the form of a class
 * with static member methods, and delegates calls to the library 
 * in a thread-safe manner.
 *
 * Information about RANLIB library:
 * 
 *                                    RANLIB.C
 *
 *                 Library of C Routines for Random Number Generation
 *
 *
 *
 *                    Department of Biomathematics, Box 237
 *                    The University of Texas, M.D. Anderson Cancer Center
 *                    1515 Holcombe Boulevard
 *                    Houston, TX      77030
 *
 * This work was supported by grant CA-16672 from the National Cancer Institute.
 *
 * NOTE: in this library, the Gamma distribution's scale parameter has the
 * interpretation of failure rate, meaning that the mean of a Gamma distribution
 * with parameters scale and shape is given by shape / scale.
 */

class RANLIBInterface  
{
public:
	static void generateBetaDeviates(double alpha, double beta, int n, double * array);
	static void generateGammaDeviates(double scale, double shape, int n, double * array);
	static void generateNormalDeviates(double mu, double sigma, int n, double * array);
	static double generateNormalDeviate(double mu, double sigma);
	static double generateGammaDeviate(double scale, double shape);
	static double generateBetaDeviate(double alpha, double beta);
	static double generateUniformDeviate();
	virtual ~RANLIBInterface() {};
private:
	// static SyncMutex mutex;
};

#endif // !defined(AFX_RANLIBINTERFACE_H__C3951477_644D_4493_B4BB_549045F5EA64__INCLUDED_)
