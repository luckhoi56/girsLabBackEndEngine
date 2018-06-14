// $Id: NetModel.h 2240 2014-02-23 02:14:31Z Dongfeng Zhu $

#pragma once

#include <iostream>

using namespace std;

/**
 * Deterministic model.
 *
 * Instances of this class are used to introduce deterministic models into 
 * the network. These models modify their 'output' variable, when the 'input' 
 * variables are modified. Note that the model itself needs to decide on which 
 * variables is the output variable based on the order in which the network 
 * is evaluated.
 */ 
class NetModel
{
public:

	NetModel(void);

	virtual ~NetModel(void);

	/**
	 * Initializes the model link.
	 * An important aspect of the init operation is for the 
	 * model to determine in which 'direction' it will operate.
	 * The function returns the index of the variable that will 
	 * be set by the model (the dependent variable), or -1 if 
	 * the input variable index does not determine in which direction
	 * the link is evaluated.
	 * @param var_index variable whose change in value triggered this call.
	 */
	virtual int init(const int var_index) = 0;

	virtual double update(float * vals, int & next_var) = 0;

	/**
	 * Returns true if the model depends on the specified variable.
	 */
	virtual bool appliesTo(const int var_index) const = 0;

	/**
	 * Returns true if the model considers the variable as its dependent variable.
	 */
	virtual bool specifies(const int var_index) const = 0;

	/**
	 * Returns the index of the variable specified by this model.
	 */
	virtual int getSpecified(void) const = 0;

	virtual NetModel * copy() const = 0;

	virtual void print(ostream & os) const = 0;

	void obtain(void) {use_cnt++;}
	bool release(void) {return (--use_cnt == 0);}

private:
	int use_cnt;

};


class IdentityModel : public NetModel
{
public:

	IdentityModel(const int index1, const int index2);

	virtual ~IdentityModel(void) {}

	/**
	 * Initializes the model link.
	 * An important aspect of the init operation is for the 
	 * model to determine in which 'direction' it will operate.
	 * The function returns the index of the variable that will 
	 * be set by the model (the dependent variable), or -1 if 
	 * the input variable index does not determine in which direction
	 * the link is evaluated.
	 * @param var_index variable whose change in value triggered this call.
	 */
	virtual int init(const int var_index);
	virtual double update(float * vals, int & next_var);

	/**
	 * Returns true if the model depends on the specified variable.
	 */
	virtual bool appliesTo(const int var_index) const;

	/**
	 * Returns true if the model considers the variable as its dependent variable.
	 */
	virtual bool specifies(const int var_index) const;

	/**
	 * Returns the index of the variable specified by this model.
	 */
	virtual int getSpecified(void) const;
	virtual NetModel * copy() const;
	virtual void print(ostream & os) const;

private:
	int index_to;
	int index_from;
	int direction;
	const static int DIR_UNDECIDED = 0;
	const static int DIR_DECIDED = 1;
};


/**
 * Reference to a NetModel object.
 * This class can be used to construct a single linked list
 * of references to NetModel objects.
 * @see NetModel
 */
class NetModelRef
{
public:
	NetModelRef(void) {model = 0; next = 0; disabled = false;}
	~NetModelRef(void) ;
	NetModel * getModel() const {return model;}
	NetModelRef * getNext() const {return next;}
	void setNext(NetModelRef * ref) {next = ref;}
	void setModel(NetModel * md); 
	// NetModelRef * reduce();
	NetModelRef * copy(void) const;
	bool contains(const NetModel * model) const;
private:
	NetModel * model;
	NetModelRef * next;
	bool disabled;
};

typedef NetModel * NetModelPtr;
typedef NetModelRef * NetModelRefPtr;

ostream & operator<<(ostream & os, const NetModel & m);

