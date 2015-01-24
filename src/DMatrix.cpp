// -*- mode: C++ -*-
/**
   Dietrich Bollmann, Kamakura, 2015/01/01
   
   DMatrix.cpp
   
   Class: DMatrix

   Copyright (c) 2015 Dietrich Bollmann
   
   This software may be modified and distributed under the terms
   of the MIT license.  See the LICENSE file for details.
*/

#include <cassert>
#include <cmath>

#include "DVector.h"
#include "DMatrix.h"

namespace nsl {

/** 
    Constructor.
*/
DMatrix::DMatrix(std::size_t rows, std::size_t cols) 
{
  _rows = rows;
  _cols = cols;

  _v = new std::vector<double> (rows * cols);
}

/** 
    Constructor.
*/
DMatrix::DMatrix(const std::vector<std::vector<double> > &values)
{
  _rows = values.size();
  _cols = (_rows == 0) ? 0 : values[0].size();

  _v = new std::vector<double> (_rows * _cols);

  for (std::size_t row = 0; row < _rows; ++row)
    {
      const std::vector<double> &rowValues = values[row];
      
      assert(rowValues.size() == _cols);
      
      for (std::size_t col = 0; col < _cols; ++col)
	(*this)(row, col) = rowValues[col];
    }
}

/** 
    Copy constructor.
*/
DMatrix::DMatrix(const DMatrix &matrix)
{
  _rows = matrix._rows;
  _cols = matrix._cols;

  _v = new std::vector<double> (*(matrix._v));
}

/** 
    Destructor.
*/
DMatrix::~DMatrix() 
{
  delete _v;
}

/**
   = operator for initialization.
*/
DMatrix &DMatrix::operator= (const std::vector<double> &values)
{
  assert(values.size() == size());
  
  std::size_t i = 0;
  for (std::size_t row = 0; row < _rows; ++row)
    for (std::size_t col = 0; col < _cols; ++col)
      (*this)(row, col) = values[i++];
  
  return *this;
}

/**
   = operator for initialization.
*/
DMatrix &DMatrix::operator= (const std::vector<std::vector<double> > &values)
{
  assert(values.size() > 0);

  std::size_t rows = values.size();
  std::size_t cols = values[0].size();

  this->resize(rows, cols);

  for (std::size_t row = 0; row < rows; ++row) 
    {
      const std::vector<double> &rowValues = values[row];
      
      assert(rowValues.size() == cols);
      
      for (std::size_t col = 0; col < cols; ++col)
	(*this)(row, col) = rowValues[col];
    }
  
  return *this;
}

/**
   Number of elements.
*/
std::size_t DMatrix::size() const
{
  return _rows * _cols;
}

/**
   Number of columns.
*/
std::size_t DMatrix::cols() const
{
  return _cols;
}

/**
   Number of rows.
*/
std::size_t DMatrix::rows() const
{
  return _rows;
}

/**
   Resize the matrix
*/
void DMatrix::resize(std::size_t rows, std::size_t cols)
{ 
  _rows = rows;
  _cols = cols;

  _v->resize(rows * cols);
}

/**
   Element accessor.
*/
double &DMatrix::operator() (std::size_t row, std::size_t col)
{
  assert(row < _rows);
  assert(col < _cols);

  return (*_v)[row * _cols + col];
}

/**
   Constant element accessor.
*/
double DMatrix::operator() (std::size_t row, std::size_t col) const
{
  assert(row < _rows);
  assert(col < _cols);

  return (*_v)[row * _cols + col];
}

/**
   Swap rows.
*/
void DMatrix::swapRows(const std::size_t i, const std::size_t j)
{
  assert(i != j);
  assert(i < _rows);
  assert(j < _rows);
  
  for (std::size_t col = 0; col < _cols; ++col)
    {
      double tmp = (*this)(i, col);
      (*this)(i, col) = (*this)(j, col);
      (*this)(j, col) = tmp;
    }
}

/**
   Remove rows and columns for which predicate(<column/row index>) is true.
*/
void DMatrix::deleteRowsAndColumns(std::function<bool (std::size_t i)> predicate)
{
  // Calculate the size of the new matrix.
  // The number of rows/columns in the new matrix
  // corresponds to the number of rows/columns in the old matrix
  // for which the predicate(<index of row/column>) is false.
  std::size_t rowsNew = 0;
  std::size_t colsNew = 0;
  std::size_t m       = std::max(_cols, _rows);
  for (std::size_t i = 0; i < m; ++i) {
    if (!predicate(i)) {
      if (i < _rows) ++rowsNew;
      if (i < _cols) ++colsNew;
    }
  }

  // Assemble the new matrix
  // using only those rows and columns for which the predicate(<index of row/column>) is false
  std::vector<double> *v = new std::vector<double> (rowsNew * colsNew);
  
  std::size_t k = 0;
  for (std::size_t r = 0; r < _rows; ++r)
    // Only use the rows for which predicate(r) is false
    if (!predicate(r))
      for (std::size_t c = 0; c < _cols; ++c)
  	// Only use the columns for which predicate(c) is false
	if (!predicate(c))
  	  (*v)[k++] = (*_v)[r * _cols + c];
  
  // Use the new vector
  delete _v;
  _v = v;
  
  _rows = rowsNew;
  _cols = colsNew;
}

/**
   Gaussian Elimination.
*/
DVector DMatrix::gaussianElimination(const DVector &b_) const
{
  // Gaussian Elimination only works for square matrices
  assert(_rows == _cols);

  // Size of matrix
  std::size_t n = _rows;

  // Copy matrix A and vector b
  DMatrix A(*this);
  DVector b(b_);
  
  for (std::size_t i = 0; i < n - 1; ++i) 
    {
      // =====================================
      // Pivoting
      // -------------------------------------
      // Find element with largest absolute value in column j
      // from A_(i, i) to A_(_rows-1, i).
      // The largest element is A_(row, i).
      std::size_t row = i;
      double largest = abs(A(i, i));

      for (std::size_t r = i + 1; r < _rows; ++r)
	{
	  double current = abs(A(r, i));
	  if (current > largest) 
	    {
	      row = r;
	      largest = current;
	    }
	}

      // If the largest found value is 0, the system is not solvable
      if (largest == 0)
	{
	  std::cerr 
	    << "ERROR The matrix is not solvable!" << std::endl
	    << std::endl
	    << "A(" << i << ", " << i << ") == 0" << std::endl
	    << std::endl
	    << "A:" << std::endl
	    << A << std::endl
	    ;

	  exit(EXIT_FAILURE);
	}

      // If row != i, exchange the equations 'row' and 'i'
      if (row != i) {

	// Swap rows of A
	A.swapRows(row, i);

	// Swap elements in b
	b.swapElements(row, i);
      }

      // =====================================
      // Elimination Step
      // -------------------------------------
      for (std::size_t r = i + 1; r < _rows; ++r)
	{
	  // Multiplier
	  double m = - A(r, i) / A(i, i);

	  A(r, i) = 0;
	  
	  for (std::size_t c = i + 1; c < _cols; ++c) 
	    A(r, c) += m * A(i, c);

	  b(r) += m * b(i);
	}
    }
      
  // =====================================
  // Back substitution
  // -------------------------------------
  DVector u(n);
  for(std::size_t i = n; i-- > 0; )
    {
      u(i) = b(i);
      
      for(std::size_t j = i + 1; j < n; ++j)
	u(i) -= A(i , j) * u(j);

      u(i) /= A(i, i);
    }
  
  return u;
}

/**
   Equality operator.
 */
bool operator== (const DMatrix &m1, const DMatrix &m2)
{
  return (m1._rows == m2._rows &&
	  m1._cols == m2._cols &&
	  *m1._v == *m2._v);
}
 
/**
   Inequality operator.
 */
bool operator!= (const DMatrix &m1, const DMatrix &m2)
{
  return !(m1 == m2);
}

/**
   Euclidean distance.
 */
double euclideanDistance(const DMatrix &m1, const DMatrix &m2)
{
  assert(m1.cols() == m2.cols());
  assert(m1.rows() == m2.rows());

  double ed = 0;
  for (std::size_t i = 0; i < m1.size(); ++i) 
    {
      double d = (*m1._v)[i] - (*m2._v)[i];
      ed += d * d;
    }
  
  return sqrt(ed);
}

/**
   Multiplication operator: DMatrix * DVector -> DVector
*/
DVector operator* (const DMatrix &m, const DVector &v)
{
  assert(m.cols() == v.size());

  DVector r(m.rows());
  for (std::size_t row = 0; row < m._rows; ++row)
    {
      double &s = r(row);
      s = 0;
      for (std::size_t col = 0; col < m._cols; ++col)
	s += m(row, col) * v(col);
    }
  
  return r;
}

/**   
      os <<
*/
std::ostream& operator<<(std::ostream& os, const DMatrix& m)
{
  for (std::size_t row = 0; row < m._rows; ++row)
    {
      for (std::size_t col = 0; col < m._cols; ++col)
	{
	  if (col > 0) os << " ";
	  os << m(row, col);
	}
      os << std::endl;
    }

  return os;
}

} // namespace nsl

/* fin */
