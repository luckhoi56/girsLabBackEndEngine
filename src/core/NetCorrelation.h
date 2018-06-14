#pragma once

/**
 * This class is used to define the instance of net correlation. The array of
 * a list of instance of this class represents the correlation within probabilistic network. 
 *
 * The parameter value of the correlated variable is always the same. User needs to use
 * updateCorrelated function to update all correlated variables when the main variable
 * value change.
 *
 * Each variable can be correlated with multiple variables.
 */
class NetCorrelation
{
public:
	NetCorrelation(void);
	~NetCorrelation(void);
	void setTarget(const int t) {target = t;}
	int getTarget(void) const {return target;}
	void setNext(NetCorrelation * next) {this->next = next;}
	NetCorrelation * getNext(void) const {return next;}
private:
	int target;					// Target index
	NetCorrelation * next;		// Pointer to the next correlation instance
};

typedef NetCorrelation * NetCorrelationPtr;