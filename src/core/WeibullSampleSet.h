// $Id: SampleSet.h 2137 2013-08-01 03:33:59Z Dongfeng Zhu $
#ifndef __WEIBULLSAMPLESET_H__
#define __WEIBULLSAMPLESET_H__

#pragma once

#include "ParamArray.h"
#include <math.h>
#include "DeviateGenerator.h"
#include "QuickSorter.h"
#include "BRASSStreams.h"

/**
 * Superclass for classes that operate on sample sets.
 * The class can be used to derive operators that must be 
 * applied to the set of samples, after the set has been 
 * completed. The WeibullSampleSet class function provides functions
 * that allow SampleOperator objects to be applied to all 
 * samples.
 * @see WeibullSampleSet#execute(SampleOperator *)
 */
class SampleOperator
{
public:
	SampleOperator() {}
	virtual ~SampleOperator() {}
	/// Process the vector representing the sample.
	virtual void process(const float * sample) = 0;
	/// Returns true if samples should be processed in random order.
	virtual bool randomOrder() const = 0;
};

/**
 * Superclass for operator fucntions.
 * Operator functions are used by the MeanSDev class and define the function
 * over which means and standard deviations are computed.
 */
class OperatorFunction
{
public:
	virtual ~OperatorFunction() {}
	/// Computes the function 
	virtual double compute(const float * arr) const = 0;
	virtual OperatorFunction * clone(void) const = 0;
};

/**
 * Function that returns value with specified index.
 * The compute method in this operator function simply returns the
 * value with the specified index in the array.
 */
class IndexFunction : public OperatorFunction
{
public:
	/// @see IndexFunction
	IndexFunction(int index) {this->index = index;}
	~IndexFunction() {}
	/// Computes the value of this function.
	double compute(const float * arr) const {return arr[index];}
	OperatorFunction * clone(void) const {return new IndexFunction(index);}
private:
	int index;
};

/**
 * Function that returns natural log of value with specified index.
 */
class LnIndexFunction : public OperatorFunction
{
public:
	/// @see LnIndexFunction
	LnIndexFunction(int index) {this->index = index;}
	~LnIndexFunction() {}
	/// Computes the value of this function.
	double compute(const float * arr) const {return log(arr[index]);}
	OperatorFunction * clone(void) const {return new LnIndexFunction(index);}
private:
	int index;
};

/**
 * Function that returns natural log of value with specified index.
 */
class Log10IndexFunction : public OperatorFunction
{
public:
	/// @see LnIndexFunction
	Log10IndexFunction(int index) {this->index = index;}
	~Log10IndexFunction() {}
	/// Computes the value of this function.
	double compute(const float * arr) const {return log(arr[index]) / log(10.0);}
	OperatorFunction * clone(void) const {return new Log10IndexFunction(index);}
private:
	int index;
};

/**
 * Wrapper function that returns the exponential of the wrapped function.
 */
class ExpWrapFunction : public OperatorFunction
{
public:
	ExpWrapFunction(OperatorFunction * f) {this->func = f;}
	~ExpWrapFunction() {delete func;}
	/// Computes the value of this function.
	double compute(const float * arr) const {return exp(func->compute(arr));}
	OperatorFunction * clone(void) const {return new ExpWrapFunction(func->clone());}
private:
	OperatorFunction * func;
};


/**
 * Wrapper function that returns the exponential of the wrapped function.
 */
class ReciprokeWrapFunction : public OperatorFunction
{
public:
	ReciprokeWrapFunction(OperatorFunction * f) {this->func = f;}
	~ReciprokeWrapFunction() {delete func;}
	/// Computes the value of this function.
	double compute(const float * arr) const {return 1 / (func->compute(arr));}
	OperatorFunction * clone(void) const {return new ReciprokeWrapFunction(func->clone());}
private:
	OperatorFunction * func;
};


/**
 * Wrapper function that returns the 10 log of the wrapped function.
 */
class Log10WrapFunction : public OperatorFunction
{
public:
	Log10WrapFunction(OperatorFunction * f) {this->func = f;}
	~Log10WrapFunction() {delete func;}
	/// Computes the value of this function.
	double compute(const float * arr) const {return log(func->compute(arr)) / log(10.0);}
	OperatorFunction * clone(void) const {return new Log10WrapFunction(func->clone());}
private:
	OperatorFunction * func;
};

/**
 * Function that returns exponential of value with specified index.
 */
class ExpIndexFunction : public OperatorFunction
{
public:
	/// @see LnIndexFunction
	ExpIndexFunction(int index) {this->index = index;}
	~ExpIndexFunction() {}
	/// Computes the value of this function.
	double compute(const float * arr) const {return exp(arr[index]);}
	OperatorFunction * clone(void) const {return new ExpIndexFunction(index);}
private:
	int index;
};

