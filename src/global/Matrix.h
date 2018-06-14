// Matrix.h: interface for the Matrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATRIX_H__57123E46_3EE8_11D4_9881_002018557056__INCLUDED_)
#define AFX_MATRIX_H__57123E46_3EE8_11D4_9881_002018557056__INCLUDED_

#include "Parameters.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Parameters.h"

/* Matrix of double values.
 */
class Matrix  
{
public:
	/* Constructs unit matrix of specified dimension
	 */
	static Matrix constructUnitMatrix(int dimension);

	/* Sets value at specified position.
	 */
	void setValue(int col, int row, double value);

	/* Returns value at specified position.
	 */
	double getValue(int col,int row) const;

	/* Sets values of specified column
	 */
	void setColumn(int col, Parameters & values);

	/* Returns values of specified column.
	 */
	Parameters getColumn(int col) const;

	/* Returns number of columns in the matrix.
	 */
	int getWidth() const;

	/* Returns number of rows in the matrix.
	 */
	int getHeight() const;

	/* assignment operator. */
	Matrix& operator= (const Matrix &rhs);

	Matrix();
	Matrix(const Matrix & matrix);
	Matrix(int cols, int rows);

	virtual ~Matrix();
private:
	double * _values;	// array of values
	int _cols;			// number of columns in the matrix
	int _rows;			// number of rows in the matrix
};

#endif // !defined(AFX_MATRIX_H__57123E46_3EE8_11D4_9881_002018557056__INCLUDED_)
