// -*- mode: C++ -*-
/**
   Dietrich Bollmann, Kamakura, 2015/01/01

   Node.h
   
   Class: Node
   
   Copyright (c) 2015 Dietrich Bollmann
   
   This software may be modified and distributed under the terms
   of the MIT license.  See the LICENSE file for details.
*/

#ifndef __Node__
#define __Node__

#include <iostream>
 
#include "FDouble.h"

namespace nsl {

/**
   Class Node
*/
class Node
{
private:
  int _index; // for internal use
  int _id;    // for external use
  FDouble _displacement;
  double _force;

public:
  Node(const int index, const int id);
  ~Node();

  // Accessors
  int getIndex() const;
  int getID() const;
  FDouble getDisplacement() const;
  double getForce() const;
  
  // Methods
  void addDisplacement(const double displacement);
  void addForce(const double force);

  // Friends  
  friend std::ostream& operator<<(std::ostream& os, const Node& node);
};

} // namespace nsl

#endif /* defined(__Node__) */

/* fin */
