// -*- mode: C++ -*-
/**
   Dietrich Bollmann, Kamakura, 2015/01/01
   
   DVector.h

   Class: DVector
   
   Copyright (c) 2015 Dietrich Bollmann
   
   This software may be modified and distributed under the terms
   of the MIT license.  See the LICENSE file for details.
*/

#ifndef __DVector__
#define __DVector__

#include <iostream>
#include <vector>

#include "FVector.h"

namespace nsl {

// =========================================================
// class DVector
// ---------------------------------------------------------

class DVector {

  std::vector<double> *_v;

 public:
  
  DVector(std::size_t size = 0);
  DVector(const std::vector<double> &values);
  DVector(const DVector &vector);
  ~DVector();

  DVector &operator= (const std::vector<double> &values);
  
  std::size_t size() const;

  void resize(std::size_t size);

  void swapElements(const std::size_t i, const std::size_t j);
  void applyBoundaryConditions(const FVector displacementVector);
  void deleteElements(std::function<bool (std::size_t i)> predicate);
  
  double &operator() (std::size_t i);
  double operator() (std::size_t i) const;

  friend bool operator== (const DVector &v1, const DVector &v2);
  friend bool operator!= (const DVector &v1, const DVector &v2);

  friend double euclideanDistance(const DVector &v1, const DVector &v2);
  
  friend double operator* (const DVector &v1, const DVector &v2);
  
  friend std::ostream& operator<<(std::ostream& os, const DVector& v);
};

} // namespace nsl

#endif /* defined(__DVector__) */

/* fin */
