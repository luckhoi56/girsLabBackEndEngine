#include "ConvergenceOperator.h"

/**
 * Constructor.
 * @param var_cnt size of the variable vector
 * @param sample_cnt number of samples
 */
ConvergenceOperator::ConvergenceOperator(const int var_cnt, const int sample_cnt)
{
	this->var_cnt = var_cnt;
	i1 = new float[2 * var_cnt];
	i2 = new float[2 * var_cnt];

	for (int i = 0 ; i < 2 * var_cnt ; i++) 
	{
		i1[i] = 0;
		i2[i] = 0;
	}

	sample = 0;
	this->sample_cnt = sample_cnt;
}

ConvergenceOperator::~ConvergenceOperator(void)
{
	delete [] i1;
	delete [] i2;
}

void ConvergenceOperator::process(const float * sample)
{
	int seq = (this->sample + sample_cnt / 2) / sample_cnt;

	for (int var = 0 ; var < var_cnt ; var++) 
	{
		i1[seq * var_cnt + var] += sample[var];
		i2[seq * var_cnt + var] += (float)pow(sample[var],2);
	}

	this->sample++;
}

/**
 * Returns the between sequence variance.
 * Based on section 8.4 in Gilks et al. 'MCMC in practice' (1996).
 * Returns 0 if a failure is encountered.
 */
float ConvergenceOperator::computeB(int var) 
{
	if (sample != sample_cnt) return 0;
	int seq_len = sample_cnt / 2;
	float psi_bar_1 = i1[var] / seq_len;
	float psi_bar_2 = i1[var + var_cnt] / seq_len;
	float psi_bar = (psi_bar_1 + psi_bar_2) / 2;
	float b = sample_cnt * (float)(pow(psi_bar_1 - psi_bar,2) + pow(psi_bar_2 - psi_bar,2));
	return b;
}

/**
 * Returns the within sequence variance.
 * Based on section 8.4 in Gilks et al. 'MCMC in practice' (1996).
 * Returns 0 if a failure is encountered.
 */
float ConvergenceOperator::computeW(int var) 
{
	if (sample != sample_cnt) return 0;
	int seq_len = sample_cnt / 2;
	float s2_1 = (i2[var] - (float)pow(i1[var],2) / seq_len) / (seq_len - 1);
	float s2_2 = (i2[var + var_cnt] - (float)pow(i1[var + var_cnt],2) / seq_len) / (seq_len - 1);
	float w = (s2_1 + s2_2) / 2;
	return w;
}

/**
 * Returns the estimated variance reduction potential.
 * Based on section 8.4 in Gilks et al. 'MCMC in practice' (1996).
 */
float ConvergenceOperator::computePotential(int var)
{
	if (sample != sample_cnt || computeW(var) == 0) return 0;
	int seq_len = sample_cnt / 2;
	float w = computeW(var);
	float b = computeB(var);

	// conservative estimate of variance
	float variance = (seq_len - 1) * w / seq_len + b / seq_len;
	float p_est = (float)sqrt(variance / w);
	return p_est;
}
