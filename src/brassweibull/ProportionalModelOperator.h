// $Id: ProportionalModelOperator.h 2240 2014-02-23 02:14:31Z Dongfeng Zhu $

#pragma once

#include "../core/NetOperator.h"
#include "../core/NetModel.h"
#include <iostream>

/**
 * Superclass for proportional time/rate constraints.
 *
 * These models are specific to the Weibull distribution.
 *
 * This class can be used to introduce the constraints Rg(t) = Ra(z*t) 
 * or lg(t) = z * la(t) where z is a fixed value. The operator causes 
 * a degree of freedom to be removed from the model. Whenever the operator
 * is called, it makes the appropriate adjustment to the parameter array,
 * such that the constraint is upheld. 
 * 
 * The class furthermore overrides the absorb method in order to 
 * get the appropriate operator lists.
 *
 * @see AdjustWeibullOperator
 */
class ProportionalModelOperator : public NetModel // NetOperator
{
public:
	ProportionalModelOperator(int alpha, int gamma, int beta, double z);
	virtual ~ProportionalModelOperator(void);
	bool specifies(const int var_index) const;
	int getSpecified(void) const;
	int init(const int var_index);
	void print(ostream & os) const;

protected:
	int alpha;
	int beta;
	int gamma;
	float lnz;
	int direction;	///< direction in which the model is evaluated
	
	const static int DIR_UNDECIDED = 0; ///< direction is undecided.
	const static int DIR_A_TO_E = -1;	///< direction is from alpha to gamma
	const static int DIR_E_TO_A = 1;	///< direction is from gamma to alpha
};

ostream & operator<<(ostream & os, const ProportionalModelOperator & m);

/**
 * Operator class for proportional time constraints with fixed z factor.
 * <b>Use of this class requires that shape factor beta is placed before
 * scale parameters in the variable sequence.</b>
 *
 * This models are specific to the Weibull distribution.
 *
 * @see ProportionalModelOperator
 * @see AdjustWeibullTimeOperator
 */
class ProportionalTimeOperator : public ProportionalModelOperator
{
public:
	ProportionalTimeOperator(int alpha, int gamma, int beta, double z) :
	  ProportionalModelOperator(alpha,gamma,beta,z) {}
	~ProportionalTimeOperator(void) {}
	double update(float * vals, int & next_var) ;
	virtual bool appliesTo(const int index) const 
		{return index == alpha || index == gamma;}
	NetModelPtr copy() const;
};

/**
 * Operator class for proportional rate constraints with fixed z factor.
 * <b>Use of this class requires that shape factor beta is placed before
 * scale parameters in the variable sequence.</b>
 *
 * This model is specific to the Weibull distribution.
 *
 * @see ProportionalModelOperator
 * @see AdjustWeibullRateOperator
 */
class ProportionalRateOperator : public ProportionalModelOperator
{
public:
	ProportionalRateOperator(int alpha, int gamma, int beta, double z) :
	  ProportionalModelOperator(alpha,gamma,beta,z) {}
	~ProportionalRateOperator(void) {}
	virtual bool appliesTo(const int index) const 
		{return index == alpha || index == beta || index == gamma;}
	double update(float * vals, int & next_var) ;
	NetModelPtr copy() const;
};
