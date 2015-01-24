// -*- mode: C++ -*-
/**
   Dietrich Bollmann, Kamakura, 2015/01/01
   
   DMatrix.h
   
   Class: DMatrix

   Copyright (c) 2015 Dietrich Bollmann
   
   This software may be modified and distributed under the terms
   of the MIT license.  See the LICENSE file for details.
*/

#ifndef __DMatrix__
#define __DMatrix__

#include <cstddef>
#include <iostream>
#include <vector>

#include "DVector.h"
#include "FVector.h"

namespace nsl {

class DMatrix {

  std::vector<double> *_v;
  std::size_t _cols, _rows;

 public:
  
  DMatrix(std::size_t rows = 0, std::size_t cols = 0);
  DMatrix(const std::vector<std::vector<double> > &values);
  DMatrix(const DMatrix &matrix);
  ~DMatrix();

  DMatrix &operator= (const std::vector<double> &values);
  DMatrix &operator= (const std::vector<std::vector<double> > &values);

  std::size_t size() const;
  std::size_t cols() const;
  std::size_t rows() const;

  void resize(std::size_t rows, std::size_t cols);

  double &operator() (std::size_t row, std::size_t column);
  double operator() (std::size_t row, std::size_t column) const;

  void swapRows(const std::size_t i, const std::size_t j);
  void deleteRowsAndColumns(std::function<bool (std::size_t i)> predicate);
  
  DVector gaussianElimination(const DVector &b) const;

  friend bool operator== (const DMatrix &m1, const DMatrix &m2);
  friend bool operator!= (const DMatrix &m1, const DMatrix &m2);
  
  friend double euclideanDistance(const DMatrix &m1, const DMatrix &m2);
  
  friend DVector operator* (const DMatrix &m, const DVector &v);
  
  friend std::ostream& operator<<(std::ostream& os, const DMatrix& m);
};

} // namespace nsl

#endif /* defined(__DMatrix__) */

/* fin */
