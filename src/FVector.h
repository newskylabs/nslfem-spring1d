// -*- mode: C++ -*-
/**
   Dietrich Bollmann, Kamakura, 2015/01/01
   
   FVector.h   
   
   Class: FVector
   
   Copyright (c) 2015 Dietrich Bollmann
   
   This software may be modified and distributed under the terms
   of the MIT license.  See the LICENSE file for details.
*/

#ifndef __FVector__
#define __FVector__

#include <vector>

#include "FDouble.h"

namespace nsl {

class DVector;

// =========================================================
// class FVector
// ---------------------------------------------------------

class FVector {

private:
  std::vector<FDouble> *_v;

public:
  FVector(size_t size = 0);
  FVector(const FVector &v);
  ~FVector();

  size_t size() const;
  size_t numberOfDefinedElements() const;
  void setElement(size_t i, const double value);
  DVector setUndefinedElements(DVector &values);

  FDouble &operator() (size_t i);
  FDouble operator() (size_t i) const;

  // Friends  
  friend std::ostream& operator<<(std::ostream& os, const FVector &fvector);
};

} // namespace nsl

#endif /* defined(__FVector__) */

/* fin */
