// -*- mode: C++ -*-
/**
   Dietrich Bollmann, Kamakura, 2015/01/01
   
   FEM.h
   
   Class: FEM
   
   Copyright (c) 2015 Dietrich Bollmann
   
   This software may be modified and distributed under the terms
   of the MIT license.  See the LICENSE file for details.
*/

#ifndef __FEM__
#define __FEM__

#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "DVector.h"
#include "DMatrix.h"
#include "FDouble.h"
#include "FVector.h"

namespace nsl {

// =========================================================
// Forward definitions
// ---------------------------------------------------------

class Node;
class Spring;

// =========================================================
// class FEM
// ---------------------------------------------------------

class FEM {
  
private:
  int _dimension = 1; // Working in 1D

  std::vector<Node*>   _nodes;
  std::vector<Spring*> _springs;

  std::map<int, int> _nodeIndexToIDMap;
  std::map<int, int> _nodeIDToIndexMap;
  std::map<int, int> _springIndexToIDMap;
  std::map<int, int> _springIDToIndexMap;

  DVector *_globalForceVector        = nullptr;
  DMatrix *_globalStiffnessMatrix    = nullptr;
  DVector *_globalDisplacementVector = nullptr;
  
public:
  FEM();
  FEM(const std::vector<std::string> &files);
  ~FEM();
  
public:
  void addNode(const int id);
  void addDisplacement(const int nodeID, const double displacement);
  void addForce(const int nodeID, const double force);
  void addSpring(const int id,
		 const int node1, const int node2, 
		 const double springConstant);

  DMatrix getGlobalStiffnessMatrix();
  DVector getGlobalDisplacementVector();
  double getGlobalDisplacement(const int nodeID);
  DVector getGlobalForceVector();
  double getGlobalForce(const int nodeID);
  DVector getLocalForces(const int springID);
  
  void solve();
  void printResults();
  
private:
  void applyBoundaryConditions(DVector &globalForceVector,
			       DMatrix &globalStiffnessMatrix, 
			       FVector &globalDisplacementVector);

  std::vector<Node*> &getNodes();
  Node *getNodeByIndex(const int i);
  Node *getNodeByID(const int id);
  int getNumberOfNodes();

  std::vector<Spring*> &getSprings();
  Spring *getSpringByIndex(const int i);
  Spring *getSpringByID(const int id);
  int getNumberOfSprings();

  int degreesOfFreedom();

  DMatrix assembleGlobalStiffnessMatrix();
  FVector assembleGlobalDisplacementVector();
  DVector assembleGlobalForceVector();

  void printGlobalDisplacements();
  void printGlobalForces();
  void printLocalForcesAtEachElement();
  
  friend std::ostream& operator<<(std::ostream& os, const FEM& fgfem);
};

} // namespace nsl

#endif /* defined(__FEM__) */

/* fin */
