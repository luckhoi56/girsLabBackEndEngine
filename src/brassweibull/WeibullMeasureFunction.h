// $Id: WeibullMeasureFunction.h 1089 2005-08-08 16:31:42Z fgroen $

#pragma once

#include <math.h>

#include "../core/WeibullSampleSet.h"
#include "WeibullModelLayout.h"

/**
 * Base class for Weibull reliability measure functions.
 * This function provides the general Weibull reliability functions.
 *
 * This class and derived classes depend on a particular layout of
 * Weibull scale and shape parameters. See implementations of 
 * getShapeIndex() and getScaleIndex for details.
 */
class WeibullMeasureFunction : public OperatorFunction
{
public:
	WeibullMeasureFunction(void);
	virtual ~WeibullMeasureFunction(void);
	virtual double compute(const float * arr, const double time) const = 0;
protected:
	double getReliability(const double alpha, const double beta, const double time) const
		{return exp(-pow(time/alpha,beta));}
	double getCumulative(const double alpha, const double beta, const double time) const
		{return pow(time/alpha,beta);}
	double getIntensity(const double alpha, const double beta, const double time) const
		{return beta * pow(time / alpha,beta - 1) / alpha;}

	/**
	 * Returns the index of the shape parameter for give mode and node.
	 * Failure modes start counting at 1. Nodes start counting at 0.
	 */
	int getShapeIndex(const int mode,const int node_cnt) const 
	{return WeibullModelLayout::getShapeIndex(mode,node_cnt);}

	/**
	 * Returns the index of the scale parameter for give mode and node.
	 * Failure modes start counting at 1. Nodes start counting at 0.
	 */
	int getScaleIndex(const int node, const int mode, const int node_cnt) const 
	{return WeibullModelLayout::getScaleIndex(node,mode,node_cnt);}

};

/**
 * This function computes the reliability for a single failure mode and node.
 */
class WeibullReliabilityFunction : public WeibullMeasureFunction
{
public:
	WeibullReliabilityFunction(int alpha, int beta, double time)
		{a = alpha; b = beta; t = time;}
	~WeibullReliabilityFunction() {}
	double compute(const float * arr) const {return getReliability(exp(arr[a]),arr[b],t);}
	double compute(const float * arr, const double t) const {return getReliability(exp(arr[a]),arr[b],t);}
	OperatorFunction * clone(void) const {return new WeibullReliabilityFunction(a,b,t);}
private:
	int a;
	int b;
	double t;
};

/**
 * This function computes the cumulative intensity for a single mode and node.
 */
class WeibullCumulativeFunction : public WeibullMeasureFunction
{
public:
	WeibullCumulativeFunction(int alpha, int beta, double time)
		{a = alpha; b = beta; t = time;}
	~WeibullCumulativeFunction() {}
	double compute(const float * arr) const {return getCumulative(exp(arr[a]),arr[b],t);}
	double compute(const float * arr, const double t) const {return getCumulative(exp(arr[a]),arr[b],t);}
	OperatorFunction * clone(void) const {return new WeibullCumulativeFunction(a,b,t);}
private:
	int a;
	int b;
	double t;
};

class WeibullI1000Function : public WeibullMeasureFunction
{
public:
	WeibullI1000Function(int alpha, int beta, double time)
		{a = alpha; b = beta; t = time;}
	~WeibullI1000Function() {}
	double compute(const float * arr) const {return 1000 * getCumulative(exp(arr[a]),arr[b],t);}
	double compute(const float * arr, const double t) const {return 1000 * getCumulative(exp(arr[a]),arr[b],t);}
	OperatorFunction * clone(void) const {return new WeibullI1000Function(a,b,t);}
private:
	int a;
	int b;
	double t;
};

class WeibullLnCumulativeFunction : public WeibullMeasureFunction
{
public:
	WeibullLnCumulativeFunction(int alpha, int beta, double time)
		{a = alpha; b = beta; lnt = log(time);}
	~WeibullLnCumulativeFunction() {}
	double compute(const float * arr) const {return (lnt-arr[a])*arr[b];}
	double compute(const float * arr, const double t) const {return (log(t)-arr[a])*arr[b];}
	OperatorFunction * clone(void) const {return new WeibullLnCumulativeFunction(a,b,lnt);}
private:
	int a;
	int b;
	double lnt;
};

/**
 * This function computes the instantaneous intensity for a single mode and node.
 */
