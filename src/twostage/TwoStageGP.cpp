// TwoStageGP.cpp: implementation of the TwoStageGP class.
//
//////////////////////////////////////////////////////////////////////

#include "TwoStageGP.h"
#include "NHSession.h"
#include "TwoStageFunction.h"
#include "SampleSet.h"
#include <iostream>
#include <MetropolisLNGenerator.h>
#include "NHObservation.h"
#include "NHLikelihoodGammaPoisson.h"
#include "NHModelGammaPoisson.h"
#include "ThreeDeeFunction.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TwoStageGP::TwoStageGP()
{

}

TwoStageGP::~TwoStageGP()
{

}

void TwoStageGP::stepByStep(double k, double T)
{
	cout << "Starting nonhomogeneous update" << endl;

	NHSession session;
	session.setModel(gampois);

			session.addDataPoint( 5,100 );
			//session.addDataPoint( 5,100 );
			//session.addDataPoint( 5,100 );
			session.addDataPoint( 15,100 );
			//session.addDataPoint( 15,100 );
			//session.addDataPoint( 15,100 );
			//session.addDataPoint( 15,100 );
			//session.addDataPoint( 15,100 );
			//session.addDataPoint( 15,100 );
			session.addDataPoint( 25,100 );
			//session.addDataPoint( 25,100 );
			//session.addDataPoint( 25,100 );
			//session.addDataPoint( 25,100 );
			//session.addDataPoint( 25,100 );
			//session.addDataPoint( 25,100 );
			session.addDataPoint( 35,100 );
			//session.addDataPoint( 35,100 );
			//session.addDataPoint( 35,100 );
			//session.addDataPoint( 35,100 );
			//session.addDataPoint( 35,100 );
			session.addDataPoint( 45,100 );
			//session.addDataPoint( 45,100 );
			//session.addDataPoint( 45,100 );
			//session.addDataPoint( 45,100 );
			session.addDataPoint( 55,100 );
			//session.addDataPoint( 55,100 );
			//session.addDataPoint( 55,100 );
			session.addDataPoint( 65,100 );
			//session.addDataPoint( 65,100 );
			session.addDataPoint( 75,100 );
			session.addDataPoint( 85,100 );

	session.setSampleCount(1000); // NH sample size
	// non homogeneous first stage
	session.runAnalysis();

	cout << "Constructing pop var curve" << endl;

	TwoStageFunction func1;
	func1.prior = &session;
	func1.likelihood = 0;

	// set up homogenous sampler
	SampleSet samples1;
	MetropolisSampler * sampler1 = new MetropolisSampler(samples1);
	sampler1->setSampleSize(4000); // H sample size
	sampler1->setGenerator(new MetropolisLNGenerator());

	// homogeneous second stage
	sampler1->sample(func1);

	// construct distribution on lambda
	Distribution lambda1;
	samples1.getMarginal(0,lambda1);

	cout << "Starting homogeneous update" << endl;

	TwoStageFunction func2;
	func2.prior = &session;
	func2.likelihood = new GammaDistribution(1 / T,k + 1); 

	// set up homogenous sampler
	SampleSet samples2;
	MetropolisSampler * sampler2 = new MetropolisSampler(samples2);
	sampler2->setSampleSize(1000); // H sample size
	sampler2->setGenerator(new MetropolisLNGenerator());

	// homogeneous second stage
	sampler2->sample(func2);

	// construct distribution on lambda
	Distribution lambda2;
	samples2.getMarginal(0,lambda2);

	for (int i = 1 ; i < 100 ; i++)
		cout << i * 0.01 << " " << lambda1.getPercentile(i * 0.01) << " " << lambda2.getPercentile(i * 0.01) << endl;

}

void TwoStageGP::threeDee(double k, double T)
{
	NHInterpretation nhi(new NHLikelihoodGammaPoisson());

	nhi.addObservation(NHObservation(5,100 ));
	/*nhi.addObservation(NHObservation(5,100 ));
	nhi.addObservation(NHObservation(5,100 ));
	nhi.addObservation(NHObservation(15,100 ));
	nhi.addObservation(NHObservation(15,100 ));
	nhi.addObservation(NHObservation(15,100 ));
	nhi.addObservation(NHObservation(15,100 ));
	nhi.addObservation(NHObservation(15,100 ));*/
	nhi.addObservation(NHObservation(15,100 ));
	nhi.addObservation(NHObservation(25,100 ));
	/*nhi.addObservation(NHObservation(25,100 ));
	nhi.addObservation(NHObservation(25,100 ));
	nhi.addObservation(NHObservation(25,100 ));
	nhi.addObservation(NHObservation(25,100 ));
	nhi.addObservation(NHObservation(25,100 ));
	nhi.addObservation(NHObservation(35,100 ));
	nhi.addObservation(NHObservation(35,100 ));
	nhi.addObservation(NHObservation(35,100 ));
	nhi.addObservation(NHObservation(35,100 ));*/
	nhi.addObservation(NHObservation(35,100 ));
	nhi.addObservation(NHObservation(45,100 ));
	/*nhi.addObservation(NHObservation(45,100 ));
	nhi.addObservation(NHObservation(45,100 ));
	nhi.addObservation(NHObservation(45,100 ));
	nhi.addObservation(NHObservation(55,100 ));
	nhi.addObservation(NHObservation(55,100 ));*/
	nhi.addObservation(NHObservation(55,100 ));
	//nhi.addObservation(NHObservation(65,100 ));
	nhi.addObservation(NHObservation(65,100 ));
	nhi.addObservation(NHObservation(75,100 ));
	nhi.addObservation(NHObservation(85,100 ));

	GammaDistribution gd(1/T,k+1);

	ThreeDeeFunction tdf;

	tdf.nonhomogeneous = & nhi; 
	tdf.homogeneous = 0;

	SampleSet samples1;
	MetropolisSampler * sampler1 = new MetropolisSampler(samples1);
	sampler1->setSampleSize(20000); // H sample size
	sampler1->setGenerator(new MetropolisLNGenerator());

	cout << "Running 3D without homogeneous" << endl;

	// homogeneous second stage
	sampler1->sample(tdf);

	// construct distribution on lambda
	Distribution lambda1;
	samples1.getMarginal(2,lambda1);

	tdf.homogeneous = & gd;

	// set up homogenous sampler
	SampleSet samples2;
	MetropolisSampler * sampler2 = new MetropolisSampler(samples2);
	sampler2->setSampleSize(20000); // H sample size
	sampler2->setGenerator(new MetropolisLNGenerator());

	cout << "Running 3D with homogeneous" << endl;

	// homogeneous second stage
	sampler2->sample(tdf);

	// construct distribution on lambda
	Distribution lambda2;
	samples2.getMarginal(2,lambda2);


	SampleSet samples3;
	samples2.first();
	while (!samples2.endOfSamples()) {
		Parameters p = samples2.getParameters();
		double prob = samples2.getProbability();
		samples3.addSample(Parameters(p.getValue(0),p.getValue(1)),prob);
		samples2.next();
	}

	NHModelGammaPoisson model;
	model.setSampleSet(&samples3);
	Distribution dist;

	for (int i = 1 ; i < 100 ; i++)
		cout << i * 0.01 << " " << lambda1.getPercentile(i * 0.01) << " " << lambda2.getPercentile(i * 0.01) << endl;
	
	for (int j = 0 ; j < 100 ; j++) {
		double x = 0.03 + j * 0.005;
		model.computeMeasure(dflt,x,NHMEASURE_POPVAR_CDF,dist,0);
		cout << x << " " << dist.getMean() << endl;
	}

}
