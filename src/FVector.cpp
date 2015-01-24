// -*- mode: C++ -*-
/**
   Dietrich Bollmann, Kamakura, 2015/01/01
   
   FVector.cpp

   Class: FVector
   
   Copyright (c) 2015 Dietrich Bollmann
   
   This software may be modified and distributed under the terms
   of the MIT license.  See the LICENSE file for details.
*/

#include <iostream>
#include <cassert>

#include "FDouble.h"
#include "DVector.h"
#include "FVector.h"

namespace nsl {

// =========================================================
// Class FVector
// ---------------------------------------------------------

/** 
    Constructor.
*/
FVector::FVector(std::size_t size)
{
  _v = new std::vector<FDouble> (size);
}

/** 
    Copy constructor.
*/
FVector::FVector(const FVector &v)
{
  _v = new std::vector<FDouble> (*v._v);
}

/** 
    Destructor.
*/
FVector::~FVector()
{
  delete _v;
}

// =========================================================
// Methods
// ---------------------------------------------------------

/**
   Get size.
 */
std::size_t FVector::size() const
{
  return _v->size();
}

/**
   Get size.
 */
std::size_t FVector::numberOfDefinedElements() const
{
  std::size_t n = 0;
  for (std::size_t i = 0; i < _v->size(); ++i)
    if ((*_v)[i].isDefined()) ++n;

  return n;
}

/**
   Set an element.
 */
void FVector::setElement(std::size_t i, double value)
{
  assert(0 <= i && i < _v->size());

  (*_v)[i].set(value);
}

/**
   Set the undefined values.

   The size of the values vector given as parameter has to to
   correspond to the number of undefined values in the current
   FVector.
 */
DVector FVector::setUndefinedElements(DVector &values)
{
  std::size_t sd  = size();
  std::size_t sdd = numberOfDefinedElements();
  std::size_t sud = values.size();

  // Assert that the size of the values vector given as parameter
  // corresponds to the number of undefined values in the current
  // FVector
  assert(sd == sdd + sud);

  DVector values2(sd);
  std::size_t j = 0;
  for (std::size_t i = 0; i < sd; ++i)
    if ((*_v)[i].isDefined())
      {
	values2(i) = (*_v)[i].getValue();
      }
    else
      {
	values2(i) = values(j);
	++j;
      }

  return values2;
}
  
/**
   Element accessor.
 */
FDouble &FVector::operator() (std::size_t i)
{
  assert(0 <= i && i < _v->size());

  return (*_v)[i];
}

/**
   Constant element accessor.
 */
FDouble FVector::operator() (std::size_t i) const
{
  assert(0 <= i && i < _v->size());

  return (*_v)[i];
}

// =========================================================
// Friends
// ---------------------------------------------------------

/**   
  os <<
*/
std::ostream& operator<<(std::ostream& os, const FVector &fvector)
{
  for (std::size_t i = 0; i < fvector._v->size(); ++i)
    {
      if (i > 0) os << " ";

      FDouble &element = (*fvector._v)[i];
      if (element.isDefined())
	os << element.getValue();
      else
	os << "-";
    }

  return os;
}

} // namespace nsl

/* fin */