class WeibullIntensityFunction : public WeibullMeasureFunction
{
public:
	WeibullIntensityFunction(int alpha, int beta, double time)
		{a = alpha; b = beta; t = time;}
	~WeibullIntensityFunction() {}
	double compute(const float * arr) const {return getIntensity(exp(arr[a]),arr[b],t);}
	double compute(const float * arr, const double t) const {return getIntensity(exp(arr[a]),arr[b],t);}
	OperatorFunction * clone(void) const {return new WeibullIntensityFunction(a,b,t);}
private:
	int a;
	int b;
	double t;
};

/**
 * Function that computes the reliability for a node considering all failure modes.
 *
 * This class and derived classes depend on a particular layout of
 * Weibull scale and shape parameters. See implementations of 
 * getShapeIndex() and getScaleIndex for details.
 */
class TotalWeibullReliabilityFunction : public WeibullMeasureFunction
{
public:

	/**
	 * Constructor. 
	 * Nodes start counting at 0.
	 */
	TotalWeibullReliabilityFunction(const int node, const int node_cnt, const int mode_cnt, const double time) 
	{this->node = node; this->node_cnt = node_cnt; this->mode_cnt = mode_cnt, this->time = time;}
	~TotalWeibullReliabilityFunction() {}
	double compute(const float * arr) const {return compute(arr,time);}
	double compute(const float * arr, const double time) const;
	OperatorFunction * clone(void) const {return new TotalWeibullReliabilityFunction(node,node_cnt,mode_cnt,time);}
private:
	int node;		// node for which to compute reliability
	int node_cnt;	// number of nodes
	int mode_cnt;	// number of failure modes
	double time;	// time for which to compute reliability
};

/**
 * Function that computes cumulative failure rate for a node considering all failure modes.
 *
 * This class and derived classes depend on a particular layout of
 * Weibull scale and shape parameters. See implementations of 
 * getShapeIndex() and getScaleIndex for details.
 */
class TotalWeibullCumulativeFunction : public WeibullMeasureFunction
{
public:
	/**
	 * Constructor. 
	 * Nodes start counting at 0.
	 */
	TotalWeibullCumulativeFunction(const int node, const int node_cnt, const int mode_cnt, const double time) 
	{this->node = node; this->node_cnt = node_cnt; this->mode_cnt = mode_cnt, this->time = time;}
	~TotalWeibullCumulativeFunction() {}
	double compute(const float * arr) const {return compute(arr,time);}
	double compute(const float * arr, const double time) const;
	OperatorFunction * clone(void) const {return new TotalWeibullCumulativeFunction(node,node_cnt,mode_cnt,time);}
private:
	int node;		// node for which to compute intensity
	int node_cnt;	// number of nodes
	int mode_cnt;	// number of failure modes
	double time;	// time for which to compute intensity
};


class TotalWeibullI1000Function : public WeibullMeasureFunction
{
public:
	/**
	 * Constructor. 
	 * Nodes start counting at 0.
	 */
	TotalWeibullI1000Function(const int node, const int node_cnt, const int mode_cnt, const double time) 
	{this->node = node; this->node_cnt = node_cnt; this->mode_cnt = mode_cnt, this->time = time;}
	~TotalWeibullI1000Function() {}
	double compute(const float * arr) const {return compute(arr,time);}
	double compute(const float * arr, const double time) const;
	OperatorFunction * clone(void) const {return new TotalWeibullI1000Function(node,node_cnt,mode_cnt,time);}
private:
	int node;		// node for which to compute intensity
	int node_cnt;	// number of nodes
	int mode_cnt;	// number of failure modes
	double time;	// time for which to compute intensity
};



/**
 * Function that computes failure intensity for a node considering all failure modes.
 *
 * This class and derived classes depend on a particular layout of
 * Weibull scale and shape parameters. See implementations of 
 * getShapeIndex() and getScaleIndex for details.
 */
class TotalWeibullIntensityFunction : public WeibullMeasureFunction
{
public:
	/**
	 * Constructor. 
	 * Nodes start counting at 0.
	 */
	TotalWeibullIntensityFunction(const int node, const int node_cnt, const int mode_cnt, const double time) 
	{this->node = node; this->node_cnt = node_cnt; this->mode_cnt = mode_cnt, this->time = time;}
	~TotalWeibullIntensityFunction() {}
	double compute(const float * arr) const {return compute(arr,time);}
	double compute(const float * arr, const double time) const;
	OperatorFunction * clone(void) const {return new TotalWeibullIntensityFunction(node,node_cnt,mode_cnt,time);}
private:
	int node;		// node for which to compute intensity
	int node_cnt;	// number of nodes
	int mode_cnt;	// number of failure modes
	double time;	// time for which to compute intensity
};
