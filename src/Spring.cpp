// -*- mode: C++ -*-
/**
   Dietrich Bollmann, Kamakura, 2015/01/01
   
   Spring.cpp

   Class: Spring
   
   Copyright (c) 2015 Dietrich Bollmann
   
   This software may be modified and distributed under the terms
   of the MIT license.  See the LICENSE file for details.
*/

#include <iostream>
#include <cmath>

#include "Node.h"
#include "Spring.h"

namespace nsl {

// =========================================================
// Class Spring
// ---------------------------------------------------------

Spring::Spring(const int index, const int id, 
	       const Node *node1, const Node *node2, 
	       const double springConstant)
  : _index(index), _id(id),
    _node1(node1), _node2(node2),
    _springConstant(springConstant)
{}

Spring::~Spring() {}

// =========================================================
// Accessors
// ---------------------------------------------------------

const Node *Spring::getNode1() const
{
  return _node1;
}

const Node *Spring::getNode2() const
{
  return _node2;
}

int Spring::getIndex() const
{
  return _index;
}

int Spring::getID() const
{
  return _id;
}

double Spring::getSpringConstant() const
{
  return _springConstant;
}

// =========================================================
// Methods
// ---------------------------------------------------------

/**
   Stiffness matrix.
*/
DMatrix Spring::stiffnessMatrix()
{
  double k = _springConstant;

  DMatrix stiffnessMatrix(2, 2);
  stiffnessMatrix = { 
     k, -k, 
    -k,  k
  };

  return stiffnessMatrix;
}

/**
   Dump the internals of the spring.
*/
void Spring::dump()
{
  std::cout 
    << ">>> spring " << _id << ":" << std::endl
    << std::endl
    << *this << std::endl
    << std::endl
    << *_node1 << std::endl
    << *_node2 << std::endl
    << std::endl
    << "stiffness matrix:" << std::endl
    << std::endl
    << stiffnessMatrix() << std::endl
    ;
}

// =========================================================
// Friends
// ---------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const Spring& spring)
{
  std::cout
    << "spring " << spring._id << "  " 
    << spring._node1->getID() << " "
    << spring._node2->getID() << "  "
    << spring._springConstant
    ;

  return os;
}

} // namespace nsl

/* fin */
