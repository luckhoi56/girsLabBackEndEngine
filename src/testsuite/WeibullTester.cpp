// WeibullTester.cpp: implementation of the WeibullTester class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WeibullTester.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

WeibullTester::WeibullTester()
{
	handle = Java_redcas_numerical_NativeWeibull_createSessionN(0,0);
}

WeibullTester::~WeibullTester()
{
	Java_redcas_numerical_NativeWeibull_destroySessionN(0,0,handle);
	handle = -1;
}


void WeibullTester::executeSimpleI()
{

	Java_redcas_numerical_NativeWeibull_clearDataN(0,0,handle);
	Java_redcas_numerical_NativeWeibull_clearSamplesN(0,0,handle);

	Java_redcas_numerical_NativeWeibull_addDataPointN(0,0,handle,0,100,false);
	Java_redcas_numerical_NativeWeibull_addDataPointN(0,0,handle,0,200,true);
	Java_redcas_numerical_NativeWeibull_addDataPointN(0,0,handle,0,300,true);
	Java_redcas_numerical_NativeWeibull_addDataPointN(0,0,handle,0,400,true);
	Java_redcas_numerical_NativeWeibull_addDataPointN(0,0,handle,0,500,false);
	Java_redcas_numerical_NativeWeibull_addDataPointN(0,0,handle,0,600,true);
	Java_redcas_numerical_NativeWeibull_addDataPointN(0,0,handle,0,700,true);
	Java_redcas_numerical_NativeWeibull_addDataPointN(0,0,handle,0,800,true);

	Java_redcas_numerical_NativeWeibull_runAnalysisN(0,0,handle,5000);	

}
