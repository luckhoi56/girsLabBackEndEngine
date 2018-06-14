// $Header: C:/cvsrepo/ptdev/native/redcas/testsuite/AlgorithmTester.h,v 1.2 2001/07/24 21:31:42 fgroen Exp $

// AlgorithmTester.h: interface for the AlgorithmTester class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALGORITHMTESTER_H__42E1BDCD_0BE3_4912_9670_8D82760D7562__INCLUDED_)
#define AFX_ALGORITHMTESTER_H__42E1BDCD_0BE3_4912_9670_8D82760D7562__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <math.h>
#include <iostream>
#include <fstream.h>
#include <iomanip>
#include <time.h>
#include "redcas_numerical_NativeGamma.h"
#include "redcas_numerical_NativeNormal.h"
#include "redcas_numerical_NativeBeta.h"
#include "redcas_numerical_NativeLogNormal.h"
#include "redcas_numerical_NativeTrLogNormal.h"
#include "redcas_numerical_NativeGamma.h"
#include "redcas_numerical_NativeGamma.h"

class AlgorithmTester  
{
public:

	void testParametricDistributions();
	void testGammaPDFCDF();
	void testNormalPDFCDF();
	void testBetaPDFCDF();
	void testLogNormalPDFCDF();
	void testTrLogNormalPDFCDF();

	void testHomogeneous();
	void testHGammaPoisson();
	void testHBetaBinomial();
	void testHLogLog();
	void testHNormalNormal();
	void testHTabularPoisson();
	void testHTabularBinomial();
	void testHTabularNormal();
	void testHTabularLogNormal();

	void constructPDFCDFTable(int distribution, double a, double b);
	void constructSampleTable(int distribution, double a, double b);

	void endTestReport();
	void startTestReport();
	AlgorithmTester();
	virtual ~AlgorithmTester();
private:
	ofstream datafile;
	ifstream rsltfile;
	ofstream texfile;
	bool readdata;
	bool writedata;
	int getLogNormalParameterCount();
	void getLogNormalParameters(int index, double &a, double &b);
	int getNormalParameterCount();
	void getNormalParameters(int index, double &a, double &b);
	int getTrLogNormalParameterCount();
	void getTrLogNormalParameters(int index, double &a, double &b);
	void getBetaParameters(int index, double & a, double & b);
	int getBetaParameterCount();
	void getGammaParameters(int index, double & a, double & b);
	int getGammaParameterCount();
	char * mx(double x,char * b);
	void writeHTabularCase(int tabSize, double * tabx, double * tabc, int posSize, double * posx, int likelihood, double p1, double p2);
	void readHTabularCase(int posSize, double * posc);
	int getTabularPriorCount();
	void getTabularPrior(int i,int &tabsize, double * tabx,double * tabc);
};

#endif // !defined(AFX_ALGORITHMTESTER_H__42E1BDCD_0BE3_4912_9670_8D82760D7562__INCLUDED_)
