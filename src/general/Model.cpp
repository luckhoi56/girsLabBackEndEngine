// Model.cpp: implementation of the Model class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Model.h"
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Model::Model()
{

}

Model::~Model()
{

}

bool Model::constructMeasure(int measure, double *parameters, SampleSet & samples, Distribution & distribution)
{
	return constructMeasure(measure, parameters, samples, distribution,0L);
}

bool Model::constructMeasure(int measure, double *parameters, SampleSet & samples, Distribution &distribution, StatusPtr status)
{
	div_t  progress;
	
	if (status != 0L) status->setStatus(0,"Generating distribution...");

	if (samples.size() == 0 || !isValidMeasure(measure)) {

		if (status != 0L) status->setStatus(100,"Error occurred...");
		distribution.clear();
		return false;

	} else {

		int count = samples.size();
		distribution.clear(count);

		double probability;
		double value = 0;

		samples.first();

		for (int index = 0; index < count; index++) {

			value = computeMeasure(measure, parameters,samples.getParameters());
			probability = samples.getProbability();
			distribution.add(value,probability);

			samples.next();

			if (status != 0L) {
				progress = div(index , count / 4);
				if (progress.rem == 0) {
					status->setStatus(progress.quot * 25);
				}
			}			
		}

		if (status != 0L) status->setStatus(100,"Done...");
		return true;
	}
}

bool Model::constructPosterior(SampleSet &samples)
{
	return constructPosterior(samples,0L);
}

bool Model::constructPosterior(SampleSet &samples, StatusPtr status)
{
	return false;
}
