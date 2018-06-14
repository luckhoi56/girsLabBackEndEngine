// $Id: WeibullSampleSet.cpp 2184 2013-10-20 17:34:55Z Dongfeng Zhu $

#include "weibullsampleset.h"
#include <algorithm>

double QuantileOperator::MAX_FLOAT = 3e300;
double QuantileOperator::MIN_FLOAT = -3e300;

////////////////////////////////////////////////////////////////////////

/**
 * Updates the totals by adding values from the specified sample.
 * @param arr sample.
 */
void MeanSDevOperator::process(const float * arr) 
{
	double y = func->compute(arr);
	i0++;
	i1 += y;
	i2 += pow(y,2);
}

void QuantileOperator::clear(void)
{
	n = -10;
	sumx1 = 0;
	sumx2 = 0;

	maxx = MIN_FLOAT;
	minx = MAX_FLOAT;
	s01 = s05 = s10 = s50 = s90 = s95 = s99 = 0;
	f01 = f05 = f10 = f50 = f90 = f95 = f99 = 0;

}

double QuantileOperator::getQuantile(const int quantile) 
{
	switch (quantile) {
		case MIN : return minx;
		case MAX : return maxx;
		case Q01 : return s01;
		case Q05 : return s05;
		case Q10 : return s10;
		case Q50 : return s50;
		case Q90 : return s90;
		case Q95 : return s95;
		case Q99 : return s99;
	}
	return 0;
}

void QuantileOperator::process(const float * arr)
{
	double x = func->compute(arr);	
	n++;

	if (x < minx) minx = x;
	if (x > maxx) maxx = x;

	if (n <= 0) {
		sumx1 += x;
		sumx2 += pow(x,2);
		if (n == 0) {
			double mean = sumx1 / 10;
			double sdev = pow(sumx2/10 - pow(mean,2),0.5);
			s01 = s05 = s10 = s50 = s90 = s95 = s99 = mean;
			f0 = 1 / (3 * sdev);
		}
	} else {
		double wn = 1. / n;
		double cn = sqrt(wn);
		//////////////////////////////////////////////////////////
		f01 = (1 - wn) * f01 + wn * computeI(x,s01,cn) / (2 * cn);
		double e01 = max(f01,f0 * cn);
		s01 += (wn / e01) * (0.01 - computeZ(x,s01));
		//////////////////////////////////////////////////////////
		f05 = (1 - wn) * f05 + wn * computeI(x,s05,cn) / (2 * cn);
		double e05 = max(f05,f0 * cn);
		s05 += (wn / e05) * (0.05 - computeZ(x,s05));
		//////////////////////////////////////////////////////////		
		f10 = (1 - wn) * f10 + wn * computeI(x,s10,cn) / (2 * cn);
		double e10 = max(f10,f0 * cn);
		s10 += (wn / e10) * (0.10 - computeZ(x,s10));
		//////////////////////////////////////////////////////////
		f50 = (1 - wn) * f50 + wn * computeI(x,s50,cn) / (2 * cn);
		double e50 = max(f50,f0 * cn);
		s50 += (wn / e50) * (0.50 - computeZ(x,s50));
		//////////////////////////////////////////////////////////
		f90 = (1 - wn) * f90 + wn * computeI(x,s90,cn) / (2 * cn);
		double e90 = max(f90,f0 * cn);
		s90 += (wn / e90) * (0.90 - computeZ(x,s90));
		//////////////////////////////////////////////////////////
		f95 = (1 - wn) * f95 + wn * computeI(x,s95,cn) / (2 * cn);
		double e95 = max(f95,f0 * cn);
		s95 += (wn / e95) * (0.95 - computeZ(x,s95));
		//////////////////////////////////////////////////////////
		f99 = (1 - wn) * f99 + wn * computeI(x,s99,cn) / (2 * cn);
		double e99 = max(f99,f0 * cn);
		s99 += (wn / e99) * (0.99 - computeZ(x,s99));
	}
}

////////////////////////////////////////////////////////////////////////

void SortingQuantileOperator::process(const float * arr)
{
	double x = this->func->compute(arr);
	if (x < minx) minx = x;
	if (x > maxx) maxx = x;
	this->arr[pos] = x;
	pos = pos + 1;
	if (pos == this->ARR_SIZE) {
		processArray();
	}
}