/**
 * Function that returns the ratio of two other functions.
 */
class RatioFunction : public OperatorFunction
{
public: 
	RatioFunction(OperatorFunction * nominator, OperatorFunction * denominator)
	{fnom = nominator; fden = denominator;}
	~RatioFunction() {delete fnom; delete fden;}
	double compute(const float * arr) const 
	{return fnom->compute(arr) / fden->compute(arr);}
	OperatorFunction * clone(void) const 
	{return new RatioFunction(fnom->clone(),fden->clone());}
private:
	OperatorFunction * fden;
	OperatorFunction * fnom;
};

/**
 * Function that computes the value of ln(z), given lna, lne, and b.
 * The function is used to compute the proportional hazard rate factor z
 * that is realized in a given parameter array, and is used when computing
 * the mean and standard deviation of ln(z) using the MeanSDevOperator.
 * The index of the required parameters are specified when the function is 
 * constructed.
 * @see MeanSDevOperator
 * @see QuantileOperator
 * @see LnYFunction
 */
class LnZFunction : public OperatorFunction
{
public:
	LnZFunction(int lna, int lne, int b);
	~LnZFunction(void) {}
	/// Computes the value of ln(z).
	double compute(const float * arr) const { return arr[b] * (arr[lna] - arr[lne]); }
	OperatorFunction * clone(void) const {return new LnZFunction(lna,lne,b);}
private:
	int lna;
	int b;
	int lne;
};

/**
 * Fucntion that computes the value of ln(y).
 * y is defined as the time reduction factor. The function is for use 
 * by the MeanSDevOperator.
 * @see LnZFunction
 * @see MeanSDevOperator
 * @see QuantileOperator
 */
class LnYFunction : public OperatorFunction
{
public:
	LnYFunction(int lna, int lne, int b);
	~LnYFunction(void) {}
	/// Computes the value of ln(z).
	double compute(const float * arr) const { return (arr[lna] - arr[lne]); }
	OperatorFunction * clone(void) const {return new LnYFunction(lna,lne,b);}
private:
	int lna;
	int b;
	int lne;
};

/**
 * Sample operator that computes means and standard deviations.
 * @see WeibullSampleSet#execute(SampleOperator *)
 */
class MeanSDevOperator : public SampleOperator
{
public:
	MeanSDevOperator(const int index) {func = new IndexFunction(index);i1=i2=i0=0;}
	MeanSDevOperator(OperatorFunction * f) {func = f;i1=i2=i0=0;}
	~MeanSDevOperator() {delete func;}
	/// Processes the sample.
	void process(const float * arr);
	/// Clears all summations.
	void clear() {i0 = 0; i1 = 0; i2 = 0;}
	/// Returns the mean of processed samples.
	double getMean() const {return i1 / i0;}
	/// Returns the standard deviation of processed samples.
	double getSDev() const {
		double s2 = i2 / i0 - pow(getMean(),2);
		return (s2 > 0) ? pow(s2,0.5) : 0;
	}
	bool randomOrder() const {return false;}
private:
	OperatorFunction * func;
	int i0;		// number of arrays processed.
	double i1;  // sum of values
	double i2;  // sum of squares
};

/**
 * Operator that retrieves (part of) a sample chain.
 */
class ChainOperator : public SampleOperator
{
public:
	ChainOperator(OperatorFunction *f, int start, int length, double * arr);
	~ChainOperator() {delete func;}
	void process(const float * arr);
	void clear();
	bool randomOrder() const {return false;}
private:
	OperatorFunction * func;
	int pos;
	int start;
	int length;
	double * arr;
};


/**
 * Operator that determines confidence in measure exceeding a value.
 */
class ConfidenceOperator : public SampleOperator
{
public:
	ConfidenceOperator(OperatorFunction *f, double val);
	~ConfidenceOperator() {delete func;}
	void process(const float * arr);
	void clear();
	bool randomOrder() const {return false;}
	double getConfidence() const {return ((double)true_cnt)/sample_cnt;}
private:
	OperatorFunction * func;
	double val;
	int sample_cnt;
	int true_cnt;
};


/**
 * Quantile estimation operator.
 * The operator processes the samples according to the 
 * specified function in order to determine the quantiles
 * of the distribution returned by the function of uncertain
 * variables.
 * The operator evaluates the function for every sample,
 * and then uses a modified Tierney recursive quantile estimator
 * to obtain quantile estimates.
 */
class QuantileOperator : public SampleOperator
{
public: 
	QuantileOperator(OperatorFunction * f) {func = f;clear();}
	~QuantileOperator() {delete func;}
	/// Processes the sample.
	void process(const float * arr);
	/// Clears the estimator content.
	void clear();
	/// Returns true to indicate the operator requires a random order.
	bool randomOrder() const {return true;}
	virtual double getQuantile(const int quantile); 

