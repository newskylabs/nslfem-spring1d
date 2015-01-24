// -*- mode: C++ -*-
/**
   Dietrich Bollmann, Kamakura, 2015/01/01
   
   FDouble.h
   
   Class: FDouble
   
   Copyright (c) 2015 Dietrich Bollmann
   
   This software may be modified and distributed under the terms
   of the MIT license.  See the LICENSE file for details.
*/

#ifndef __FDouble__
#define __FDouble__

namespace nsl {

// =========================================================
// class FDouble
// ---------------------------------------------------------

class FDouble {

private:
  bool _defined;
  double _value;

public:
  FDouble();
  FDouble(const double value);
  FDouble(const FDouble &value);
  ~FDouble();

  // Accessors
  void set(double value);
  void unset();
  bool isDefined() const;
  double getValue() const;

  // Friends  
  friend std::ostream& operator<<(std::ostream& os, const FDouble &fdouble);
};

} // namespace nsl

#endif /* defined(__FDouble__) */

/* fin */
