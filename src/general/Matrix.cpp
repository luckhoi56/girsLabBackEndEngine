// Matrix.cpp: implementation of the Matrix class.
//
//////////////////////////////////////////////////////////////////////

/****************************************************
 *
 *  Bayesian Weibull Sampler.
 *
 *	(c) 2000, Frank Groen. All Rights Reserved.
 *
 ****************************************************/

#include "stdafx.h"
#include "Matrix.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Matrix::Matrix(int cols, int rows)
{
	_cols = cols;
	_rows = rows;
	_values = new double[cols * rows];

	for (int i = 0; i < cols * rows; i++) {
		_values[i] = 0;
	}
}

Matrix::Matrix()
{
	_cols = 0;
	_rows = 0;
	_values = 0L;
}

Matrix::Matrix(const Matrix & matrix)
{
	_cols = matrix.getWidth();
	_rows = matrix.getHeight();
	_values = new double[_cols * _rows];

	for (int col = 0; col < _cols; col++) {
		for (int row = 0; row < _rows; row++) {
			_values[col * _rows + row] = matrix.getValue(col,row);
		}
	}
}

Matrix::~Matrix()
{

}

Parameters Matrix::getColumn(int col) const
{
	Parameters result(_rows);
	
	for (int row = 0; row < _rows; row++)
		result.setValue(row,_values[col * _rows + row]);

	return result;
}

void Matrix::setColumn(int col, Parameters & values)
{
	for (int row = 0; row < _rows; row++)
		_values[col * _rows + row] = values.getValue(row);
}

double Matrix::getValue(int col, int row) const
{
	return _values[col * _rows + row];
}

void Matrix::setValue(int col, int row, double value)
{
	_values[col * _rows + row] = value;
}

Matrix Matrix::constructUnitMatrix(int dimension)
{
	Matrix m(dimension,dimension);
	for (int i = 0; i < dimension; i++) {
		m.setValue(i,i,1);
	}
	return m;
}

int Matrix::getHeight() const
{
	return _rows;
}

int Matrix::getWidth() const
{
	return _cols;
}

Matrix & Matrix::operator= (const Matrix &rhs)
{
	// resize if necessary
	if (rhs.getWidth() != _cols || rhs.getHeight() != _rows) {
		if (_values != 0L) {
			delete _values;
		}
		_cols = rhs.getWidth();
		_rows = rhs.getHeight();
		_values = new double[_cols * _rows];
	}

	for (int row = 0; row < _rows; row++) {
		for (int col = 0; col < _cols; col++) {
			_values[col * _rows + row] = rhs.getValue(col,row);
		}
	}

	return *this;
}