	const static int Q01 = 0; ///< 0.01 quantile
	const static int Q05 = 1; ///< 0.05 quantile
	const static int Q10 = 2; ///< 0.10 quantile
	const static int Q50 = 3; ///< 0.50 quantile (median)
	const static int Q90 = 4; ///< 0.90 quantile
	const static int Q95 = 5; ///< 0.95 quantile
	const static int Q99 = 6; ///< 0.99 quantile
	const static int MIN = -1; ///< minimum value
	const static int MAX = -2; ///< maximum value

	static double MAX_FLOAT; ///< (positive infinity)
	static double MIN_FLOAT; ///< (negative infinity)

protected:

	int computeZ(const double x, const double y) 
		{return x > y ? 0 : 1;}
	int computeI(const double x, const double y, const double z)
		{return fabs(x - y) > z ? 0 : 1;}

	OperatorFunction * func;
	double sumx1;
	double sumx2;
	double f0;
	int n;
	double minx;
	double maxx;

	double s01;
	double s05;
	double s10;
	double s50;
	double s90;
	double s95;
	double s99;
	double f01;
	double f05;
	double f10;
	double f50;
	double f90;
	double f95;
	double f99;
};

/**
 * Quantile estimator that estimates based on sorted arrays.
 *
 * This estimator is bothered less by strongly skewed distributions.
 */
class SortingQuantileOperator : public QuantileOperator
{
public:
	SortingQuantileOperator(OperatorFunction *f) : QuantileOperator(f) {clear();}
	~SortingQuantileOperator() {}
	void clear() {weight = 0; pos = 0;}
	void process(const float * arr);
	double getQuantile(const int quantile) ;
	bool randomOrder() const {return false;}
protected:
	static const int ARR_SIZE = 5000;
	void processArray();
	double estimateQuantile(double * arr, int n, double z); 

	double weight;
	int pos;
	double arr[ARR_SIZE];
	QuickSorter sorter;
};


/**
 * Block of double arrays.
 * This class is used by the WeibullSampleSet class in order to 
 * allocate memory in blocks rather than on an individual
 * basis. Blocks are stored in singly linked lists by the 
 * WeibullSampleSet class.
 */
class SampleBlock
{
public:
	SampleBlock(int size, int length ,SampleBlock * next);
	~SampleBlock(void);
	/// Returns number of arrays stored in the block.
	int getArrayCount() const {return next_arr;}
	float * getNextArray();
	void execute(SampleOperator * op, DeviateGenerator * generator = 0) const;
	/// Returns the block linked to this block.
	SampleBlock * getNextBlock() {return next;}
	bool isEmpty(void) const;
private:
	int size;	// size of the vectors
	int length; // number of vectors
	int next_arr; 
	float * data;
	SampleBlock * next;
};

/**
 * Set of sample values.
 *
 * This class can be used to store sample values, which are read from
 * instances of the ParamArray class. The class provides facilities to 
 * perform operations on the samples through the execute() method.
 * 
 * Samples are allocated in blocks at a time. The size of the blocks 
 * can be indicated.
 */
class WeibullSampleSet
{
public:
	WeibullSampleSet(int size, int increment);
	~WeibullSampleSet(void);
	void fetch(const ParamArray * params);
	void fetch(const ParamArray & params) {fetch(&params);}
	int getSampleCount() const;
	/// Executes the operator for all samples in the set.
	void execute(SampleOperator & op, DeviateGenerator * generator = 0) const
		{execute(&op,generator);}
	void execute(SampleOperator * op, DeviateGenerator * generator = 0) const;
	bool isEmpty(void) const;
	int getSize() { return size; }
	int read(InputStream &is, int sample_cnt);
private:
	float * getNextArray();
	int size;			// length of the parameter arrays
	int increment;		// number of samples in a block
	SampleBlock * block; // reference to first sample block
};



/**
 * Operator that writes samples to a OutputStream.
 *
 * This operator is part of a set of classes intended to 
 * support storing the state of the engine (sample results)
 * to an output stream. The stream can then transparently 
 * be coupled to a file, or any other storage medium.
 *
 * The 'processing' performed by this operator therefore
 * consists of the writing of sample values to the output
 * stream.
 */
class OutputStreamOperator : public SampleOperator
{
public:
	OutputStreamOperator(int size, OutputStream &os) 
		{this->size = size; this->os = &os;}
	~OutputStreamOperator() {}
	void process(const float * arr);
	bool randomOrder() const {return false;}
	int getReturnCode() const {return retval;}
private:
	int size;
	OutputStream * os;
	int retval;
};

#endif