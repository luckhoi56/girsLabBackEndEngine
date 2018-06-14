// $Id: AdjustAggregateOperator.h 352 2005-03-11 17:08:23Z saulo $

#pragma once

#include "../core/NetOperator.h"
#include "WeibullModelLayout.h"

/**
 * Base class for aggregate reliability function adjustors.
 * See derived class descriptions for details. This class and
 * derived classes are specific to the Weibull model.
 *
 * <B>DO NOT USE THIS CLASS.</B> This class has been superceded by 
 * the LinkedAdjustmentModel subclasses in combination with an instance
 * of an AdjustWeibullOperator.
 */
class AdjustAggregateOperator : public NetOperator
{
public:
	AdjustAggregateOperator(int a_node,int e_node,double z,double errfac,int mode_cnt,int node_cnt);
	virtual ~AdjustAggregateOperator(void);

	bool appliesTo(const int index) const;
	bool specifies(const int index) const;
	NetOperatorPtr absorb(NetOperatorPtr * operators, int index);
	int getOrder() const;

	bool isDensity(const int index) const {return false;}

protected:
	/// Returns shape variable index for specified mode.
	int getShapeIndex(const int mode) const 
	{return WeibullModelLayout::getShapeIndex(mode,node_cnt);}
	/// Returns scale varaible index for specified mode and node.
	int getScaleIndex(const int node, const int mode) const 
	{return WeibullModelLayout::getScaleIndex(node,mode,node_cnt);}

	int node_cnt;	///< number of nodes in the model
	int mode_cnt;	///< number of modes in the model
	int a_node;		///< alpha node index
	int e_node;		///< eta node index
	double mean;	///< natural log of median, mean of underlying normal
	double sdev;	///< standard deviation of underlying normal
};

/**
 * Aggregate failure rate adjustment operator <b>DO NOT USE</b>.
 *
 * This operator implements a proportional failure rate adjustment model, where
 * the adjustment factor is uncertain (lognormally distributed). The model is
 * specified between two product nodes, and applies to the sum of failure rates.
 * The adjustment is implemented by applying at all times the same z-factor 
 * to all failure modes. Note that strictly speaking this is different from 
 * applying z to the sum of failure rates.
 *
 * <B>DO NOT USE THIS CLASS.</B> This class has been superceded by 
 * the LinkedAdjustmentModel subclasses in combination with an instance
 * of an AdjustWeibullOperator.
 *
 * This operator is specific to the Weibull model.
 */
class AggregateRateOperator : public AdjustAggregateOperator
{
public:

//	AggregateRateOperator(int a_node,int e_node,double z,double errfac,int mode_cnt, int node_cnt) :
//		AdjustAggregateOperator(a_node,e_node,z,errfac,mode_cnt,node_cnt) {}
	~AggregateRateOperator(void) {}
	NetOperatorPtr copy() const ;
	double compute(float * params, float * derivs) const;
};

/**
 * Aggregate failure rate adjustment operator <b>DO NOT USE</b>.
 *
 * This operator implements a proportional time to failure adjustment model, where
 * the adjustment factor is uncertain (lognormally distributed). The model is
 * specified between two product nodes, and applies to the aggregate reliability
 * function.
 *
 * The adjustment is implemented by applying at all times the same z-factor 
 * to all failure modes. Note that strictly speaking this is different from 
 * applying z to the aggregate reliability function.
 *
 * <B>DO NOT USE THIS CLASS.</B> This class has been superceded by 
 * the LinkedAdjustmentModel subclasses in combination with an instance
 * of an AdjustWeibullOperator.
 *
 * This operator is specific to the Weibull model.
 */
class AggregateTimeOperator : public AdjustAggregateOperator
{
public:

//	AggregateTimeOperator(int a_node,int e_node,double z,double errfac,int mode_cnt,int node_cnt) :
//		AdjustAggregateOperator(a_node,e_node,z,errfac,mode_cnt,node_cnt) {}
	~AggregateTimeOperator(void) {}
	NetOperatorPtr copy() const; // {return new AggregateTimeOperator(*this);}
	double compute(float * params, float * derivs) const;
};
