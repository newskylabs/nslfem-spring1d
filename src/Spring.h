// -*- mode: C++ -*-
/**
   Dietrich Bollmann, Kamakura, 2015/01/01
   
   Spring.h

   Class: Spring
   
   Copyright (c) 2015 Dietrich Bollmann
   
   This software may be modified and distributed under the terms
   of the MIT license.  See the LICENSE file for details.
*/

#ifndef __Spring__
#define __Spring__

#include <iostream>

#include "Node.h"
#include "DMatrix.h"

namespace nsl {

// =========================================================
// class Spring
// ---------------------------------------------------------

class Spring
{
private:
  const int _index;
  const int _id;
  const Node *_node1;
  const Node *_node2;
  const double _springConstant;
  
public:
  Spring(const int index, const int id, 
	 const Node *node1, const Node *node2, 
	 const double springConstant);
  ~Spring();

  // Accessors
  const Node *getNode1() const;
  const Node *getNode2() const;
  int getIndex() const;
  int getID() const;
  double getSpringConstant() const;
  
  // Methods
  DMatrix stiffnessMatrix();
  void dump();

  // Friends  
  friend std::ostream& operator<<(std::ostream& os, const Spring& spring);
};

} // namespace nsl

#endif /* defined(__Spring__) */

/* fin */
