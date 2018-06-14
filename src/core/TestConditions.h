// $Id: TestConditions.h 2238 2014-02-22 15:48:40Z Dongfeng Zhu $

#pragma once

/**
 * Acceleration factor information (fixed factor)
 * Type is one of ACCELERATE_NONE, ACCELERATE_TIME, ACCELERATE_RATE
 * defined in TestConditions.
 */
struct AccelerationFactor 
{
	/// Acceleration mode: no acceleration (acc. fac = 1)
	static const int ACCELERATE_NONE = 0;
	/// Acceleration mode: time to failure adjustment
	static const int ACCELERATE_TIME = 1;
	/// Acceleration mode: failure rate adjustment
	static const int ACCELERATE_RATE = 2;

	static const int ACCELERATE_NULL = 3;
	/// acceleration type identifier
	int type;		// acceleration type	
	/// acceleration factor
	double factor;	// acceleration factor
};

/**
 * Container class for details on testing conditions.
 * The class stores acceleration factors for each combination of 
 * test condition and failure mode.
 * 
 * Design Note: 
 *
 * The current implementation is limited to fixed factor models. For 
 * physical models, it is anticipated that the acceleration factors 
 * will become functions of the parameter array and stress factors
 * recorded in the data records.
 */
class TestConditions
{
public:
	TestConditions(int conditions, int modes);
	~TestConditions(void);

	/// Obtains a handle to the object. 
	TestConditions * obtain(void) {use_count++; return this;}
	/// Release the object. Returns true if no handles are left
	bool release() {return (use_count--) == 0;}
	/// Returns the number of handles for this object.
	int getUseCount(void) const {return use_count;}
	/// Returns acceleration info for specified condition and failure mode.
	/// Failure mode numbering starts at 1.
	const AccelerationFactor * getAcceleration(int condition, int mode) const
		{ return &accelerations[condition_cnt * (mode - 1) + condition]; }
	void setAcceleration(int condition, int mode, int type, double factor);
	int getConditionCount() const {return condition_cnt;}
	int getModeCount() const {return mode_cnt;}

private:
	int use_count;			// number of handles to this object
	int condition_cnt;		// number of conditions
	int mode_cnt;			// number of failure modes
	AccelerationFactor * accelerations;	// acceleration factors.
};