void SortingQuantileOperator::processArray()
{
	if (pos < 100) return;
	sorter.sort(this->pos,this->arr);
	this->s01 += estimateQuantile(arr,pos,0.01) * pos;
	this->s05 += estimateQuantile(arr,pos,0.05) * pos;
	this->s10 += estimateQuantile(arr,pos,0.10) * pos;
	this->s50 += estimateQuantile(arr,pos,0.50) * pos;
	this->s90 += estimateQuantile(arr,pos,0.90) * pos;
	this->s95 += estimateQuantile(arr,pos,0.95) * pos;
	this->s99 += estimateQuantile(arr,pos,0.99) * pos;
	weight += pos;
	this->pos = 0;
}

/**
 * Returns the specified quantile.
 */
double SortingQuantileOperator::getQuantile(const int quantile) 
{
	processArray();

	if (weight == 0) return 0;
	switch (quantile) {
		case MIN : return minx;
		case MAX : return maxx;
		case Q01 : return s01 / weight;
		case Q05 : return s05 / weight;
		case Q10 : return s10 / weight;
		case Q50 : return s50 / weight;
		case Q90 : return s90 / weight;
		case Q95 : return s95 / weight;
		case Q99 : return s99 / weight;
	}
	return 0;
}

/**
 * Estimates the z quantile based on the array of sample values.
 * The array must be sorted.
 */
double SortingQuantileOperator::estimateQuantile(double * arr, int n, double z) 
{
	double p = (z * pos) - 0.5;
	int i0 = (int)floor(p); 
	int i1 = (int)ceil(p);
	double r = p - i0;
	double q = (1 - r) * arr[i0] + r * arr[i1];
	return q;
}

////////////////////////////////////////////////////////////////////////

ConfidenceOperator::ConfidenceOperator(OperatorFunction * f, double val) 
{
	clear();
	this->func = f;
	this->val = val;
}

void ConfidenceOperator::process(const float * sample)
{
	if (func->compute(sample) > val) {
		true_cnt++;
	} 
	sample_cnt++;
}

/**
 * Clears the operator.
 */
void ConfidenceOperator::clear() {
	true_cnt = 0;
	sample_cnt = 0;
}

/**
 * Constructor. 
 * @param f function specifying the variable
 * @param start index of first sample to be retrieved
 * @param length number of samples to be retrieved
 * @param arr array in which to store chain
 */
ChainOperator::ChainOperator(OperatorFunction *f, int start, int length, double * arr)
{
	this->func = f;
	this->start = start;
	this->length = length;
	this->arr = arr;
	clear();
}

/**
 * Processes the current sample.
 * If the sample falls within the specified interval, the 
 * function is computed and the result stored in the chain.
 * @param sample current sample.
 */
void ChainOperator::process(const float * sample)
{
	if (pos >= start && pos < start + length) {
		arr[pos - start] = func->compute(sample);
	}
	pos++;
}

/**
 * Clears the operator.
 */
void ChainOperator::clear() {
	for (int i = 0 ; i < length ; i++) arr[i] = 0;
	this->pos = 0;
}

////////////////////////////////////////////////////////////////////////

/**
 * Writes the sample to an output stream.
 */
void OutputStreamOperator::process(const float * arr) {
	retval = os->writeFloatArray(arr,size);
}


////////////////////////////////////////////////////////////////////////

/**
 * Constructor.
 * @param lna index of ln(a) in parameter array.
 * @param lne index of ln(e) in parameter array.
 * @param b index of b in parameter array.
 */
LnZFunction::LnZFunction(int lna, int lne, int b) 
{
	this->lna = lna; 
	this->b = b; 
	this->lne = lne;
}

LnYFunction::LnYFunction(int lna, int lne, int b) 
{
	this->lna = lna; 
	this->b = b; 
	this->lne = lne;
}


////////////////////////////////////////////////////////////////////////

/**
 * Constructor.
 * This constructor creates a block of specified dimensions, and attaches
 * the specified block in the single linked list.
 * @param size number of values in each sample
 * @param length number of samples in the block
 * @param next block to be linked to this.
 */
SampleBlock::SampleBlock(int size, int length ,SampleBlock * next)
{
	this->data = new float[size * length];
	this->length = length;
	this->size = size;
	this->next = next;
	this->next_arr = 0;
}

