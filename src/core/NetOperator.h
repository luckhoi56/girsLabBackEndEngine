// $Id: NetOperator.h 2091 2006-01-09 13:17:55Z fgroen $

#pragma once

class NetOperator;

typedef NetOperator * NetOperatorPtr;
typedef int OperatorID;

/**
 * Base class for network operators.
 * Operators are used to compute the natural log of the (unnormalized) density
 * given the values stored in the parameter array. 
 * 
 * Operators are normally stored in lists. For more information, see the ParamArray
 * documentation.
 */
class NetOperator
{
public:

	const static int ORDER_LAST = 999999;

	NetOperator(void);
	NetOperator(const NetOperator & op);
	virtual ~NetOperator(void);

	/// Returns the next operator in the list.
	inline NetOperatorPtr getNext() const {return next;}
	inline int getOperatorID() const {return operator_id;}

	/**
	 * Links the operator to this operator. 
	 * The new head of the list is returned.
	 */
	NetOperatorPtr link(NetOperatorPtr op_ptr) {next = op_ptr; return this;}

	/**
	 * Computes the value for this operator.
	 * Implementations of this method perform the density computations. 
	 * Normally, the natural log of the density is returned.
	 */
	virtual double compute(float * params, float * derivs) const = 0;

	virtual NetOperatorPtr reduce(NetOperatorPtr & trash);
	virtual NetOperatorPtr absorb(NetOperatorPtr * operator_arr,int index);
	NetOperatorPtr absorb(NetOperatorPtr operator_lst);
	virtual NetOperatorPtr reorder(void);
	virtual int getOrder() const {return ORDER_LAST;}
	virtual bool equals(const NetOperatorPtr &op) const 
		{return op->operator_id == operator_id;} 

	/**
	 * Creates an unlinked copy of the operator.
	 * Implementations of this method create a duplicate of the operator 
	 * without linking the operator to any operator list.
	 */
	virtual NetOperatorPtr copy() const = 0;

	bool contains(const NetOperatorPtr op) const;

	/// Returns true if operator should be attached to specified variable.
	virtual bool appliesTo(const int index) const = 0;

	/// Returns true if operator list specifies (non-uniform) density for variable.
	virtual bool isDensity(const int index) const = 0;

	/**
	 * Returns true if operator specifies the value of the variable.
	 * Based on the return value, variables are marked as RANGE_DEPENDENT.
	 * @param index int index of the variable.
	 */
	virtual bool specifies(const int index) const = 0;

protected:
	NetOperatorPtr next;	  // next operator in the list.
	OperatorID operator_id;   // operator ID number, shared by duplicate operators
	static int next_id;		  // next operator ID to be generated.
};
