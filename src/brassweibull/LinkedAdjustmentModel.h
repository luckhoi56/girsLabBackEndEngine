// $Id: LinkedAdjustmentModel.h 2240 2014-02-23 02:14:31Z Dongfeng Zhu $

#pragma once

#include "../core/NetModel.h"

/**
 * Superclass for model that takes adjustment factor from variables in the model.
 */
class LinkedAdjustmentModel : public NetModel
{
public:
	LinkedAdjustmentModel(const int node0, const int node1, const int node_cnt, const int mode, const int src_mode);
	virtual ~LinkedAdjustmentModel(void);
	virtual int init(const int var_index);
	virtual bool specifies(const int var_index) const;
	int getSpecified(void) const;
	virtual void print(ostream & os) const;
protected:
	// Variable for source failure mode; the detailed adjustment factor will be calculated from the source
	// mode
	int src_alpha;		// Scale parameter index for node 0
	int src_beta;		// Shape parameter index
	int src_gamma;		// Scale parameter index for node 1

	// Variable for destination failure mode
	int dst_alpha;		// Scale parameter index for node 0
	int dst_beta;		// Shape parameter index
	int dst_gamma;		// Scale parameter index for node 1

	int direction;		// Direction; must be one of the three value defined below

	const static int DIR_UNDECIDED = 0; ///< direction is undecided.
	const static int DIR_A_TO_E = -1;	///< direction is from alpha to gamma
	const static int DIR_E_TO_A = 1;	///< direction is from gamma to alpha
};

/**
 * Rate adjustment model that takes adjustment factor from variables in the model.
 *
 * This model allows the creation of correlated adjustment models. It determines
 * the adjustment factor based on the scale/shape parameters of another adjustment
 * model, then applies the same adjustment factor to a 'target' model. This allows
 * for instance to have an aggregate adjustment model in which all failure modes
 * of an aggregate reliability model are adjusted in an identical manner in each
 * iteration step.
 * 
 */
class LinkedRateAdjustmentModel : public LinkedAdjustmentModel 
{
public:
	LinkedRateAdjustmentModel(const int node0, const int node1, const int node_cnt, const int mode, const int src_mode) : 
	  LinkedAdjustmentModel(node0,node1,node_cnt,mode,src_mode) {}
	  ~LinkedRateAdjustmentModel(void) {}
	virtual double update(float * vals, int & next_var);
	virtual bool appliesTo(const int var_index) const;
	virtual NetModel * copy() const;

};

/**
 * Time adjustment model that takes adjustment factor from variables in the model.
 *
 * This model allows the creation of correlated adjustment models. It determines
 * the adjustment factor based on the scale/shape parameters of another adjustment
 * model, then applies the same adjustment factor to a 'target' model. This allows
 * for instance to have an aggregate adjustment model in which all failure modes
 * of an aggregate reliability model are adjusted in an identical manner in each
 * iteration step.
 * 
 */
class LinkedTimeAdjustmentModel : public LinkedAdjustmentModel
{
public:
	LinkedTimeAdjustmentModel(const int node0, const int node1, const int node_cnt, const int mode, const int src_mode) : 
	  LinkedAdjustmentModel(node0,node1,node_cnt,mode,src_mode) {}
	  ~LinkedTimeAdjustmentModel(void) {}
	virtual double update(float * vals, int & next_var);
	virtual bool appliesTo(const int var_index) const;
	virtual NetModel * copy() const;
};
