// -*- mode: C++ -*-
/**
   Dietrich Bollmann, Kamakura, 2015/01/01
   
   DVector.cpp

   Class: DVector
   
   Copyright (c) 2015 Dietrich Bollmann
   
   This software may be modified and distributed under the terms
   of the MIT license.  See the LICENSE file for details.
*/

#include <vector>
#include <cassert>
#include <cmath>

#include "DVector.h"

namespace nsl {

/** 
    Constructor.
*/
DVector::DVector(std::size_t size) 
{
  _v = new std::vector<double> (size);
}

/** 
    Constructor.
*/
DVector::DVector(const std::vector<double> &values)
{
  _v = new std::vector<double> (values);
}

/** 
    Constructor.
*/
DVector::DVector(const DVector &v)
{
  _v = new std::vector<double> (*(v._v));
}

/** 
    Destructor.
*/
DVector::~DVector() 
{
  delete _v;
}

/**
   = operator for initialization.
 */
DVector &DVector::operator= (const std::vector<double> &values)
{
  std::size_t size = values.size();
  this->resize(size);
  
  for (std::size_t i = 0; i < size; ++i)
    (*this)(i) = values[i];
  
  return *this;
}

/**
   Number of elements.
*/
std::size_t DVector::size() const
{
  return _v->size();
}

/**
   Resize the vector.
*/
void DVector::resize(std::size_t size)
{ 
  _v->resize(size);
}

/**
   Swap elements.
 */
void DVector::swapElements(const std::size_t i, const std::size_t j)
{
  assert(i != j);
  assert(i < size());
  assert(j < size());
  
  double tmp = (*this)(i);
  (*this)(i) = (*this)(j);
  (*this)(j) = tmp;
}

/**
   Apply boundary conditions.
*/
void DVector:: applyBoundaryConditions(const FVector displacementVector)
{ 
  std::size_t sizeDisplacementVector = displacementVector.size();

  // The displacement vector has to be the same size as the current vector
  assert(sizeDisplacementVector == size());

  // Size of the new vector
  // All elements for which a displacement is defined are removed
  std::size_t sizeNew = sizeDisplacementVector - displacementVector.numberOfDefinedElements();
  std::vector<double> *v = new std::vector<double> (sizeNew);
  
  std::size_t j = 0;
  for (std::size_t i = 0; i < size(); ++i)
    // Skip elements for which a displacement is defined
    if (!displacementVector(i).isDefined())
      (*v)[j++] = (*_v)[i];
  
  // Use the new vector
  delete _v;
  _v = v;
}

/**
   Remove elements for which predicate(<element index>) is true.
 */
void DVector::deleteElements(std::function<bool (std::size_t i)> predicate)
{
  // Calculate the size of the new vector.
  // The number of elements in the new vector
  // corresponds to the number of elements in the old vector
  // for which the predicate(<index of row/column>) is false.
  std::size_t sizeNew = 0;
  for (std::size_t i = 0; i < size(); ++i)
    if (!predicate(i)) 
      ++sizeNew;

  // Assemble the new vector
  // using only those elements for which the predicate(<index of row/column>) is false
  std::vector<double> *v = new std::vector<double> (sizeNew);
  std::size_t j = 0;
  for (std::size_t i = 0; i < size(); ++i)
    // Only use the elements for which predicate(i) is false
    if (!predicate(i))
      (*v)[j++] = (*_v)[i];
  
  // Use the new vector
  delete _v;
  _v = v;
}

/**
   Element accessor.
 */
double &DVector::operator() (std::size_t i)
{
  assert(0 <= i && i < _v->size());

  return (*_v)[i];
}

/**
   Constant element accessor.
 */
double DVector::operator() (std::size_t i) const
{
  assert(0 <= i && i < _v->size());

  return (*_v)[i];
}

/**
   Equality operator.
 */
bool operator== (const DVector &v1, const DVector &v2)
{
  return (*v1._v == *v2._v);
}
 
/**
   Inequality operator.
 */
bool operator!= (const DVector &v1, const DVector &v2)
{
  return !(v1 == v2);
}

/**
   Euclidean distance.
 */
double euclideanDistance(const DVector &v1, const DVector &v2)
{
  assert(v1.size() == v2.size());

  double ed = 0;
  for (std::size_t i = 0; i < v1.size(); ++i) 
    {
      double d = v1(i) - v2(i);
      ed += d * d;
    }
  
  return sqrt(ed);
}
  
/**
   Multiplication operator: DVector * DVector -> DVector
 */
double operator* (const DVector &v1, const DVector &v2)
{
  // Assert that both vectors have the same dimension
  assert(v1.size() == v2.size());

  // Multiply
  double result = 0;
  for (std::size_t i = 0; i < v1.size(); ++i)
    result += v1(i) * v2(i);
  
  return result;
}

/**   
  os <<
*/
std::ostream& operator<<(std::ostream& os, const DVector& m)
{
  for (std::size_t i = 0; i < m.size(); ++i)
    {
      if (i > 0) os << " ";
      os << m(i);
    }

  return os;
}

} // namespace nsl

/* fin */
