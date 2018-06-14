#pragma once
#include "WeibullSampleSet.h"

class ConvergenceOperator :
	public SampleOperator
{
public:
	ConvergenceOperator(const int var_cnt, const int sample_cnt);
	~ConvergenceOperator(void);
	void process(const float * sample);
	/// Returns true if samples should be processed in random order.
	bool randomOrder() const {return false;}

	float computePotential(int var);
	float computeW(int var);
	float computeB(int var);
private :
	int var_cnt;
	int sample_cnt;
	int sample;
	float *i1;
	float *i2;
};
