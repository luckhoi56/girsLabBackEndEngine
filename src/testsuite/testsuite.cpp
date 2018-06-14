// testsuite.cpp : Defines the entry point for the console application.
//



#include "stdafx.h"

#include <iostream>
#include <math.h>
#include "NHSession.h"

//#include "TabularDistribution.h"
//#include "WeibullInterpretation.h"

//#define CERTAIN_FAILURE 1
//#define CERTAIN_SUCCESS 2
//#define BINNED_OBSERVATION 3

#include "NHSession.h"

#include "TabularLogNormalBinomialIntegrator.h"
#include "UniformPoissonDistribution.h"
#include "TabularDistribution.h"
#include "Beta0595InputMode.h"
#include "Gamma0595InputMode.h"
#include "NHSession.h"
#include "TabularLogNormalBinomialIntegrator.h"
#include "TabularLogNormalPoissonIntegrator.h"
#include "TabularDistribution.h"
#include "..\parametric\LogNormalDistribution.h"
#include "..\homogeneous\LogNormalPoissonDistribution.h"
#include <iostream.h>

#include <time.h>


int main(int argc, char* argv[])
{
	// Test lognormal distribution
	LogNormalDistribution logDist(log(0.0001265), 1.1213);
	double dMean = logDist.getMean();
	double dMedian = logDist.getMedian();
	double dVar = logDist.getVariance();

	double d5thEst = logDist.getQuantile(0.05);
	double d95thEst = logDist.getQuantile(0.95);


	// Test Lognormal Possion distribution
	LogNormalPoissonDistribution logPosDist(log(0.0001265), 1.1213, 0, 5000);
	dMean = logPosDist.getMean();
	dMedian = logPosDist.getMedian();
	dVar = logPosDist.getVariance();

	double o1,o2;
	Beta0595InputMode im;
	im.convert(0.001,0.01,o1,o2);

	exit(0);


	double vals[] = {0.001,0.9};
	double cdfs[] = {0.0,1.0};

	TabularDistribution * td = new TabularDistribution(vals,cdfs,2,TABULAR_LOG_BINOMIAL,0,100.5);

	double mn = td->getMean();

	exit(0);

	NHSession session;

	session.setModel(gampois);

	// Data Set #02
	/*
	session.setPrior(0,log(1e-5), log(100.0) / 1.64485, log(1.0), log(100.0) / 1.64485);

	session.addDataPoint( 0, 0, 0, 6.433E4);
    session.addDataPoint( 0, 0, 0, 8.295E4);
    session.addDataPoint( 0, 0, 0, 1.244E4);
    session.addDataPoint( 2.1E-6, log(3) / 1.64485, 0, 0);
    session.addDataPoint( 1.4E-6, log(6) / 1.64485, 0, 0);
    session.addDataPoint( 2.0E-5, log(4) / 1.64485, 0, 0);
    session.addDataPoint( 1.4E-6, log(4) / 1.64485, 0, 0);
    session.addDataPoint( 1.9E-6, log(9) / 1.64485, 0, 0);
	*/

	//Data Set #03
	session.setPrior(0,log(1e-6), log(100.0) / 1.64485, log(1.0), log(100.0) / 1.64485);

	session.addDataPoint( 0, 0, 0, 6.368E4);
    session.addDataPoint( 0, 0, 0, 7.93E4);
    session.addDataPoint( 3.0E-6, log(10) / 1.64485, 0, 0);
    session.addDataPoint( 1.8E-5, log(6) / 1.64485, 0, 0);
    session.addDataPoint( 8.4E-6, log(7) / 1.64485, 0, 0);
    	

	session.setSampleCount(20000);

	int retval = session.runAnalysis();



/*
	session.setModel(logpois);
	session.addDataPoint(0,0,	0.007053061	,	1000	);
	session.addDataPoint(0,0,	0.01033042	,	1000	);
	session.addDataPoint(0,0,	0.013160698	,	1000	);
	session.addDataPoint(0,0,	0.015790042	,	1000	);
	session.addDataPoint(0,0,	0.018311809	,	1000	);
	session.addDataPoint(0,0,	0.020773112	,	1000	);
	session.addDataPoint(0,0,	0.023201853	,	1000	);
	session.addDataPoint(0,0,	0.025616509	,	1000	);
	session.addDataPoint(0,0,	0.02803025	,	1000	);
	session.addDataPoint(0,0,	0.030452536	,	1000	);
	session.addDataPoint(0,0,	0.03289104	,	1000	);
	session.addDataPoint(0,0,	0.035352099	,	1000	);
	session.addDataPoint(0,0,	0.037840993	,	1000	);
	session.addDataPoint(0,0,	0.040362116	,	1000	);
	session.addDataPoint(0,0,	0.042919847	,	1000	);
	session.addDataPoint(0,0,	0.045517642	,	1000	);
	session.addDataPoint(0,0,	0.048159096	,	1000	);
	session.addDataPoint(0,0,	0.050847385	,	1000	);
	session.addDataPoint(0,0,	0.053585856	,	1000	);
	session.addDataPoint(0,0,	0.056377516	,	1000	);
	session.addDataPoint(0,0,	0.059225372	,	1000	);
	session.addDataPoint(0,0,	0.062132575	,	1000	);
	session.addDataPoint(0,0,	0.065102026	,	1000	);
	session.addDataPoint(0,0,	0.068136788	,	1000	);
	session.addDataPoint(0,0,	0.071239957	,	1000	);
	session.addDataPoint(0,0,	0.074415017	,	1000	);
	session.addDataPoint(0,0,	0.077664805	,	1000	);
	session.addDataPoint(0,0,	0.080993067	,	1000	);
	session.addDataPoint(0,0,	0.084402947	,	1000	);
	session.addDataPoint(0,0,	0.087898169	,	1000	);
	session.addDataPoint(0,0,	0.091482784	,	1000	);
	session.addDataPoint(0,0,	0.095160258	,	1000	);
	session.addDataPoint(0,0,	0.098934843	,	1000	);
	session.addDataPoint(0,0,	0.102811029	,	1000	);
	session.addDataPoint(0,0,	0.106792839	,	1000	);
	session.addDataPoint(0,0,	0.110885437	,	1000	);
	session.addDataPoint(0,0,	0.115093467	,	1000	);
	session.addDataPoint(0,0,	0.119422409	,	1000	);
	session.addDataPoint(0,0,	0.123877493	,	1000	);
	session.addDataPoint(0,0,	0.12846483	,	1000	);
	session.addDataPoint(0,0,	0.133190028	,	1000	);
	session.addDataPoint(0,0,	0.13806002	,	1000	);
	session.addDataPoint(0,0,	0.143081888	,	1000	);
	session.addDataPoint(0,0,	0.148262352	,	1000	);
	session.addDataPoint(0,0,	0.153609599	,	1000	);
	session.addDataPoint(0,0,	0.15913197	,	1000	);
	session.addDataPoint(0,0,	0.164837918	,	1000	);
	session.addDataPoint(0,0,	0.17073705	,	1000	);
	session.addDataPoint(0,0,	0.176839697	,	1000	);
	session.addDataPoint(0,0,	0.183156389	,	1000	);
	session.addDataPoint(0,0,	0.189698713	,	1000	);
	session.addDataPoint(0,0,	0.196479105	,	1000	);
	session.addDataPoint(0,0,	0.203510595	,	1000	);
	session.addDataPoint(0,0,	0.210807815	,	1000	);
	session.addDataPoint(0,0,	0.218386501	,	1000	);
	session.addDataPoint(0,0,	0.226262853	,	1000	);
	session.addDataPoint(0,0,	0.234454992	,	1000	);
	session.addDataPoint(0,0,	0.242983181	,	1000	);
	session.addDataPoint(0,0,	0.251867675	,	1000	);
	session.addDataPoint(0,0,	0.261131881	,	1000	);
	session.addDataPoint(0,0,	0.270801918	,	1000	);
	session.addDataPoint(0,0,	0.280904256	,	1000	);
	session.addDataPoint(0,0,	0.291469738	,	1000	);
	session.addDataPoint(0,0,	0.302530826	,	1000	);
	session.addDataPoint(0,0,	0.314124648	,	1000	);
	session.addDataPoint(0,0,	0.326290506	,	1000	);
	session.addDataPoint(0,0,	0.339074301	,	1000	);
	session.addDataPoint(0,0,	0.352523875	,	1000	);
	session.addDataPoint(0,0,	0.366694816	,	1000	);
	session.addDataPoint(0,0,	0.381649167	,	1000	);
	session.addDataPoint(0,0,	0.397453691	,	1000	);
	session.addDataPoint(0,0,	0.414186846	,	1000	);
	session.addDataPoint(0,0,	0.431936483	,	1000	);
	session.addDataPoint(0,0,	0.450799638	,	1000	);
	session.addDataPoint(0,0,	0.470891113	,	1000	);
	session.addDataPoint(0,0,	0.492337017	,	1000	);
	session.addDataPoint(0,0,	0.515287536	,	1000	);
	session.addDataPoint(0,0,	0.539914255	,	1000	);
	session.addDataPoint(0,0,	0.566417085	,	1000	);
	session.addDataPoint(0,0,	0.5950291	,	1000	);
	session.addDataPoint(0,0,	0.626028306	,	1000	);
	session.addDataPoint(0,0,	0.659744117	,	1000	);
	session.addDataPoint(0,0,	0.696571692	,	1000	);
	session.addDataPoint(0,0,	0.736994747	,	1000	);
	session.addDataPoint(0,0,	0.781602579	,	1000	);
	session.addDataPoint(0,0,	0.831132421	,	1000	);
	session.addDataPoint(0,0,	0.88650589	,	1000	);
	session.addDataPoint(0,0,	0.948918557	,	1000	);
	session.addDataPoint(0,0,	1.019921014	,	1000	);
	session.addDataPoint(0,0,	1.101591778	,	1000	);
	session.addDataPoint(0,0,	1.196787862	,	1000	);
	session.addDataPoint(0,0,	1.309556378	,	1000	);
	session.addDataPoint(0,0,	1.445844098	,	1000	);
	session.addDataPoint(0,0,	1.614888687	,	1000	);
	session.addDataPoint(0,0,	1.831946923	,	1000	);
	session.addDataPoint(0,0,	2.124520192	,	1000	);
	session.addDataPoint(0,0,	2.548972888	,	1000	);
	session.addDataPoint(0,0,	3.247328114	,	1000	);
	session.addDataPoint(0,0,	4.756270212	,	1000	);

	session.setPrior(2,log(0.001),3,log(10),1.4);

	session.setSampleCount(2000);
	session.runAnalysis();

	double cdf05 = session.getMeasureMean(dflt,0.0183/1000,cdf);
	double cdf50 = session.getMeasureMean(dflt,0.183/1000,cdf);
	double cdf95 = session.getMeasureMean(dflt,1.83/1000,cdf);

	cout << cdf05 << endl;
	cout << cdf50 << endl;
	cout << cdf95 << endl;

*/


	// bet-log        OK
    // bet-bin        OK
    // bet-bin-log    OK

	//session.setModel(betbin);
	//session.addDataPoint(0.00001,0.8,0,20000);
	//double l1 = session.computeLnLikelihood(0.5,1);
	//double l2 = session.computeLnLikelihood(1,10000);
	//double l3 = session.computeLnLikelihood(10,990);
	//double x2 = l2 - l1;
	//double x3 = l3 - l1;

	return 0;
}
