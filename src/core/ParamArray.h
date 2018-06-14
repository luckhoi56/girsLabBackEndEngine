// $Id: ParamArray.h 2258 2014-03-07 02:57:39Z Dongfeng Zhu $

#pragma once

#include "NetOperator.h"
#include "NetModel.h"
#include "NetCorrelation.h"

#include <iostream>

using namespace std;

class SliceSamplerAid;

/**
 * Parameter array.
 *
 * This class represents an array representation of the probabilistic network.
 * Each variable in the network is represented in the array.
 *
 * The size of the array are determined by the layout used.In BRASS, WeibullModelLayout
 * is used by default.
 *
 * In addition, it holds references to the operators applicable to each 
 * variable in the network. Operators are stored in singly linked lists,
 * which are defined for each value in the array. Therefore, copies of the 
 * operators are created for each parameter to which the operator applies.
 *
 * @see WeibullModelLayout
 * @see NetOperator
 */
class ParamArray
{
public:
	ParamArray(const int len);
	~ParamArray(void);
	
	bool addModel(NetModel & md);
	bool addOperator(const NetOperator & op);
	int size() const {return len;}
	double getLnDensity(const int index); // const;
	/// Sets the value in the parameter array.
	void setParam(const int index, const float value);

	void updateParam(const int index, const float value);

	/// Returns the value with specified index.
	inline float getParam(const int index) const {return params[index];}
	/// Copies the values to the specified array.
	inline void getParams(float * arr) const 
		{for (int i = 0 ; i < len ; i++) arr[i] = (float)params[i];}
	/// Copies the n first values to the specified array.
	inline void getParams(float * arr,int n) const 
		{for (int i = 0 ; i < n ; i++) arr[i] = (float)params[i];}

	int initialize();
	bool hasModel(const int index) const {return this->operators[index] != 0;}

	void dumpSummary(void);

	/**
	 * Returns the range specifier for the specified parameter.
	 * The range indicates whether the parameter is fixed, positive only,
	 * or positive or negative. Range specifier constants are defined in 
	 * this class.
	 */
	inline short getRange(const int index) const {return ranges[index];}

	/// Returns the sampler aid for the specified variable.
	SliceSamplerAid *getAid(const int index) const {return this->sampler_aids[index];}

	/// Sets the sampler aid for the specified variable.
	void setAid(const int index, SliceSamplerAid * aid);

	/// Sets the Range property for the parameter.
	void setRange(const int index, const short range) {ranges[index] = range;}

	void addCorrelation(const int i1, const int i2);
	void updateCorrelated(const int index, const float distance);

	const static short RANGE_UNDEFINED = -1;

    /// Range specifier: fixed value.
	const static short RANGE_FIXED = 3;
	/// Range specifier: dependent value (skipped by sampler)
	const static short RANGE_DEPENDENT = 4;
	/// Range specifier: negative values only
	const static short RANGE_VARIABLE = 5;

protected:

	double updateLinks(const int index);
	void exploreLinks(const int index); 
	void exploreCorrelated(const int index); 
	int initLinks(const int index);
	void reduceLinks(const int index, bool * reduced_vars);

	void addOperator(const int index, const NetOperator & op);

	float * params;						// array of parameter values
	short * ranges;						// array of parameter range indicators; possible values are:
										//		RANGE_UNDEFINED, RANGE_FIXED, RANGE_DEPENDENT, RANGE_VARIABLE
	NetOperatorPtr * operators;			// array of operator lists
	NetModelRefPtr * models;			// array of model lists; possible types of model are:
										//      IdentityModel, LinkedAdjustmentModel, LinkedRateAdjustmentModel, LinkedTimeAdjustmentModel
										//	    ProportionalModelOperator, ProportionalTimeOperator, ProportionalRateOperator
	NetCorrelationPtr * correlations;	// array of correlation lists.
	SliceSamplerAid ** sampler_aids;	
	int len;							// number of parameters (length of array)

	int validate(void);
};

ostream & operator<<(ostream & os, const ParamArray & a);
