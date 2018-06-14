// Parameters.h: interface for the Parameters class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARAMETERS_H__265CFB24_3E72_11D4_9881_002018557056__INCLUDED_)
#define AFX_PARAMETERS_H__265CFB24_3E72_11D4_9881_002018557056__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Parameters  
{
public:
	bool inSpace(const Parameters & minima, const Parameters & maxima) const;
	Parameters getSquared() const;
	void setDimension(int dimension);

	/**
	 * Creates Parameters object with dimension 0.
	 */
	Parameters();

	/**
	 * Creates Parameters object with specified dimension.
	 * Values are defaulted to 0.
	 */
	Parameters(int dimension);

	/**
	 * Creates a 2-dimensional Parameters object with given values.
	 */
	Parameters(double value0, double value1);

	Parameters(double value0);

	/**
	 * Copy constructor.
	 */
	Parameters(const Parameters & params);

	virtual ~Parameters();

	/**
	 * Sets value with specified index. Index ranges from 0 to dimension - 1.
	 */
	void setValue(int index, double value);

	/**
	 * Returns value with specified index. Index ranges from 0 to dimension - 1.
	 */
	double getValue(int index) const ;

	/**
	 * Returns the dimension of the Parameters object.
	 */
	int getDimension() const ;

	/**
	 * Adds two Parameters by adding their corresponding values.
	 */
	Parameters operator+ (const Parameters & rhs) const;

	/**
	 * Subtracts parameter values.
	 */
	Parameters operator- (const Parameters & rhs) const;

	/**
	 * Multiplies each value with 
	 */
	Parameters operator* (const double rhs) const;

	/**
	 * Copies the content of rhs into this Parameter object.
	 */
//	Parameters & operator= (const Parameters & rhs);
	void operator= (const Parameters & rhs);

	/**
	 * Returns a parameter object in which each of the 
	 * values is replaced by its exponent.
	 */
	Parameters getExponential() const;

	/**
	 * Dumps the content of this Parameter to cout. Only available
	 * when compiled with _DEBUG.
	 */
	void dump();

	/**
	 * Performs unit test on this class.
	 */
	static void testUnit();

#ifdef _DEBUG
	static int _nextID;
	int _objectID;
	static int _instances;
#endif

private:
	double * _values;
	int _dimension;
};

#endif // !defined(AFX_PARAMETERS_H__265CFB24_3E72_11D4_9881_002018557056__INCLUDED_)