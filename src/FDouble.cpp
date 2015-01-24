// -*- mode: C++ -*-
/**
   Dietrich Bollmann, Kamakura, 2015/01/01
   
   FDouble.cpp
   
   Class: FDouble
   
   Copyright (c) 2015 Dietrich Bollmann
   
   This software may be modified and distributed under the terms
   of the MIT license.  See the LICENSE file for details.
*/

#include <iostream>
#include <cassert>

#include "FDouble.h"

namespace nsl {

// =========================================================
// Class FDouble
// ---------------------------------------------------------

FDouble::FDouble() : _defined(false), _value(0.0) {}

FDouble::FDouble(const double value)
{
  set(value);
}

FDouble::FDouble(const FDouble &fdouble) 
{
  _defined = fdouble._defined;
  _value   = fdouble._value;
}

FDouble::~FDouble() {}

// =========================================================
// Accessors
// ---------------------------------------------------------

void FDouble::set(double value)
{
  _defined = true;
  _value   = value;
}

void FDouble::unset()
{
  _defined = false;
  _value   = 0.0;
}

bool FDouble::isDefined() const
{
  return _defined;
}

double FDouble::getValue() const
{
  assert(_defined);
  
  return _value;
}

// =========================================================
// Friends
// ---------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const FDouble &fdouble)
{
  if (fdouble._defined) 
    os << fdouble._value;
  else
    os << "-";

  return os;
}

} // namespace nsl

/* fin */
