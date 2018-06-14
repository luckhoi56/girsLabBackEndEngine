//
// File:      $Id: NHSession.cpp 2308 2014-06-22 04:16:33Z Dongfeng Zhu $
// Revision:  $Revision: 2308 $
// Copyright: (c) 2002, Enrique Lopez Droguett and Frank Groen. All rights reserved.
//


#include "NHSession.h"
#include "NHObservation.h"
#include "NHModelBetaBinomial.h"
#include "NHModelGammaPoisson.h"
#include "NHModelLogPoisson.h"
#include "NHModelLogBinomial.h"
#include "NHModelLogLog.h"
#include "NHFunction.h"
#include "../global/HyperIndex.h"
#include "../parametric/GammaDistribution.h"
#include "../parametric/LogNormalDistribution.h"
#include "../parametric/BetaDistribution.h"

// #include <io.h>
// #include <direct.h>
#include <iostream>
#include <fstream>
#include <iomanip>

NHSession::NHSession()
{
	interpretation = 0;
	model = 0;
	user_prior = 0;
	parameterBoundsComputed = false;
}

NHSession::~NHSession()
{
	delete this->interpretation;
	delete this->model;
//	delete this->user_prior;
}

int NHSession::setModel(NHModels model)
{

#ifdef _DEBUG

	switch (model) {
		case loglog:	cout << "switching to lognormal lognormal model" << endl; break;
		case logpois:   cout << "switching to lognormal poisson model" << endl; break;
		case gampois:   cout << "switching to gamma poisson model" << endl; break;
		case betbin:	cout << "switching to beta binomial" << endl; break;
		case logbin:	cout << "switching to lognormal binomial" << endl; break;
		case normnorm:	cout << "switching to unknown model" << endl; break;
	}
#endif

	clearData();
	clearSamples();

	delete this->interpretation;
	delete this->model;

	NHModel * newModel = 0;
	NHInterpretation * newInterpretation = 0;

	switch (model) {
		case logpois:   newModel = new NHModelLogPoisson(); break;   // time-based, Lognormal
		case gampois:   newModel = new NHModelGammaPoisson(); break; // time-based, Gamma
		case betbin:	newModel = new NHModelBetaBinomial(); break; // demand-based, Beta
		case logbin:    newModel = new NHModelLogBinomial(); break;

		case loglog:	newModel = new NHModelLogLog(); break;
		case normnorm:	newModel = 0; break;
	}

	if (newModel) {
		newModel->setSampleSet(getSampleSet());
		newInterpretation = newModel->createInterpretation(0);
	}

	this->model = newModel;
	this->interpretation = newInterpretation;

	return model && interpretation;
}

/*
void NHSession::addDataPoint(double a, double b)
{

#ifdef _DEBUG
	if (!interpretation) cout << "DLL: interpretation missing: data can not be added!" << endl;
#endif

	if (this->interpretation)
		this->interpretation->addObservation(NHObservation(a,b));
}
*/

/*
void NHSession::addDataPoint(double a, double b, double c)
{

#ifdef _DEBUG
	if (!interpretation) cout << "DLL: interpretation missing: data can not be added!" << endl;
#endif

	if (this->interpretation)
		this->interpretation->addObservation(NHObservation(a,b,c));
}
*/

double NHSession::computeLnLikelihood(double p1,double p2) const
{
	if (interpretation == 0) return 0;
	Parameters p(p1,p2);
	double ln_like = interpretation->getLogLikelihood(p);
	return ln_like;
}

void NHSession::addDataPoint(double a, double b, double c, double d)
{

#ifdef _DEBUG
	if (!interpretation) cout << "DLL: interpretation missing: data can not be added!" << endl;
#endif

	if (this->interpretation)
		this->interpretation->addObservation(NHObservation(a,b,c,d));
}

void NHSession::clearData()
{
	delete interpretation;
	if (model) interpretation = model->createInterpretation(0);
	clearMeasure();
}

void NHSession::setPrior(int mode, double nu1, double tau1, double nu2, double tau2)
{
	if (user_prior != 0) {
		delete user_prior;
	}

	if (model != 0) {
		user_prior = model->createUserPrior(mode,nu1,tau1,nu2,tau2);
	}
}

int NHSession::runAnalysis()
{
	int retval = -1;

#ifdef _DEBUG
	cout << "session starting analysis" << endl;
#endif

	clearBounds();

	if (this->model && this->interpretation) {


		if (!model->accept(*interpretation)) {
			return STATUS_INSUFFICIENT_DATA;
		}

		// detach the sample set from the model
		model->setSampleSet(0);

		// retrieves a sample generator appropriate for this model
		getSampler()->setGenerator(model->createGenerator());

		// construct the function to be sampled from
		NHPrior * prior = user_prior;
		user_prior = 0; // can use only once, because of way it is deallocated
		if (prior == 0) prior = model->createDefaultPrior();
		NHFunction function(prior,interpretation);

		try {
			// performs the sampling
			retval = this->analyze(function);
		} catch (int errCode) {
#ifdef _DEBUG
			cout << "EXCEPTION THROWN: " << errCode << endl;
#endif
			clearMeasure();
			
			delete prior;
			
			//return errCode; 
			return STATUS_ERROR;
		}	
			
		if (retval == STATUS_OK) {
			computeBounds();
		}
		
		clearMeasure();

		delete prior;
	} else {

#ifdef _DEBUG
		cout << "DLL: model or interpretation missing!" << endl;
#endif
	}

	return retval;
}

double NHSession::getBinnedDensity(int a, int b)
{
	HyperIndex hyper(2);
	hyper.setValue(0,a);
	hyper.setValue(1,b);
	return getBinnedDensityA(hyper);
}

 double NHSession::getSampleAlpha(int index)
 {
 	return getSampleParameter(0,index);
 }

 double NHSession::getSampleBeta(int index)
 {
 	return getSampleParameter(1,index);
 }

double NHSession::getBinnedAlpha(int a) {return getBinnedParameter(0,a);}

double NHSession::getBinnedBeta(int b) {return getBinnedParameter(1,b);}

/* returns recommended parameter lower bound for the current pop var curve */
double NHSession::getPopVarLowerBound()
{
	computeBounds();
	return parameterLowerBound;
}

/* returns recommended parameter upper bound for the current pop var curve */
double NHSession::getPopVarUpperBound()
{
	computeBounds();
	return parameterUpperBound;
}

void NHSession::computeBounds()
{
	_status->setStatusCode(STATUS_ACTIVE);
	_status->setStatus(0,"Determining variability bounds...");
	if (!parameterBoundsComputed && getSampleSet()->size() > 0 && model) {
		model->setSampleSet(getSampleSet());
		model->computeBounds(0,parameterLowerBound,parameterUpperBound);
		parameterBoundsComputed = true;
	}

	_status->setProgress(100);
	_status->setStatusCode(STATUS_IDLE);
}

void NHSession::clearBounds()
{
	parameterBoundsComputed = false;
}


double NHSession::findMeanPercentile(double cdf)
{
	double retval = 0;

	_status->setStatusCode(STATUS_ACTIVE);
	_status->setStatus(0,"Determining variability bounds...");
	if (getSampleSet()->size() > 0 && model) {
		model->setSampleSet(getSampleSet());
		retval = model->findPercentile(cdf,1.0001);
	}
	_status->setProgress(100);
	_status->setStatusCode(STATUS_IDLE);
	return retval;
}

/// returns a percentile of the last computed measure */
double NHSession::getMeasure(NHParameters parameter, double value, NHMeasures measure, double quantile)
{
	computeMeasure(parameter,value,measure);
	return distribution.size() > 0 ? distribution.getPercentile(quantile) : 0;
}

/// returns the mean of the last computed measure */
double NHSession::getMeasureMean(NHParameters parameter, double value, NHMeasures measure)
{
	computeMeasure(parameter,value,measure);
	return distribution.size() > 0 ? distribution.getMean() : 0;
}

/// computes the specified distribution, if not already computed */
void NHSession::computeMeasure(NHParameters param, double value, NHMeasures measure)
{

#ifdef _DEBUG
	//cout << "session computing measure " << value << " " << measure << endl;
#endif

	if ((param != distParameter || value != distValue || measure != distMeasure || 
		distribution.size() == 0) && model != 0 && getSampleSet()->size() != 0) {

		int msr ;
		switch(measure) {
		case pdf: 
			msr = NHMEASURE_POPVAR_PDF; 
			model->computeMeasure(param,value,msr,distribution,_status);
			break;
		case cdf: 
			msr = NHMEASURE_POPVAR_CDF; 
			model->computeMeasure(param,value,msr,distribution,_status);
			break;
		case quantile: 
			model->computePercentile(value,distribution,_status);
			break;
		case mean:
			model->computeMean(distribution,_status);
			break;
		case variance:
			model->computeVariance(distribution,_status);
		}
		
		distParameter = param;
		distValue = value;
		distMeasure = measure;
		cursorChanged();
	}
}

/* clears the measure uncertainty distribution. */
void NHSession::clearMeasure()
{
	distribution.clear();
}

