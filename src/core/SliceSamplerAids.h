// $Id:$ 

#pragma once

#include "paramarray.h"
#include "deviategenerator.h"
#include <math.h>

/**
 * Utiliity class that specifies the implementations of STEP OUT phase.
 *
 */
class SliceSamplerAid
{
public:
	SliceSamplerAid(void) {}
	virtual ~SliceSamplerAid(void) {}
	virtual int doStepOut(ParamArray & params, const int index, const double lny, 
		float & lo, float & hi, const DeviateGenerator & deviate) const = 0;
	virtual float getStartValue() const = 0;
	// notifies the sampler aid of the distance between the last samples.
	virtual void notify(float distance) = 0;
};

/**
 * Sampler aid that performs standard step out procedure.
 */
class DefaultSliceSamplerAid : public SliceSamplerAid
{
public:
	DefaultSliceSamplerAid(float width,int maxsteps,float start) 
		{w=width;m=maxsteps;s=start;}
	~DefaultSliceSamplerAid(void) {}
	int doStepOut(ParamArray & params, const int index, const double lny, 
		float & lo, float & hi, const DeviateGenerator & deviate) const;
	float getStartValue() const {return s;}
	void notify(float distance) {w *= (float)0.96; w += (float)fabs(distance)  / 25;}
protected:
	float w;	// initial width of interval
	int m;		// max number of steps
	float s;	// start value
};

/** 
 * Sampler aid that returns fixed interval bounds.
 */
class FixedSliceSamplerAid : public SliceSamplerAid
{
public:
	FixedSliceSamplerAid(float lo, float hi, float start)
		{min = lo; max = hi;s = start; w = (hi + lo) / 2;}
	~FixedSliceSamplerAid() {}
	int doStepOut(ParamArray & params, const int index, const double lny, 
		float & lo, float & hi, const DeviateGenerator & deviate) const;
	/// Returns the start value for the variable.
	float getStartValue() const {return s;}
	void notify(float distance) {w *= (float)0.96; w += (float)fabs(distance)  / 25;}
protected:
	float w;
	float min;	// interval lower bound
	float max;	// interval upper bound
	float s;	// start value
};

/**
 * Slice sampler aid for strictly positive variables.
 */
class PositiveSliceSamplerAid : public SliceSamplerAid
{
public:
	PositiveSliceSamplerAid(float width,int maxsteps, float start)
		{w = width;m = maxsteps; s = start;}
	~PositiveSliceSamplerAid() {}
	int doStepOut(ParamArray & params, const int index, const double lny, 
		float & lo, float & hi, const DeviateGenerator & deviate) const;
	float getStartValue() const {return s;}
	void notify(float distance) {w *= (float)0.96; w += (float)fabs(distance) / 25;}
protected:
	float s;
	float w;
	int m;
};

/**
 * Slice sampler aid for strictly positive variables.
 */
class NegativeSliceSamplerAid : public SliceSamplerAid
{
public:
	NegativeSliceSamplerAid(float width,int maxsteps, float start)
		{w = width;m = maxsteps; s = start;}
	~NegativeSliceSamplerAid() {}
	int doStepOut(ParamArray & params, const int index, const double lny, 
		float & lo, float & hi, const DeviateGenerator & deviate) const;
	float getStartValue() const {return s;}
	void notify(float distance) {w *= (float)0.96; w += (float)fabs(distance) / 25;}
protected:
	float s;
	float w;
	int m;
};
