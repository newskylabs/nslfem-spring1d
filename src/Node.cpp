// -*- mode: C++ -*-
/**
   Dietrich Bollmann, Kamakura, 2015/01/01
   
   DMatrix.h
   
   Class: Node
   
   Copyright (c) 2015 Dietrich Bollmann
   
   This software may be modified and distributed under the terms
   of the MIT license.  See the LICENSE file for details.
*/

#include <iostream>

#include "Node.h"

namespace nsl {

// =========================================================
// Class Node
// ---------------------------------------------------------

Node::Node(const int index, const int id)
  : _index(index), _id(id), _force(0.0) {}

Node::~Node() {}

// =========================================================
// Accessors
// ---------------------------------------------------------

/**
   Get index.
*/
int Node::getIndex() const 
{
  return _index;
}

/**
   Get ID.
*/
int Node::getID() const 
{
  return _id;
}

/**
   Get displacement.
*/
FDouble Node::getDisplacement() const 
{
  return _displacement;
}

/**
   Get force.
*/
double Node::getForce() const 
{
  return _force;
}

// =========================================================
// Methods
// ---------------------------------------------------------

/**
   Add a displacement.
*/
void Node::addDisplacement(const double displacement) 
{
  _displacement.set(displacement);
}

/**
   Add a force.
*/
void Node::addForce(const double force)
{
  _force = force;
}

// =========================================================
// Friends
// ---------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const Node& node)
{
  os << "node " << node._id;
  if (node._displacement.isDefined()) os << "  d " << node._displacement.getValue();
  if (node._force != 0)               os << "  f " << node._force;

  return os;
}

} // namespace nsl

/* fin */