void NHSession::clearSamplesEx()
{
	clearBounds();
	clearSamples();
}

void NHSession::addSample(double alpha, double beta, double prob)
{
	Parameters param = Parameters(alpha, beta);
	addSampleA(param, prob);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void NHSession::testUnit(char * dir)
{
/*
	struct _finddata_t nh_file;
    long hFile;
	_chdir(dir);
    if( (hFile = _findfirst( "*.nh", &nh_file )) != -1L ) {
		NHSession session;
		NHModels modelselect;

		char caseName[100];
		char theoreticalDistName[15];
		int modelType;
		double v1, v2;
		int sampleSize;
		double field1, field2;
		int samples;
		int i;
		ParametricDistribution *theoreticalDist = NULL;
		double lower, higher;
		double tvalue_5, tvalue_50, tvalue_95;
		double tmean, tvar;
		double svalue_5, svalue_50, svalue_95;
		double smean, svar;

		double eventcnt;
		double exposurecnt;

		ofstream out( "NHResult.txt" );
		out.setf(ios::scientific);
		out << setprecision(4);		
		do {
			ifstream in(nh_file.name);
			in.getline(caseName, 100);
			in >> modelType;
			in >> v1 >> v2;
			in >> sampleSize;
			in >> samples;
			switch (modelType) {
			case 1:	modelselect = logpois; 
					session.setModel(logpois);
					theoreticalDist = new LogNormalDistribution(log(v1), v2);
					strcpy(theoreticalDistName, "LogNormal");
					break;
			case 2: modelselect = gampois; 
					session.setModel(gampois);
					theoreticalDist = new GammaDistribution(v2, v1); // Scale, Shape
					strcpy(theoreticalDistName, "Gamma");
					break;
			case 3: modelselect = loglog; 
					session.setModel(loglog);
					theoreticalDist = new LogNormalDistribution(log(v1), v2);
					strcpy(theoreticalDistName, "LogNormal");
					break;
			case 4: modelselect = normnorm; 
					session.setModel(normnorm);
					break;
			case 5: modelselect = betbin; 
					session.setModel(betbin);
					theoreticalDist = new BetaDistribution(v1, v2); // Alpha, Beta
					strcpy(theoreticalDistName, "Beta");
					break;
			}
			
			exposurecnt = 0;
			eventcnt = 0;

			for(i = 0; i < sampleSize; i++) {
				in >> field1 >> field2;
				session.addDataPoint( field1, field2 );

				if (modelselect == betbin || modelselect == gampois || modelselect == logpois) {
					exposurecnt += field2;
					eventcnt += field1;
				}
			}
			in.close();
			
			cout << endl << "Test Case: " << nh_file.name << endl << endl;
			out << "#################################################################" << endl << endl;
			out << nh_file.name << endl << endl;
			out << "#################################################################" << endl << endl;

			bool shouldrun = true;

			if ((modelselect == betbin || modelselect == gampois) && eventcnt < 3) shouldrun = false;
			if (modelselect == logpois && (eventcnt < 3 || eventcnt > 500)) shouldrun = false;

		
			if (!shouldrun) {

				cout << "BAD DATA: CASE ABORTED" << endl;
				out << "BAD DATA: CASE ABORTED" << endl << endl;

			} else {
				session.setSampleCount(samples);
				int retval = session.runAnalysis();
				if(retval != STATUS_OK) {
					cout << "Error in the sampling process!" << endl;
					cout << "Simulation aborted." << endl;

					out << caseName << endl;
					out << "Error in the sampling process!" << endl;
					out << "Error code: " << retval << endl;
					out << "Simulation aborted." << endl;
					break;
				}

				tvalue_5 = theoreticalDist->getQuantile(0.05);
				tvalue_50 = theoreticalDist->getQuantile(0.50);
				tvalue_95 = theoreticalDist->getQuantile(0.95);

				tmean = theoreticalDist->getMean();
				tvar = theoreticalDist->getVariance();
				

				lower = theoreticalDist->getQuantile(0.000001);
				higher = theoreticalDist->getQuantile(0.9999999);

				double low = session.getPopVarLowerBound();
				double high = session.getPopVarUpperBound();

				if (modelselect == betbin || modelselect == gampois || modelselect == logpois) {
					cout << endl << "Estimating 5th Percentile...";
//					session.model->findPercentile(svalue_5, low, high, 0.05);
					cout << endl << "Estimating 50th Percentile...";
//					session.model->findPercentile(svalue_50, svalue_5, high, 0.50);
					cout << endl << "Estimating 95th Percentile...";
//					session.model->findPercentile(svalue_95, svalue_50, high, 0.95);
				} else {
					cout << endl << "Estimating 5th Percentile...";
//					session.model->findPercentile(svalue_5, lower, higher, 0.05);
					cout << endl << "Estimating 50th Percentile...";
//					session.model->findPercentile(svalue_50, lower, higher, 0.50);
					cout << endl << "Estimating 95th Percentile...";
//					session.model->findPercentile(svalue_95, lower, higher, 0.95);
				}

				int intervals = 20;
				int steps = 100;
				double c, c_prev, x;
				double inc;
				
				cout << endl << "Estimating Mean and Standard Deviation...";

				Distribution mndist;
				session.model->computeMeasure(0,0,NHMEASURE_POPVAR_MEAN,mndist,0);
				smean = mndist.getMean();

				cout << endl << "Dumping results..." << endl << "Done!" << endl;
				
				out << "Model:                 " << caseName << endl;
				out << "Theoretical dist:      " << theoreticalDistName << endl;

				switch (modelType) {
				case 1:
				case 3:	out << "Nu =                   " << v1 << endl << "Tau =                  " << v2 << endl;
						break;
				case 2:	out << "Shape =                " << v1 << endl << "Scale =                " << v2 << endl;
						break;
				case 5:	out << "Alpha =                " << v1 << endl << "Beta =                 " << v2 << endl;
						break; 
				}						

				out << "Number of Data Points: " << sampleSize << ((sampleSize <= 5) ? " <<<<<<<<<<<<<<<<<<" : "") << endl;
				out << "Number of Samples:     " << samples << endl;

				if (modelselect == betbin || modelselect == gampois || modelselect == logpois) {
					out << "Total Exposure:        " << exposurecnt << endl;
					out << "Total Events:          " << eventcnt << ((eventcnt == 0) ? " <<<<<<<<<<<<<<<<<<" : "") << endl;
				}  

				out << endl << "Population Variability:" << endl << endl;
				out << "Measure" << "\t" << "Theoretical" << "\t" << "Obtained" << endl;
				out << "Mean" << "\t" << tmean << "\t" << smean << "\t" << tmean / smean << endl;
				//out << "Std" << "\t" << sqrt(tvar) << "\t" << sqrt(svar) << endl;
				out << "5th" << "\t" << tvalue_5 << "\t" << svalue_5 << "\t" << tvalue_5 / svalue_5 << endl;
				out << "50th" << "\t" << tvalue_50 << "\t" << svalue_50 << "\t" << tvalue_50 / svalue_50 << endl;
				out << "95th" << "\t" << tvalue_95 << "\t" << svalue_95 << "\t" << tvalue_95 / svalue_95 << endl;
				out << "Min" << "\t" << " -         " << "\t" << low << endl;
				out << "Max" << "\t" << " -         " << "\t" << high << endl << endl;
			}

			delete theoreticalDist;
			theoreticalDist = 0;
		}
		while (_findnext( hFile, &nh_file ) == 0);
		out.close();
		_findclose( hFile );
	}
	else
		cout <<"No *.nh files in current directory!\n" << endl;
*/
}


void NHSession::testBetaBinomial()
{
/*
	int NumberOfPasses = 1;
	int Pass = 1;

	NHSession session;
	NHModels modelselect;

	modelselect = betbin;
	
	ofstream out( "NHTestBB.dat" );
	
	//session.setModel(betbin);

	
	//ifstream in( "c:\\Trash\\data.bb.txt" );
	//session.clearData();
	//double demands, events;

	//for(int i = 0; i < 41; i++) {
	//	in >> demands >> events;
	//	session.addDataPoint( events, demands );
	//}
	

	cout << "NonHomogneous Beta-Binomial:" << endl;
	out << "NonHomogneous Beta-Binomial:" << endl;
	do {
		cout << "Pass " << Pass << " out of " << NumberOfPasses << endl << endl;	
		out << "Pass " << Pass << " out of " << NumberOfPasses << endl << endl;

		for( int testCase = 1; testCase <= 1; testCase++ ) {
			session.setModel(betbin);

			if( testCase == 1 ) {
				cout << "Test Case #01: Alpha = 3  Beta = 2" << endl;
				out << "Test Case #01: Alpha = 3  Beta = 2" << endl;

				session.clearData();
				session.addDataPoint(0.1, 0.9, 15,100 );
				session.addDataPoint(0.2, 0.7, 25,100 );
				session.addDataPoint(0.3, 0.5, 25,100 );
				session.addDataPoint(0.4, 0.2, 35,100 );
				session.addDataPoint(0.5, 0.3, 35,100 );
				session.addDataPoint(0.6, 0.6, 35,100 );
				session.addDataPoint(0.7, 0.8, 35,100 );
				session.addDataPoint(0.8, 0.9, 45,100 );
				session.addDataPoint(0.9, 0.8, 45,100 );
				session.addDataPoint(1.0, 1, 45,100 );
				session.addDataPoint(1.1, 1.1, 45,100 );
				session.addDataPoint(1.2, 1.5, 45,100 );
				session.addDataPoint(1.5, 1.6, 45,100 );
				session.addDataPoint(0.4, 0.4, 55,100 );
				session.addDataPoint(0.3, 0.5, 55,100 );
				session.addDataPoint(0.5, 0.4, 55,100 );
				session.addDataPoint(0.6, 0.3, 55,100 );
				session.addDataPoint(0.6, 0.7, 55,100 );
				session.addDataPoint(0.4, 0.2, 55,100 );
				session.addDataPoint(0.3, 0.3, 55,100 );
				
				session.addDataPoint(0, 0, 65,100 );
				session.addDataPoint(0, 0, 65,100 );
				session.addDataPoint(0, 0, 65,100 );
				session.addDataPoint(0, 0, 65,100 );
				session.addDataPoint(0, 0, 65,100 );
				session.addDataPoint(0, 0, 65,100 );
				session.addDataPoint(0, 0, 65,100 );
				session.addDataPoint(0, 0, 65,100 );
				session.addDataPoint(0, 0, 75,100 );
				session.addDataPoint(0, 0, 75,100 );
				session.addDataPoint(0, 0, 75,100 );
				session.addDataPoint(0, 0, 75,100 );
				session.addDataPoint(0, 0, 75,100 );
				session.addDataPoint(0, 0, 75,100 );
				session.addDataPoint(0, 0, 75,100 );
				session.addDataPoint(0, 0, 85,100 );
				session.addDataPoint(0, 0, 85,100 );
				session.addDataPoint(0, 0, 85,100 );
				session.addDataPoint(0, 0, 85,100 );
				session.addDataPoint(0, 0, 85,100 );
				session.addDataPoint(0, 0, 85,100 );
				session.addDataPoint(0, 0, 95,100 );
				session.addDataPoint(0, 0, 95,100 );
			}
			else if( testCase == 2 ) {
				cout << "Test Case #02: Alpha = 2 Beta = 2" << endl;
				out << "Test Case #02: Alpha = 2 Beta = 2" << endl;

				session.clearData();
				session.addDataPoint( 5,100 );
				session.addDataPoint( 15,100 );
				session.addDataPoint( 15,100 );
				session.addDataPoint( 15,100 );
				session.addDataPoint( 25,100 );
				session.addDataPoint( 25,100 );
				session.addDataPoint( 25,100 );
				session.addDataPoint( 25,100 );
				session.addDataPoint( 35,100 );
				session.addDataPoint( 35,100 );
				session.addDataPoint( 35,100 );
				session.addDataPoint( 35,100 );
				session.addDataPoint( 35,100 );
				session.addDataPoint( 45,100 );
				session.addDataPoint( 45,100 );
				session.addDataPoint( 45,100 );
				session.addDataPoint( 45,100 );
				session.addDataPoint( 45,100 );
				session.addDataPoint( 45,100 );
				session.addDataPoint( 55,100 );
				session.addDataPoint( 55,100 );
				session.addDataPoint( 55,100 );
				session.addDataPoint( 55,100 );
				session.addDataPoint( 55,100 );
				session.addDataPoint( 55,100 );
				session.addDataPoint( 65,100 );
				session.addDataPoint( 65,100 );
				session.addDataPoint( 65,100 );
				session.addDataPoint( 65,100 );
				session.addDataPoint( 65,100 );
				session.addDataPoint( 75,100 );
				session.addDataPoint( 75,100 );
				session.addDataPoint( 75,100 );
				session.addDataPoint( 75,100 );
				session.addDataPoint( 85,100 );
				session.addDataPoint( 85,100 );
				session.addDataPoint( 85,100 );
				session.addDataPoint( 95,100 );
			}
			else if( testCase == 3 ) {
				cout << "Test Case #03: Alpha = 2 Beta = 50" << endl;
				out << "Test Case #03: Alpha = 2 Beta = 50" << endl;
				session.clearData();
				session.addDataPoint( 1,500 );
				session.addDataPoint( 4,500 );
				session.addDataPoint( 5,500 );
				session.addDataPoint( 7,500 );
				session.addDataPoint( 8,500 );
				session.addDataPoint( 9,500 );
				session.addDataPoint( 11,500 );
				session.addDataPoint( 12,500 );
				session.addDataPoint( 13,500 );
				session.addDataPoint( 15,500 );
				session.addDataPoint( 16,500 );
				session.addDataPoint( 18,500 );
				session.addDataPoint( 20,500 );
				session.addDataPoint( 21,500 );
				session.addDataPoint( 24,500 );
				session.addDataPoint( 26,500 );
				session.addDataPoint( 29,500 );
				session.addDataPoint( 32,500 );
				session.addDataPoint( 37,500 );
				session.addDataPoint( 45,500 );
				session.addDataPoint( 62,500 );
			}
			else if( testCase == 4 ) {
				cout << "Test Case #04: Alpha = 0.3 Beta = 3" << endl;
				out << "Test Case #04: Alpha = 0.3 Beta = 3" << endl;
					
				session.clearData();
				session.addDataPoint( 0,1000 );
				session.addDataPoint( 0,1000 );
				session.addDataPoint( 0,1000 );
				session.addDataPoint( 0,1000 );
				session.addDataPoint( 1,1000 );
				session.addDataPoint( 3,1000 );
				session.addDataPoint( 5,1000 );
				session.addDataPoint( 8,1000 );
				session.addDataPoint( 13,1000 );
				session.addDataPoint( 19,1000 );
				session.addDataPoint( 27,1000 );
				session.addDataPoint( 38,1000 );
				session.addDataPoint( 52,1000 );
				session.addDataPoint( 69,1000 );
				session.addDataPoint( 92,1000 );
				session.addDataPoint( 121,1000 );
				session.addDataPoint( 158,1000 );
				session.addDataPoint( 209,1000 );
				session.addDataPoint( 282,1000 );
				session.addDataPoint( 401,1000 );
				session.addDataPoint( 625,1000 );
			}
			else if( testCase == 5 ) {
				cout << "Test Case #05: Alpha = 0.5 Beta = 0.6" << endl;
				out << "Test Case #05: Alpha = 0.5 Beta = 0.6" << endl;
					
				session.clearData();
				session.addDataPoint( 0.0,100 );
				session.addDataPoint( 0.5,100 );
				session.addDataPoint( 1.9,100 );
				session.addDataPoint( 4.3,100 );
				session.addDataPoint( 7.5,100 );
				session.addDataPoint( 11.6,100 );
				session.addDataPoint( 16.5,100 );
				session.addDataPoint( 22.1,100 );
				session.addDataPoint( 28.3,100 );
				session.addDataPoint( 35,100 );
				session.addDataPoint( 42.1,100 );
				session.addDataPoint( 49.4,100 );
				session.addDataPoint( 56.9,100 );
				session.addDataPoint( 64.3,100 );
				session.addDataPoint( 71.6,100 );
				session.addDataPoint( 78.4,100 );
				session.addDataPoint( 84.8,100 );
				session.addDataPoint( 90.4,100 );
				session.addDataPoint( 95,100 );
				session.addDataPoint( 98.4,100 );
				session.addDataPoint( 99.9,100 );
			}
			else if( testCase == 6 ) {
				cout << "Test Case #06: Bad Data!" << endl;
				out << "Test Case #06: Bad Data!" << endl;
					
				session.clearData();
				session.addDataPoint( 1, 1000 );
				session.addDataPoint( 1, 1500 );
				session.addDataPoint( 1, 10000 );
				session.addDataPoint( 1, 30000 );
				session.addDataPoint( 1, 250 );
				session.addDataPoint( 1, 600 );
				session.addDataPoint( 1, 350 );
				session.addDataPoint( 1, 20000 );
				session.addDataPoint( 1, 5000 );
				session.addDataPoint( 1, 5600 );
				session.addDataPoint( 1, 200 );
				session.addDataPoint( 1, 2800 );
				session.addDataPoint( 1, 18000 );
				session.addDataPoint( 1, 2500 );
				session.addDataPoint( 1, 15000 );
				session.addDataPoint( 1, 1400 );
				session.addDataPoint( 1, 1000 );
				session.addDataPoint( 1, 2000 );
				session.addDataPoint( 1, 7800 );
				session.addDataPoint( 1, 1100 );
				session.addDataPoint( 1, 18900 );
				session.addDataPoint( 1, 250 );
				session.addDataPoint( 1, 500 );
				session.addDataPoint( 1, 600 );
				session.addDataPoint( 1, 4000 );
				
			}
			else if( testCase == 7 ) {
				cout << "Test Case #07: Bad Data!" << endl;
				out << "Test Case #07: Bad Data!" << endl;
					
				session.clearData();
				session.addDataPoint( 1, 1000 );
				session.addDataPoint( 1, 7000 );
				session.addDataPoint( 1, 30000 );
				
			}
			session.setSampleCount( 1000 );
			
			session.runAnalysis();

			int intervals = 20;

			session.constructBinnedSpace(intervals,false);

			for (int k = 0 ; k < intervals ; k++) {
				cout << session.getBinnedBeta(k) << " ";
				out << session.getBinnedBeta(k) << " ";
			}
			cout << endl;
			out << endl;

			for (int i = 0 ; i < intervals ; i++) {
				cout << session.getBinnedAlpha(i) << " " ;
				out << session.getBinnedAlpha(i) << " " ;
				for (int j = 0 ; j < intervals ; j++) {
					cout << session.getBinnedDensity(i,j) << " ";
					out << session.getBinnedDensity(i,j) << " ";
				}
				cout << endl;
				out << endl;
			}

			int steps = 100;
			int marginalSize;
			double c, p, x;
			double low, high;
			double inc;

			cout << endl << "Alpha Marginal Distribution" << endl;
			out << endl << "Alpha Marginal Distribution" << endl;
			session.constructMarginal(0, steps, true);
			marginalSize = session.getMarginalSize();
			c = 0;
			for(i = 0; i < marginalSize; i++) {
				session.getMarginalPoint(i, x, p);
				c += p;
				cout << x << " " << c << endl;
				out << x <<  " " << c << endl;
			}

			
			cout << endl << "Beta Marginal Distribution" << endl;
			out << endl << "Beta Marginal Distribution" << endl;
			session.constructMarginal(1, steps, true);
			marginalSize = session.getMarginalSize();
			c = 0;
			for(i = 0; i < marginalSize; i++) {
				session.getMarginalPoint(i, x, p);
				c += p;
				cout << x << " " << c << endl;
				out << x <<  " " << c << endl;
			}


			cout << endl << "Population Variability" << endl;
			out << endl << "Population Variability" << endl;
			steps = 40;
			i = 0;
			low = session.getPopVarLowerBound();
			high = session.getPopVarUpperBound();
			inc = log( high / low ) / ( steps - 1 );
			x = low;
			do {
				p = session.getMeasureMean(dflt,x,pdf);
				c = session.getMeasureMean(dflt,x,cdf);

				cout << x << " " << p << " " << c << endl;
				out << x << " " << p << " " << c << endl;
				i++;
				x = low * exp( inc * i );
			}
			while ( i < steps );
		}

		Pass++;
	}
	while (Pass <= NumberOfPasses);

	out.close();
	//in.close();
*/
}


void NHSession::testGammaPoisson()
{
/*
	int NumberOfPasses = 1;
	int Pass = 1;
	int i;

	NHSession session;
	NHModels modelselect;

	modelselect = gampois;

	ofstream out( "NHTestGP.dat" );

	session.setModel(gampois);
//	ifstream in1( "c:\\Trash\\data.gp.txt" );
//	session.clearData();
//	double k, T, type;
//	for(i = 0; i < 21; i++) {
//		in1 >> k >> T >> type;
//		session.addDataPoint( k, T, type );
//	}
//	in1.close();

//	ifstream in2( "c:\\Trash\\data.ll.txt" );
//	for(i = 0; i < 20; i++) {
//		in2 >> k >> T >> type;
//		session.addDataPoint( k, T, type );
//	}
//	in2.close();


	cout << "NonHomogneous Gamma-Poisson:" << endl;
	out << "NonHomogneous Gamma-Poisson:" << endl;
	do {
		cout << "Pass " << Pass << " out of " << NumberOfPasses << endl << endl;	
		out << "Pass " << Pass << " out of " << NumberOfPasses << endl << endl;

		for( int testCase = 1; testCase <= 1; testCase++ ) {
			//session.setModel(gampois);

			/*
			if( testCase == 1 ) {
				cout << "Test Case #01: Alpha = 2  Beta = 5" << endl;
				out << "Test Case #01: Alpha = 2  Beta = 5" << endl;

				session.clearData();
				session.addDataPoint(0.1, 0.5, 5,100 );
				session.addDataPoint(0.1, 0.9, 5,100 );
				session.addDataPoint(0.05, 1, 5,100 );
				session.addDataPoint(0.2, .8, 15,100 );
				session.addDataPoint(0.3, 0.7, 15,100 );
				session.addDataPoint(0.25, 0.4, 15,100 );
				session.addDataPoint(0.6, 0.3, 15,100 );
				session.addDataPoint(0.5, 0.1, 15,100 );
				session.addDataPoint(0.9, 0.9, 15,100 );
				session.addDataPoint(0.4, 1.1, 25,100 );
				session.addDataPoint(0.35, 1.5, 25,100 );
				session.addDataPoint(0.25, 0.8, 25,100 );
				session.addDataPoint(0.75, 1.4, 25,100 );
				session.addDataPoint(0.6, 0.7, 25,100 );
				session.addDataPoint(0.25, 0.9, 25,100 );
				session.addDataPoint(1.1, 1, 35,100 );
				session.addDataPoint(1.4, 1.2, 35,100 );
				session.addDataPoint(0.01, 1.4, 35,100 );
				session.addDataPoint(0.4, 0.5, 35,100 );
				session.addDataPoint(0.1, 0.9, 35,100 );
				session.addDataPoint(0.9, 0.6, 45,100 );
				session.addDataPoint(1, 1, 45,100 );
				}
			else if( testCase == 2 ) {
				cout << "Test Case #02: Alpha = 0.5 Beta = 10" << endl;
				out << "Test Case #02: Alpha = 0.5 Beta = 10" << endl;

				session.clearData();
				session.addDataPoint( 0,1000 );
				session.addDataPoint( 0,1000 );
				session.addDataPoint( 1,1000 );
				session.addDataPoint( 2,1000 );
				session.addDataPoint( 3,1000 );
				session.addDataPoint( 5,1000 );
				session.addDataPoint( 7,1000 );
				session.addDataPoint( 10,1000 );
				session.addDataPoint( 14,1000 );
				session.addDataPoint( 18,1000 );
				session.addDataPoint( 23,1000 );
				session.addDataPoint( 29,1000 );
				session.addDataPoint( 35,1000 );
				session.addDataPoint( 44,1000 );
				session.addDataPoint( 54,1000 );
				session.addDataPoint( 66,1000 );
				session.addDataPoint( 82,1000 );
				session.addDataPoint( 105,1000 );
				session.addDataPoint( 135,1000 );
				session.addDataPoint( 192,1000 );
				session.addDataPoint( 332,1000 );
			}
			else if( testCase == 3 ) {
				cout << "Test Case #03: Alpha = 2.4 Beta = 0.9" << endl;
				out << "Test Case #03: Alpha = 2.4 Beta = 0.9" << endl;
				session.clearData();
				session.addDataPoint( 138,500 );
				session.addDataPoint( 293,500 );
				session.addDataPoint( 416,500 );
				session.addDataPoint( 518,500 );
				session.addDataPoint( 611,500 );
				session.addDataPoint( 700,500 );
				session.addDataPoint( 788,500 );
				session.addDataPoint( 876,500 );
				session.addDataPoint( 965,500 );
				session.addDataPoint( 1057,500 );
				session.addDataPoint( 1153,500 );
				session.addDataPoint( 1256,500 );
				session.addDataPoint( 1365,500 );
				session.addDataPoint( 1485,500 );
				session.addDataPoint( 1619,500 );
				session.addDataPoint( 1772,500 );
				session.addDataPoint( 1953,500 );
				session.addDataPoint( 2179,500 );
				session.addDataPoint( 2486,500 );
				session.addDataPoint( 2989,500 );
				session.addDataPoint( 4092,500 );
			}
			else if( testCase == 4 ) {
				cout << "Test Case #04: Alpha = 0.3 Beta = 4" << endl;
				out << "Test Case #04: Alpha = 0.3 Beta = 4" << endl;
					
				session.clearData();
				session.addDataPoint( 0,1000 );
				session.addDataPoint( 0,100 );
				session.addDataPoint( 0,10 );
				session.addDataPoint( 0,100 );
				session.addDataPoint( 2,1000 );
				session.addDataPoint( 0,100 );
				session.addDataPoint( 1,100 );
				session.addDataPoint( 0,10 );
				session.addDataPoint( 1,100 );
				session.addDataPoint( 0,10 );
				session.addDataPoint( 3,100 );
				session.addDataPoint( 4,100 );
				session.addDataPoint( 0,10 );
				session.addDataPoint( 6,100 );
				session.addDataPoint( 1,10 );
				session.addDataPoint( 12,100 );
				session.addDataPoint( 16,100 );
				session.addDataPoint( 2,10 );
				session.addDataPoint( 34,100 );
			}
			else if( testCase == 5 ) {
				cout << "Test Case #05: Alpha = 2.5 Beta = 7" << endl;
				out << "Test Case #05: Alpha = 2.5 Beta = 7" << endl;
					
				session.clearData();
				session.addDataPoint( 8,100 );
				session.addDataPoint( 12,100 );
				session.addDataPoint( 14,100 );
				session.addDataPoint( 17,100 );
				session.addDataPoint( 19,100 );
				session.addDataPoint( 21,100 );
				session.addDataPoint( 24,100 );
				session.addDataPoint( 26,100 );
				session.addDataPoint( 29,100 );
				session.addDataPoint( 31,100 );
				session.addDataPoint( 34,100 );
				session.addDataPoint( 37,100 );
				session.addDataPoint( 40,100 );
				session.addDataPoint( 43,100 );
				session.addDataPoint( 47,100 );
				session.addDataPoint( 52,100 );
				session.addDataPoint( 58,100 );
				session.addDataPoint( 66,100 );
				session.addDataPoint( 79,100 );
			}
			else if( testCase == 6 ) {
				cout << "Test Case #06: Frank's Data" << endl;
				out << "Test Case #06: Frank's Data" << endl;

				session.clearData();
				session.addDataPoint( 1, 1000 );
				session.addDataPoint( 1, 1500 );
				session.addDataPoint( 1, 10000 );
				session.addDataPoint( 1, 30000 );
				session.addDataPoint( 1, 250 );
				session.addDataPoint( 1, 600 );
				session.addDataPoint( 1, 350 );
				session.addDataPoint( 1, 20000 );
				session.addDataPoint( 1, 5000 );
				session.addDataPoint( 1, 5600 );
				session.addDataPoint( 1, 200 );
				session.addDataPoint( 1, 2800 );
				session.addDataPoint( 1, 18000 );
				session.addDataPoint( 1, 2500 );
				session.addDataPoint( 1, 15000 );
				session.addDataPoint( 1, 1400 );
				session.addDataPoint( 1, 1000 );
				session.addDataPoint( 1, 2000 );
				session.addDataPoint( 1, 7800 );
				session.addDataPoint( 1, 1100 );
				session.addDataPoint( 1, 18900 );
				session.addDataPoint( 1, 250 );
				session.addDataPoint( 1, 500 );
				session.addDataPoint( 1, 600 );
				session.addDataPoint( 1, 4000 );
			}
	
			session.setSampleCount( 3000 );
			
			session.runAnalysis();

			int intervals = 20;

			session.constructBinnedSpace(intervals,false);

			for (int k = 0 ; k < intervals ; k++) {
				cout << session.getBinnedBeta(k) << " ";
				out << session.getBinnedBeta(k) << " ";
			}
			cout << endl;
			out << endl;

			for (int i = 0 ; i < intervals ; i++) {
				cout << session.getBinnedAlpha(i) << " " ;
				out << session.getBinnedAlpha(i) << " " ;
				for (int j = 0 ; j < intervals ; j++) {
					cout << session.getBinnedDensity(i,j) << " ";
					out << session.getBinnedDensity(i,j) << " ";
				}
				cout << endl;
				out << endl;
			}

			int steps = 100;
			int marginalSize;
			double c, p, x;
			double low, high;
			double inc;

			cout << endl << "Alpha Marginal Distribution" << endl;
			out << endl << "Alpha Marginal Distribution" << endl;
			session.constructMarginal(0, steps, true);
			marginalSize = session.getMarginalSize();
			c = 0;
			for(i = 0; i < marginalSize; i++) {
				session.getMarginalPoint(i, x, p);
				c += p;
				cout << x << " " << c << endl;
				out << x <<  " " << c << endl;
			}

			
			cout << endl << "Beta Marginal Distribution" << endl;
			out << endl << "Beta Marginal Distribution" << endl;
			session.constructMarginal(1, steps, true);
			marginalSize = session.getMarginalSize();
			c = 0;
			for(i = 0; i < marginalSize; i++) {
				session.getMarginalPoint(i, x, p);
				c += p;
				cout << x << " " << c << endl;
				out << x <<  " " << c << endl;
			}


			cout << endl << "Population Variability" << endl;
			out << endl << "Population Variability" << endl;
			steps = 40;
			i = 0;
			low = session.getPopVarLowerBound();
			high = session.getPopVarUpperBound();
			inc = log( high / low ) / ( steps - 1 );
			x = low;
			do {
				p = session.getMeasureMean(dflt,x,pdf);
				c = session.getMeasureMean(dflt,x,cdf);

				cout << x << " " << p << " " << c << endl;
				out << x << " " << p << " " << c << endl;
				i++;
				x = low * exp( inc * i );
			}
			while ( i < steps );
		}

		Pass++;
	}
	while (Pass <= NumberOfPasses);

	out.close();
*/
}

void NHSession::testLogNormalLogNormal()
{
/*
	int NumberOfPasses = 1;
	int Pass = 1;
	int i;
	double k, n, nu, tau;

	NHSession session;
	NHModels modelselect;

	modelselect = loglog;
	session.setModel(loglog);
	
	ifstream in1( "e:\\trash\\nhlnb.csv" );
	session.clearData();
	//double k, T, type;
	for(i = 0; i < 100; i++) {
		in1 >> k >> n >> nu >> tau;
		session.addDataPoint( nu, tau, k, n );
	}
	in1.close();
	

//	ifstream in2( "c:\\Trash\\data.ll.txt" );
//	for(i = 0; i < 20; i++) {
//		in2 >> k >> T >> type;
//		session.addDataPoint( k, T, type );
//	}
//	in2.close();


	ofstream out( "NHTestLL.dat" );

	cout << "NonHomogneous LogNormal-LogNormal:" << endl;
	out << "NonHomogneous LogNormal-LogNormal:" << endl;
	do {
		cout << "Pass " << Pass << " out of " << NumberOfPasses << endl << endl;	
		out << "Pass " << Pass << " out of " << NumberOfPasses << endl << endl;

		for( int testCase = 1; testCase <= 1; testCase++ ) {

			

			/*if( testCase == 1 ) {
				cout << "Test Case #01: Mu = 1  Tau = 1" << endl;
				out << "Test Case #01: Mu = 1  Tau = 1" << endl;

				session.clearData();
				
				session.addDataPoint( 5e-5, 0.3 , 1, 1000);;
				session.addDataPoint( 7e-5, 2.0 , 1, 100);;
				session.addDataPoint( 4e-3, 1.6 , 1, 10000);;
				session.addDataPoint( 2e-5, 0.5 , 1, 10000);;
				session.addDataPoint( 3e-4, 1.7 , 1, 1000);;
				session.addDataPoint( 5e-4, 1.0 , 1, 100000);;
				session.addDataPoint( 2e-3, 1.2 , 1, 1000000);;
				session.addDataPoint( 1e-3, 1.6 , 1, 100);;
				session.addDataPoint( 4e-3, 0.9 , 1, 1000);;
				session.addDataPoint( 7e-4, 1.1 , 1, 10000);;
				session.addDataPoint( 9e-4, 0.4 , 1, 10000);;
				session.addDataPoint( 3e-3, 0.7 , 1, 100000);;
				

				
				/*
				session.addDataPoint( 0.1,0.1 );
				session.addDataPoint( 0.1,0.1 );
				session.addDataPoint( 0.1,0.1 );
				session.addDataPoint( 0.1,0.1 );
				session.addDataPoint( 0.1,0.1 );
				session.addDataPoint( 0.1,0.1 );
				session.addDataPoint( 0.1,0.1 );
				session.addDataPoint( 0.55,0.1 );
				session.addDataPoint( 0.55,0.1 );
				session.addDataPoint( 0.55,0.1 );
				session.addDataPoint( 0.55,0.1 );
				session.addDataPoint( 0.55,0.1 );
				session.addDataPoint( 0.55,0.1 );
				session.addDataPoint( 0.55,0.1 );
				session.addDataPoint( 0.55,0.1 );
				session.addDataPoint( 0.55,0.1 );
				session.addDataPoint( 0.55,0.1 );
				session.addDataPoint( 0.55,0.1 );
				session.addDataPoint( 0.55,0.1 );
				session.addDataPoint( 0.55,0.1 );
				session.addDataPoint( 0.55,0.1 );
				session.addDataPoint( 0.55,0.1 );
				session.addDataPoint( 1.0,0.1 );
				session.addDataPoint( 1.0,0.1 );
				session.addDataPoint( 1.0,0.1 );
				session.addDataPoint( 1.0,0.1 );
				session.addDataPoint( 1.0,0.1 );
				session.addDataPoint( 1.0,0.1 );
				session.addDataPoint( 1.0,0.1 );
				session.addDataPoint( 1.0,0.1 );
				session.addDataPoint( 1.0,0.1 );
				session.addDataPoint( 1.0,0.1 );
				session.addDataPoint( 1.52,0.1 );
				session.addDataPoint( 1.52,0.1 );
				session.addDataPoint( 1.52,0.1 );
				session.addDataPoint( 1.52,0.1 );
				session.addDataPoint( 1.52,0.1 );
				session.addDataPoint( 1.52,0.1 );
				session.addDataPoint( 1.97,0.1 );
				session.addDataPoint( 1.97,0.1 );
				session.addDataPoint( 1.97,0.1 );
				session.addDataPoint( 1.97,0.1 );
				session.addDataPoint( 2.32,0.1 );
				session.addDataPoint( 2.32,0.1 );
				session.addDataPoint( 2.32,0.1 );
				session.addDataPoint( 2.85,0.1 );
				session.addDataPoint( 2.85,0.1 );
				session.addDataPoint( 3.9,0.1 );
				
			}
			else if( testCase == 2 ) {
				cout << "Test Case #02: Mu = 0.1  Tau = 0.2" << endl;
				out << "Test Case #02: Mu = 0.1  Tau = 0.2" << endl;

				session.clearData();
				session.addDataPoint( 0.06,0.02 );
				session.addDataPoint( 0.07,0.02 );
				session.addDataPoint( 0.08,0.02 );
				session.addDataPoint( 0.08,0.02 );
				session.addDataPoint( 0.08,0.02 );
				session.addDataPoint( 0.09,0.02 );
				session.addDataPoint( 0.09,0.02 );
				session.addDataPoint( 0.09,0.02 );
				session.addDataPoint( 0.1,0.02 );
				session.addDataPoint( 0.1,0.02 );
				session.addDataPoint( 0.1,0.02 );
				session.addDataPoint( 0.1,0.02 );
				session.addDataPoint( 0.11,0.02 );
				session.addDataPoint( 0.11,0.02 );
				session.addDataPoint( 0.11,0.02 );
				session.addDataPoint( 0.11,0.02 );
				session.addDataPoint( 0.12,0.02 );
				session.addDataPoint( 0.12,0.02 );
				session.addDataPoint( 0.13,0.02 );
				session.addDataPoint( 0.14,0.02 );
				session.addDataPoint( 0.16,0.02 );
			}
			else if( testCase == 3 ) {
				cout << "Test Case #03: Mu = 0.01  Tau = 0.3" << endl;
				out << "Test Case #03: Mu = 0.01  Tau = 0.3" << endl;

				session.clearData();
				session.addDataPoint( 0.005,0.03 );
				session.addDataPoint( 0.006,0.03 );
				session.addDataPoint( 0.007,0.03 );
				session.addDataPoint( 0.007,0.03 );
				session.addDataPoint( 0.008,0.03 );
				session.addDataPoint( 0.008,0.03 );
				session.addDataPoint( 0.009,0.03 );
				session.addDataPoint( 0.009,0.03 );
				session.addDataPoint( 0.009,0.03 );
				session.addDataPoint( 0.01,0.03 );
				session.addDataPoint( 0.01,0.03 );
				session.addDataPoint( 0.01,0.03 );
				session.addDataPoint( 0.011,0.03 );
				session.addDataPoint( 0.011,0.03 );
				session.addDataPoint( 0.012,0.03 );
				session.addDataPoint( 0.012,0.03 );
				session.addDataPoint( 0.013,0.03 );
				session.addDataPoint( 0.014,0.03 );
				session.addDataPoint( 0.015,0.03 );
				session.addDataPoint( 0.016,0.03 );
				session.addDataPoint( 0.02,0.03 );
			}
			else if( testCase == 4 ) {
				cout << "Test Case #04: Mu = 1e-5  Tau = 0.1" << endl;
				out << "Test Case #04: Mu = 1e-5  Tau = 0.1" << endl;

				session.clearData();
				session.addDataPoint( 7.9e-6,0.01 );
				session.addDataPoint( 8.5e-6,0.01 );
				session.addDataPoint( 8.8e-6,0.01 );
				session.addDataPoint( 9.0e-6,0.01 );
				session.addDataPoint( 9.2e-6,0.01 );
				session.addDataPoint( 9.3e-6,0.01 );
				session.addDataPoint( 9.5e-6,0.01 );
				session.addDataPoint( 9.6e-6,0.01 );
				session.addDataPoint( 9.7e-6,0.01 );
				session.addDataPoint( 9.9e-6,0.01 );
				session.addDataPoint( 1e-5,0.01 );
				session.addDataPoint( 1e-5,0.01 );
				session.addDataPoint( 1e-5,0.01 );
				session.addDataPoint( 1e-5,0.01 );
				session.addDataPoint( 1.1e-5,0.01 );
				session.addDataPoint( 1.1e-5,0.01 );
				session.addDataPoint( 1.1e-5,0.01 );
				session.addDataPoint( 1.1e-5,0.01 );
				session.addDataPoint( 1.1e-5,0.01 );
				session.addDataPoint( 1.2e-5,0.01 );
				session.addDataPoint( 1.3e-5,0.01 );
			}
			else if( testCase == 5 ) {
				cout << "Test Case #05: Mu = 1e-7  Tau = 1.1" << endl;
				out << "Test Case #05: Mu = 1e-7  Tau = 1.1" << endl;

				session.clearData();
				session.addDataPoint( 7.7e-9,0.11 );
				session.addDataPoint( 1.6e-8,0.11 );
				session.addDataPoint( 2.4e-8,0.11 );
				session.addDataPoint( 3.2e-8,0.11 );
				session.addDataPoint( 4.0e-8,0.11 );
				session.addDataPoint( 4.8e-8,0.11 );
				session.addDataPoint( 5.6e-8,0.11 );
				session.addDataPoint( 6.5e-8,0.11 );
				session.addDataPoint( 7.6e-8,0.11 );
				session.addDataPoint( 8.7e-8,0.11 );
				session.addDataPoint( 1.0e-7,0.11 );
				session.addDataPoint( 1.1e-7,0.11 );
				session.addDataPoint( 1.3e-7,0.11 );
				session.addDataPoint( 1.5e-7,0.11 );
				session.addDataPoint( 1.8e-7,0.11 );
				session.addDataPoint( 2.1e-7,0.11 );
				session.addDataPoint( 2.5e-7,0.11 );
				session.addDataPoint( 3.1e-7,0.11 );
				session.addDataPoint( 4.1e-7,0.11 );
				session.addDataPoint( 6.1e-7,0.11 );
				session.addDataPoint( 1.3e-6,0.11 );
			}
			else if( testCase == 6 ) {
				cout << "Test Case #06: M-1" << endl;
				out << "Test Case #06: M-1" << endl;

				session.clearData();
				session.addDataPoint( 3.00E06, 1.2 );
				session.addDataPoint( 1.00E04, 1.5 );
				session.addDataPoint( 4.00E12, 1.2 );
				session.addDataPoint( 3.00E5, 1.5 );

				session.addDataPoint( 1.00E-08, 1.4 );
				session.addDataPoint( 3.00E-07, 1.4 );
				session.addDataPoint( 2.50E-07, 0.98 );
				session.addDataPoint( 3.50E-05, 0.98 );
				session.addDataPoint( 4.00E-05, 0.98 );
				session.addDataPoint( 3.00E-04, 1.4 );
				session.addDataPoint( 1.00E-03, 1.4 );
				session.addDataPoint( 2.00E-03, 0.98 );
				session.addDataPoint( 1.00E-02, 0.98 );
				session.addDataPoint( 1.00E-02, 2.07 );
			}
			
			session.setSampleCount( 1000 );
			
			session.runAnalysis();

			int intervals = 20;

			session.constructBinnedSpace(intervals,false);

			for (int k = 0 ; k < intervals ; k++) {
				cout << session.getBinnedBeta(k) << " ";
				out << session.getBinnedBeta(k) << " ";
			}
			cout << endl;
			out << endl;

			for (int i = 0 ; i < intervals ; i++) {
				cout << session.getBinnedAlpha(i) << " " ;
				out << session.getBinnedAlpha(i) << " " ;
				for (int j = 0 ; j < intervals ; j++) {
					cout << session.getBinnedDensity(i,j) << " ";
					out << session.getBinnedDensity(i,j) << " ";
				}
				cout << endl;
				out << endl;
			}

			int steps = 100;
			int marginalSize;
			double c, p, x;
			double low, high;
			double inc;

			cout << endl << "Alpha Marginal Distribution" << endl;
			out << endl << "Alpha Marginal Distribution" << endl;
			session.constructMarginal(0, steps, true);
			marginalSize = session.getMarginalSize();
			c = 0;
			for(i = 0; i < marginalSize; i++) {
				session.getMarginalPoint(i, x, p);
				c += p;
				cout << x << " " << c << endl;
				out << x <<  " " << c << endl;
			}

			
			cout << endl << "Beta Marginal Distribution" << endl;
			out << endl << "Beta Marginal Distribution" << endl;
			session.constructMarginal(1, steps, true);
			marginalSize = session.getMarginalSize();
			c = 0;
			for(i = 0; i < marginalSize; i++) {
				session.getMarginalPoint(i, x, p);
				c += p;
				cout << x << " " << c << endl;
				out << x <<  " " << c << endl;
			}


			cout << endl << "Population Variability" << endl;
			out << endl << "Population Variability" << endl;
			steps = 40;
			i = 0;
			low = session.getPopVarLowerBound();
			high = session.getPopVarUpperBound();
			inc = log( high / low ) / ( steps - 1 );
			x = low;
			do {
				p = session.getMeasureMean(dflt,x,pdf);
				c = session.getMeasureMean(dflt,x,cdf);

				cout << x << " " << p << " " << c << endl;
				out << x << " " << p << " " << c << endl;
				i++;
				x = low * exp( inc * i );
			}
			while ( i < steps );

			double p01 = session.findMeanPercentile(0.01);
			double p05 = session.findMeanPercentile(0.05);
			double p10 = session.findMeanPercentile(0.10);
			double p50 = session.findMeanPercentile(0.50);
			double p90 = session.findMeanPercentile(0.90);
			double p95 = session.findMeanPercentile(0.95);
			double p99 = session.findMeanPercentile(0.99);

			cout << "P01 " << p01 << endl;
			cout << "P05 " << p05 << endl;
			cout << "P10 " << p10 << endl;
			cout << "P50 " << p50 << endl;
			cout << "P90 " << p90 << endl;
			cout << "P95 " << p95 << endl;
			cout << "P99 " << p99 << endl;

		}

		Pass++;
	}
	while (Pass <= NumberOfPasses);

	out.close();
*/
}


void NHSession::testLogNormalPoisson()
{
/*
	int NumberOfPasses = 1;
	int Pass = 1;
	int i;

	NHSession session;
	NHModels modelselect;

	modelselect = logpois;

	ofstream out( "NHTestLP.dat" );

	session.setModel(logpois);
	
	
//	ifstream in1( "c:\\Trash\\data.lp.txt" );
//	session.clearData();
//	double k, T, type;
//	for(i = 0; i < 20; i++) {
//		in1 >> k >> T >> type;
//		session.addDataPoint( k, T, type );
//	}
//	in1.close();
	

//	ifstream in2( "c:\\Trash\\data.ll.txt" );
//	for(i = 0; i < 20; i++) {
//		in2 >> k >> T >> type;
//		session.addDataPoint( k, T, type );
//	}
//	in2.close();

	cout << "NonHomogneous LogNormal-Poisson:" << endl;
	out << "NonHomogneous LogNormal-Poisson:" << endl;
	do {
		cout << "Pass " << Pass << " out of " << NumberOfPasses << endl << endl;	
		out << "Pass " << Pass << " out of " << NumberOfPasses << endl << endl;

		for( int testCase = 1; testCase <= 1; testCase++ ) {
			//session.setModel(logpois);

			if( testCase == 1 ) {
				cout << "Test Case #01: Mu = 0.3  Tau = 1" << endl;
				out << "Test Case #01: Mu = 0.3  Tau = 1" << endl;

				session.clearData();
				
				session.addDataPoint( 0,0, 5e-5, 1.2 );
				session.addDataPoint( 0,0, 7e-5, 0.7 );
				session.addDataPoint( 0,0, 4e-3, 0.5 );
				session.addDataPoint( 0,0, 2e-5, 0.9 );
				session.addDataPoint( 0,0, 3e-4, 1.3 );
				session.addDataPoint( 0,0, 5e-4, 1.1 );
				session.addDataPoint( 0,0, 2e-3, 0.8 );
				session.addDataPoint( 0,0, 1e-3, 0.5 );
				

				
				session.addDataPoint(0.1, 0.5, 2,100 );
				session.addDataPoint(0.1, 0.2, 2,100 );
				session.addDataPoint(0.05, 1.3, 1,10 );
				session.addDataPoint(0.05, 1, 1,10 );
				session.addDataPoint(0.01, 1, 1,10 );
				session.addDataPoint(0.01, 1, 1,10 );
				session.addDataPoint(0.11, .8, 1,10 );
				session.addDataPoint(0.16, 0.7, 1,10 );
				session.addDataPoint(0.3, 0.6, 1,10 );
				session.addDataPoint(0.2, 0.4, 25,100 );
				session.addDataPoint(0.4, 0.5, 25,100 );
				session.addDataPoint(0.35, 1.1, 25,100 );
				session.addDataPoint(0.45, 1.2, 25,100 );
				session.addDataPoint(0.5, 1.2, 25,100 );
				session.addDataPoint(0.5, 1.3, 25,100 );
				session.addDataPoint(0.15, 1, 33,100 );
				session.addDataPoint(0.3, 1, 33,100 );
				session.addDataPoint(0.1, 0.9, 33,100 );
				session.addDataPoint(0.2, 0.8, 33,100 );
				session.addDataPoint(0.01, 0.8, 35,100 );
				session.addDataPoint(0.05, 0.9, 42,100 );
				
			}
			else if( testCase == 2 ) {
				cout << "Test Case #02: Mu = 0.3  Tau = 0.4" << endl;
				out << "Test Case #02: Mu = 0.3  Tau = 0.4" << endl;

				session.clearData();
				session.addDataPoint( 12,100 );
				session.addDataPoint( 16,100 );
				session.addDataPoint( 18,100 );
				session.addDataPoint( 20,100 );
				session.addDataPoint( 21,100 );
				session.addDataPoint( 23,100 );
				session.addDataPoint( 24,100 );
				session.addDataPoint( 26,100 );
				session.addDataPoint( 27,100 );
				session.addDataPoint( 29,100 );
				session.addDataPoint( 30,100 );
				session.addDataPoint( 32,100 );
				session.addDataPoint( 33,100 );
				session.addDataPoint( 35,100 );
				session.addDataPoint( 37,100 );
				session.addDataPoint( 39,100 );
				session.addDataPoint( 42,100 );
				session.addDataPoint( 45,100 );
				session.addDataPoint( 50,100 );
				session.addDataPoint( 58,100 );
				session.addDataPoint( 76,100 );
			}
			else if( testCase == 3 ) {
				cout << "Test Case #03: Mu = 0.01  Tau = 0.5" << endl;
				out << "Test Case #03: Mu = 0.01  Tau = 0.5" << endl;

				session.clearData();
				session.addDataPoint( 3,1000 );
				session.addDataPoint( 4,1000 );
				session.addDataPoint( 5,1000 );
				session.addDataPoint( 6,1000 );
				session.addDataPoint( 7,1000 );
				session.addDataPoint( 7,1000 );
				session.addDataPoint( 8,1000 );
				session.addDataPoint( 8,1000 );
				session.addDataPoint( 9,1000 );
				session.addDataPoint( 9,1000 );
				session.addDataPoint( 10,1000 );
				session.addDataPoint( 11,1000 );
				session.addDataPoint( 11,1000 );
				session.addDataPoint( 12,1000 );
				session.addDataPoint( 13,1000 );
				session.addDataPoint( 14,1000 );
				session.addDataPoint( 15,1000 );
				session.addDataPoint( 17,1000 );
				session.addDataPoint( 19,1000 );
				session.addDataPoint( 23,1000 );
				session.addDataPoint( 32,1000 );
				
			}
			else if( testCase == 4 ) {
				cout << "Test Case #04: Mu = 2.5e-3  Tau = 1.0" << endl;
				out << "Test Case #04: Mu = 2.5e-3  Tau = 1.0" << endl;

				session.clearData();
				session.addDataPoint( 0,2000 );
				session.addDataPoint( 1,2000 );
				session.addDataPoint( 1,2000 );
				session.addDataPoint( 2,2000 );
				session.addDataPoint( 2,2000 );
				session.addDataPoint( 3,2000 );
				session.addDataPoint( 3,2000 );
				session.addDataPoint( 3,2000 );
				session.addDataPoint( 4,2000 );
				session.addDataPoint( 4,2000 );
				session.addDataPoint( 5,2000 );
				session.addDataPoint( 6,2000 );
				session.addDataPoint( 6,2000 );
				session.addDataPoint( 7,2000 );
				session.addDataPoint( 8,2000 );
				session.addDataPoint( 10,2000 );
				session.addDataPoint( 12,2000 );
				session.addDataPoint( 14,2000 );
				session.addDataPoint( 18,2000 );
				session.addDataPoint( 26,2000 );
				session.addDataPoint( 51,2000 );
			}
			else if( testCase == 5 ) {
				cout << "Test Case #05: Mu = 1e-4  Tau = 0.9" << endl;
				out << "Test Case #05: Mu = 1e-4  Tau = 0.9" << endl;

				session.clearData();
				session.addDataPoint( 1,100000 );
				session.addDataPoint( 2,100000 );
				session.addDataPoint( 3,100000 );
				session.addDataPoint( 4,100000 );
				session.addDataPoint( 5,100000 );
				session.addDataPoint( 5,100000 );
				session.addDataPoint( 6,100000 );
				session.addDataPoint( 7,100000 );
				session.addDataPoint( 8,100000 );
				session.addDataPoint( 9,100000 );
				session.addDataPoint( 10,100000 );
				session.addDataPoint( 11,100000 );
				session.addDataPoint( 13,100000 );
				session.addDataPoint( 14,100000 );
				session.addDataPoint( 16,100000 );
				session.addDataPoint( 18,100000 );
				session.addDataPoint( 21,100000 );
				session.addDataPoint( 25,100000 );
				session.addDataPoint( 32,100000 );
				session.addDataPoint( 44,100000 );
				session.addDataPoint( 81,100000 );
			}
			else if( testCase == 6 ) {
				cout << "Test Case #06: Mu = 0.01  Tau = 0.1" << endl;
				out << "Test Case #06: Mu = 0.01  Tau = 0.1" << endl;

				session.clearData();
								
				session.addDataPoint( 0, 1207 );
				session.addDataPoint( 0, 356 );
				session.addDataPoint( 0, 38 );
				session.addDataPoint( 0, 4397 );
				session.addDataPoint( 0, 184 );
				session.addDataPoint( 0, 3375 );
				session.addDataPoint( 0, 324 );
				session.addDataPoint( 5, 22198 );
				session.addDataPoint( 0, 94 );
				session.addDataPoint( 1, 5265 );
				session.addDataPoint( 0, 735 );
				session.addDataPoint( 0, 3852 );
				session.addDataPoint( 0, 970 );
				session.addDataPoint( 0, 246 );
				session.addDataPoint( 1, 4007 );
				session.addDataPoint( 0, 246 );
				session.addDataPoint( 3, 4387 );
				session.addDataPoint( 2, 27324 );
				session.addDataPoint( 0, 7737 );
				session.addDataPoint( 0, 228 );
				session.addDataPoint( 0, 838 );
				
				
				/*session.addDataPoint( 1, 1000 );
				session.addDataPoint( 1, 1500 );
				session.addDataPoint( 1, 10000 );
				session.addDataPoint( 1, 30000 );
				session.addDataPoint( 1, 250 );
				session.addDataPoint( 1, 600 );
				session.addDataPoint( 1, 350 );
				session.addDataPoint( 1, 20000 );
				session.addDataPoint( 1, 5000 );
				session.addDataPoint( 1, 5600 );
				session.addDataPoint( 1, 200 );
				session.addDataPoint( 1, 2800 );
				session.addDataPoint( 1, 18000 );
				session.addDataPoint( 1, 2500 );
				session.addDataPoint( 1, 15000 );
				session.addDataPoint( 1, 1400 );
				session.addDataPoint( 1, 1000 );
				session.addDataPoint( 1, 2000 );
				session.addDataPoint( 1, 7800 );
				session.addDataPoint( 1, 1100 );
				session.addDataPoint( 1, 18900 );
				session.addDataPoint( 1, 250 );
				session.addDataPoint( 1, 500 );
				session.addDataPoint( 1, 600 );
				session.addDataPoint( 1, 4000 );*/

				/*
				session.addDataPoint( 50,10000 );
				session.addDataPoint( 100,10000 );
				session.addDataPoint( 100,10000 );
				session.addDataPoint( 200,10000 );
				session.addDataPoint( 350,10000 );
				session.addDataPoint( 300,10000 );
				session.addDataPoint( 450,10000 );
				session.addDataPoint( 350,10000 );
				session.addDataPoint( 100,10000 );
				session.addDataPoint( 20,10000 );
				session.addDataPoint( 500,10000 );
				session.addDataPoint( 300,10000 );
				session.addDataPoint( 1,10000 );
				session.addDataPoint( 0,10000 );
				session.addDataPoint( 1,10000 );
				session.addDataPoint( 0,10000 );
				session.addDataPoint( 2,10000 );
				session.addDataPoint( 1,10000 );
				session.addDataPoint( 1,10000 );
				session.addDataPoint( 1,10000 );
				session.addDataPoint( 1,10000 );
				session.addDataPoint( 0,10000 );
				session.addDataPoint( 1,10000 );
				session.addDataPoint( 1,10000 );
				session.addDataPoint( 0,10000 );
				
			}
			else if( testCase == 7 ) {
				cout << "Test Case #07: Mu = 1e-3  Tau = 1.5" << endl;
				out << "Test Case #07: Mu = 1e-3  Tau = 1.5" << endl;

				session.clearData();
								
				session.addDataPoint( 0, 1215,1 );
				session.addDataPoint( 6, 793, 1 );
				session.addDataPoint( 0, 1160, 1 );
				session.addDataPoint( 3, 1078, 1 );
				session.addDataPoint( 0, 1249, 1 );
				session.addDataPoint( 0, 550, 1 );
				session.addDataPoint( 0, 344, 1 );
				session.addDataPoint( 0, 1457, 1 );
				session.addDataPoint( 0, 881, 1 );
				session.addDataPoint( 1, 1411, 1 );
				session.addDataPoint( 3, 1779, 1 );
				session.addDataPoint( 0, 1687, 1 );
				session.addDataPoint( 3, 1780, 1 );
				session.addDataPoint( 1, 1300, 1 );
				session.addDataPoint( 0, 504, 1 );
				session.addDataPoint( 1, 872, 1 );
				session.addDataPoint( 2, 1374, 1 );
				session.addDataPoint( 0, 1046, 1 );
				session.addDataPoint( 6, 996, 1 );
				session.addDataPoint( 7, 1171, 1 );
				session.addDataPoint( 0, 424, 1 );
				
				session.addDataPoint( 0.000352146, 0.524343, 0 );
				session.addDataPoint( 0.00246547, 0.520236, 0 );
				session.addDataPoint( 0.000209316, 0.492206, 0 );
				session.addDataPoint( 0.000263181, 0.478043, 0 );
				session.addDataPoint( 0.000775111, 0.48693, 0 );
				session.addDataPoint( 0.00272869, 0.469343, 0 );
				session.addDataPoint( 0.00117872, 0.487787, 0 );
				session.addDataPoint( 0.00334772, 0.493834, 0 );
				session.addDataPoint( 0.0000525164, 0.538819, 0 );
				session.addDataPoint( 0.00433728, 0.48229, 0 );
				session.addDataPoint( 0.000149184, 0.511809, 0 );
				session.addDataPoint( 0.000251607, 0.477612, 0 );
				session.addDataPoint( 0.000745024, 0.516385, 0 );
				session.addDataPoint( 0.000531211, 0.494132, 0 );
				session.addDataPoint( 0.00102614, 0.489799, 0 );
				session.addDataPoint( 0.00038299, 0.537771, 0 );
				session.addDataPoint( 0.00041002, 0.531182, 0 );
				session.addDataPoint( 0.00150653, 0.484722, 0 );
				session.addDataPoint( 0.000415375, 0.449604, 0 );
				session.addDataPoint( 0.0012268, 0.534098, 0 );
				session.addDataPoint( 0.00041301, 0.495472, 0 );
				
			}
			
			session.setSampleCount( 1000 );
			
			int retval = session.runAnalysis();

			int intervals = 20;

			session.constructBinnedSpace(intervals,false);

			for (int k = 0 ; k < intervals ; k++) {
				cout << session.getBinnedBeta(k) << " ";
				out << session.getBinnedBeta(k) << " ";
			}
			cout << endl;
			out << endl;

			for (int i = 0 ; i < intervals ; i++) {
				cout << session.getBinnedAlpha(i) << " " ;
				out << session.getBinnedAlpha(i) << " " ;
				for (int j = 0 ; j < intervals ; j++) {
					cout << session.getBinnedDensity(i,j) << " ";
					out << session.getBinnedDensity(i,j) << " ";
				}
				cout << endl;
				out << endl;
			}

			int steps = 100;
			int marginalSize;
			double c, p, x;
			double low, high;
			double inc;

			cout << endl << "Alpha Marginal Distribution" << endl;
			out << endl << "Alpha Marginal Distribution" << endl;
			session.constructMarginal(0, steps, true);
			marginalSize = session.getMarginalSize();
			c = 0;
			for(i = 0; i < marginalSize; i++) {
				session.getMarginalPoint(i, x, p);
				c += p;
				cout << x << " " << c << endl;
				out << x <<  " " << c << endl;
			}

			
			cout << endl << "Beta Marginal Distribution" << endl;
			out << endl << "Beta Marginal Distribution" << endl;
			session.constructMarginal(1, steps, true);
			marginalSize = session.getMarginalSize();
			c = 0;
			for(i = 0; i < marginalSize; i++) {
				session.getMarginalPoint(i, x, p);
				c += p;
				cout << x << " " << c << endl;
				out << x <<  " " << c << endl;
			}


			cout << endl << "Population Variability" << endl;
			out << endl << "Population Variability" << endl;
			steps = 40;
			i = 0;
			low = session.getPopVarLowerBound();
			high = session.getPopVarUpperBound();
			inc = log( high / low ) / ( steps - 1 );
			x = low;
			do {
				p = session.getMeasureMean(dflt,x,pdf);
				c = session.getMeasureMean(dflt,x,cdf);

				cout << x << " " << p << " " << c << endl;
				out << x << " " << p << " " << c << endl;
				i++;
				x = low * exp( inc * i );
			}
			while ( i < steps );
		}

		Pass++;
	}
	while (Pass <= NumberOfPasses);

	out.close();
*/
}