/**
 * Destructor.
 * This destructor destroys the linked blocks through 
 * an iterative procedure.
 */
SampleBlock::~SampleBlock(void)
{
	delete [] data;
	SampleBlock * block = next;
	// iteratively destruct all instances in the list.
	while (block != 0) {
		SampleBlock * nxtblck = block->next;
		block->next = 0;
		delete block;
		block = nxtblck;
	}
}

/**
 * Returns the next empty array.
 * This method is used to obtain references to the sample arrays one at a time.
 * Each time the method is called, a new array is returned.
 */
float * SampleBlock::getNextArray()
{
	float * arr;
	if (next_arr < length) {
		arr = &data[next_arr * size];
		next_arr++;
	} else {
		arr = 0;
	}
	return arr;
}

/**
 * Executes the operation for every array in the block.
 * Operation is performed in no particular order.
 */
void SampleBlock::execute(SampleOperator * op, DeviateGenerator * generator) const
{
	if (!op->randomOrder() || generator == 0) {
		for (int i = 0 ; i < next_arr ; i++) 
			op->process(&data[i * size]);
	} else {
		int * order = new int[next_arr];
		for (int i = 0 ; i < next_arr ; i++) order[i] = i;
		for (int j = 0 ; j < next_arr ; j++) {
			// uniformly select any of the remaining samples
			int r = generator->discrete(j,next_arr - 1);
			int k = order[r];
			order[r] = order[j];
			// process the selected sample
			op->process(&data[k * size]);
		}
		delete [] order;
	}
}

/**
 * Returns true if the block contains no data.
 */
bool SampleBlock::isEmpty(void) const
{
	return next_arr == 0;
}

////////////////////////////////////////////////////////////////////////

/**
 * Constructor.
 * @param size number of values in each sample.
 * @param increment number of samples that are allocated at a time.
 */
WeibullSampleSet::WeibullSampleSet(int size, int increment)
{
	this->size = size;
	this->increment = increment;
	this->block = new SampleBlock(size,increment,0);
}

/**
 * Destructor.
 * This destructor destroys all sample blocks.
 */
WeibullSampleSet::~WeibullSampleSet(void)
{
	delete block;
}

/**
 * Returns the number of samples.
 */
int WeibullSampleSet::getSampleCount(void) const
{
	int cnt = 0;
	SampleBlock * block = this->block;
	while (block != 0) {
		cnt += block->getArrayCount();
		block = block->getNextBlock();
	}
	return cnt;
}

/**
 * Returns true if the sample set is empty.
 */
bool WeibullSampleSet::isEmpty(void) const
{
	return block == 0 || block->isEmpty();
}

/**
 * Reads the samples from the input stream.
 * Returns 0 if operation was successful.
 */
int WeibullSampleSet::read(InputStream &is, int sample_cnt)
{
	int retval = 0;
	for (int i = 0 ; i < sample_cnt && retval == 0; i++) {
		float * buffer = getNextArray();
		if (buffer != 0) {
			retval = is.readFloatArray(buffer,this->size);
		} else {
			// failed to allocate memory
			retval = -1;
		}
	}
	return retval;
}

/**
 * Returns a new uninitialized sample array.
 * If the current block is full, a new block is allocated, and 
 * a sample array from the new block is returned.
 */
float * WeibullSampleSet::getNextArray()
{
	float * arr = block->getNextArray();
	if (arr == 0)
	{
		block = new SampleBlock(size,increment,block);
		arr = block->getNextArray();
	}
	return arr;
}

/**
 * Copies the current state of params into the sample set.
 * The values are copied to the next empty sample array in the 
 * sample set. The sample set is automatically expanded when the 
 * set is full.
 */
void WeibullSampleSet::fetch(const ParamArray * params)
{
	float * arr = this->getNextArray();
	params->getParams(arr,size);
}

/**
 * Iteratively performs the operation for all arrays in the sample set.
 * Depending on the type of operator, a random generator may be used to 
 * process the samples in a random order.
 * @param op operator to be executed
 * @param dg random deviate generator
 */
void WeibullSampleSet::execute(SampleOperator * op, DeviateGenerator * dg) const
{
	SampleBlock * block = this->block;
	while (block != 0) {
		block->execute(op,dg);
		block = block->getNextBlock();
	}
}