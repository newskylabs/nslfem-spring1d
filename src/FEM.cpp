// -*- mode: C++ -*-
/**
   Dietrich Bollmann, Kamakura, 2015/01/01
   
   FEM.cpp
   
   Class: FEM
   
   Copyright (c) 2015 Dietrich Bollmann
   
   This software may be modified and distributed under the terms
   of the MIT license.  See the LICENSE file for details.
*/

#include <string>
#include <map>
#include <iostream>

#include "Parser.h"
#include "Node.h"
#include "Spring.h"

#include "FEM.h"

namespace nsl {

// =========================================================
// Class FEM
// ---------------------------------------------------------

FEM::FEM() {}

FEM::FEM(const std::vector<std::string> &files)
{
  // Parse the definition files
  // and create the finite element model
  Parser parser(this, files);
  parser.parse();
}

FEM::~FEM()
{
  // Delete all springs
  for (auto spring : _springs)
    delete spring;
  _springs.clear();
  
  // Delete all nodes
  for (auto node : _nodes)
    delete node;
  _nodes.clear();

  // Clear the indices
  _springIndexToIDMap.clear();
  _springIDToIndexMap.clear();
  _nodeIndexToIDMap.clear();
  _nodeIDToIndexMap.clear();
  
  // Delete force vector, stiffness matrix
  if (_globalForceVector) delete _globalForceVector;
  if (_globalStiffnessMatrix) delete _globalStiffnessMatrix;
  if (_globalDisplacementVector) delete _globalDisplacementVector;
}

// =========================================================
// Methods
// ---------------------------------------------------------

DVector FEM::getGlobalForceVector()
{
  return *_globalForceVector;
}

double FEM::getGlobalForce(const int nodeID)
{
  // Get index of node
  int i = _nodeIDToIndexMap[nodeID];

  // Return the corresponding force
  return (*_globalForceVector)(i);
}

DMatrix FEM::getGlobalStiffnessMatrix()
{
  return *_globalStiffnessMatrix;
}

DVector FEM::getGlobalDisplacementVector()
{
  return *_globalDisplacementVector;
}

double FEM::getGlobalDisplacement(const int nodeID)
{
  // Get index of node
  int i = _nodeIDToIndexMap[nodeID];

  // Return the corresponding force
  return (*_globalDisplacementVector)(i);
}
  
/**
   Solve the finite element model.
*/
void FEM::solve()
{
  // =====================================
  // Assemble the FEM model
  // -------------------------------------
  
  // Assemble the global force vector
  DVector globalForceVector = assembleGlobalForceVector();

  // Assemble the global stiffness matrix
  DMatrix globalStiffnessMatrix = assembleGlobalStiffnessMatrix();

  // Store a copy of the global stiffness matrix for later use
  _globalStiffnessMatrix = new DMatrix(globalStiffnessMatrix);

  // Assemble the global displacement vector
  FVector globalDisplacementVector = assembleGlobalDisplacementVector();

  // =====================================
  // Apply boundary conditions
  // -------------------------------------

  // Apply the boundary conditions
  applyBoundaryConditions(globalForceVector, globalStiffnessMatrix, globalDisplacementVector);

  // Calculate the unconstrained global displacements 
  // by solving the partitioned stiffness matrix and force vector
  DVector unconstrainedDisplacements = globalStiffnessMatrix.gaussianElimination(globalForceVector);
  
  // Add the calculated global displacements to the original displacement vector
  _globalDisplacementVector = new DVector(globalDisplacementVector.setUndefinedElements(unconstrainedDisplacements));
  
  // =====================================
  // Solve the original displacement matrix 
  // by applying the calculated global displacements
  // -------------------------------------
  _globalForceVector = new DVector(*_globalStiffnessMatrix * *_globalDisplacementVector);
}

/**
   Apply boundary conditions
*/
void FEM::applyBoundaryConditions(DVector &globalForceVector,
				  DMatrix &globalStiffnessMatrix, 
				  FVector &globalDisplacementVector)
{
  // A lambda function
  // which returns true for elements for which a displacement is defined
  auto displacementDefined = [&globalDisplacementVector] (std::size_t i) -> bool { 
    return globalDisplacementVector(i).isDefined(); 
  };
  
  // Bring the values in those columns which correspond to a
  // given displacement to the left side by muliplying them
  // with the corresponding displacement and subtracting
  // their sum from the given force on the left side:
  for (std::size_t i = 0; i < globalDisplacementVector.size(); ++i)
    { 
      // The displacement has to be calculated 
      // only for the rows for which no displacement has been given already:
      if (!displacementDefined(i))
	{
	  double &f = globalForceVector(i);
	  for (std::size_t j = 0; j < globalDisplacementVector.size(); ++j)
	    if (displacementDefined(j))
	      f -= globalStiffnessMatrix(i, j) * globalDisplacementVector(j).getValue();
	}
    }

  // Remove rows and columns for which displacements are defined
  // i.e. for which displacementDefined(<index of row / column>) is true
  globalStiffnessMatrix.deleteRowsAndColumns(displacementDefined);
  
  // Remove elements for which displacements are defined
  // i.e. for which displacementDefined(<index of element>) is true
  globalForceVector.deleteElements(displacementDefined);
}
    
/**
   Add a node.
*/
void FEM::addNode(const int id)
{
  // Check if a node with the same index has been defined already
  if ( _nodeIDToIndexMap.find(id) != _nodeIDToIndexMap.end() )
    {
      std::cerr << "ERROR A node with index " << id << " has been defined already!" << std::endl;
      exit(EXIT_FAILURE);
    }
  
  // Calculate the internal index of the node
  int index = _nodes.size();
  
  // Add entries to the external/internal node indices
  _nodeIDToIndexMap[id] = index;
  _nodeIndexToIDMap[index] = id;
  
  // Add node
  Node *node = new Node(index, id);
  _nodes.push_back(node);
}

/**
   Add a displacement.
*/
void FEM::addDisplacement(const int nodeID, const double displacement)
{
  // Get node object
  Node *node = getNodeByID(nodeID);

  // Add displacement
  node->addDisplacement(displacement);
}

/**
   Add a force.
*/
void FEM::addForce(const int nodeID, const double force)
{
  // Get node object
  Node *node = getNodeByID(nodeID);

  // Add displacement
  node->addForce(force);
}

/**
   Get the nodes.
*/
std::vector<Node*>  &FEM::getNodes()
{
  return _nodes;
}

/**
   Get the node with the given internal id.
*/
Node *FEM::getNodeByIndex(const int i)
{
  if (i >= _nodes.size())
    {
      std::cerr << "ERROR A node with index " << i << " does not yet exist!" << std::endl;
      exit(EXIT_FAILURE);
    }

  return _nodes[i];
}

/**
   Get the node with the given external id.
*/
Node *FEM::getNodeByID(const int id)
{
  std::map<int, int>::const_iterator it = _nodeIDToIndexMap.find(id);
  if (it == _nodeIDToIndexMap.end())
    {
      std::cerr << "ERROR A node with id " << id << " does not yet exist!" << std::endl;
      exit(EXIT_FAILURE);
    }

  return getNodeByIndex(it->second);
}

/**
   Get the number of nodes.
*/
int FEM::getNumberOfNodes()
{
  return _nodes.size();
}

/**
   Add a spring.
*/
void FEM::addSpring(const int id,
		      const int node1, const int node2, 
		      const double springConstant)
{
  // Check if a spring with the same index has been defined already
  if ( _springIDToIndexMap.find(id) != _springIDToIndexMap.end() )
    {
      std::cerr << "ERROR An spring with index " << id << " has been defined already!" << std::endl;
      exit(EXIT_FAILURE);
    }
  
  // Get nodes
  Node *nodep1 = getNodeByID(node1);
  Node *nodep2 = getNodeByID(node2);
  
  // Exit if nodep1 has not been defined
  if (nodep1 == nullptr) {
    std::cerr << "ERROR node(" << node1 << ") not found!" << std::endl;
    exit (EXIT_FAILURE);
  }
  
  // Exit if nodep2 has not been defined
  if (nodep2 == nullptr) {
    std::cerr << "ERROR node(" << node2 << ") not found!" << std::endl;
    exit (EXIT_FAILURE);
  }
  
  // Calculate the internal index of the spring
  int index = _springs.size();
  
  // Add entries to the external/internal spring indices
  _springIDToIndexMap[id] = index;
  _springIndexToIDMap[index] = id;
  
  // Add spring
  Spring *spring = new Spring(index, id, nodep1, nodep2, springConstant);
  _springs.push_back(spring);
}

/**
   Get the springs.
*/
std::vector<Spring*> &FEM::getSprings()
{
  return _springs;
}

/**
   Get the spring with the given internal id.
*/
Spring *FEM::getSpringByIndex(const int i)
{
  return _springs[i];
}

/**
   Get the spring with the given external id.
*/
Spring *FEM::getSpringByID(const int id)
{
  std::map<int, int>::const_iterator it = _springIDToIndexMap.find(id);
  if (it == _springIDToIndexMap.end())
    return nullptr;
  else
    return getSpringByIndex(it->second);
}

/**
   Get the number of springs.
*/
int FEM::getNumberOfSprings()
{
  return _springs.size();
}

/**
   Calculate the degrees of freedom.
*/
int FEM::degreesOfFreedom()
{
  // Calculate the degrees of freedom (corresponding to the size of
  // the global stiffness matrix) by multiplying the number of nodes
  // with the dimension we are using (1D)
  return _dimension * getNumberOfNodes();
}

/**
   Calculate the global stiffness matrix.
*/
DMatrix FEM::assembleGlobalStiffnessMatrix()
{
  // The size of the global stiffness matrix
  // is equal to the degrees of freedom
  // (Number of nodes * dimension of space (1 as we are in 1D))
  int dimension = degreesOfFreedom();
  
  // The global stiffness matrix
  DMatrix globalStiffnessMatrix(dimension, dimension);

  // Calculating the global stiffness matrix
  // by summing the spring stiffness matrices
  for (const auto& spring : getSprings())
    {
      // Get the indices of the nodes of the spring
      int nodeIndex1 = spring->getNode1()->getIndex();
      int nodeIndex2 = spring->getNode2()->getIndex();
      
      // Get the stiffness matrix of the spring
      DMatrix springStiffnessMatrix = spring->stiffnessMatrix();
      
      // Array of global indices
      int gOffset[2] = {
      	nodeIndex1,
      	nodeIndex2
      };
  
      // Number of nodes per spring
      int numberOfNodesPerSpring = 2;
  
      // (0, 0) => add the upper  left value of the spring matrix
      // (0, 1) => add the upper right value of the spring matrix
      // (1, 0) => add the lower  left value of the spring matrix
      // (1, 1) => add the lower right value of the spring matrix
      for (int i = 0; i < numberOfNodesPerSpring; ++i)
      	for (int j = 0; j < numberOfNodesPerSpring; ++j)
  	  {
	    int gRow = gOffset[i];
	    int gColumn = gOffset[j];
  	      
	    int lRow = i;
	    int lColumn = j;
  	      
	    // Get value of the spring stiffness matrix
	    double value = springStiffnessMatrix(lRow, lColumn);

	    // Add value to the global stiffness matrix
	    globalStiffnessMatrix(gRow, gColumn) += value;
  	  }
    }

  // Return the calculated global stiffness matrix
  return globalStiffnessMatrix;
}

/**
   Calculate the global displacement vector.
*/
FVector FEM::assembleGlobalDisplacementVector()
{
  // The size of the displacement vector
  // is equal to the degrees of freedom
  // (Number of nodes * dimension of space (1 as we are in 1D))
  int dimension = degreesOfFreedom();
  
  // The global displacement vector
  FVector globalDisplacementVector(dimension);
  
  // Calculating the displacement vector
  // from the spring displacements
  for (std::size_t i = 0; i < _nodes.size(); ++i)
    globalDisplacementVector(i) = _nodes[i]->getDisplacement();
  
  // Return the displacement vector
  return globalDisplacementVector;
}

/**
   Calculate the force vector.
*/
DVector FEM::assembleGlobalForceVector()
{
  // The size of the force vector
  // is equal to the degrees of freedom
  // (Number of nodes * dimension of space (1 as we are in 1D))
  int dimension = degreesOfFreedom();
  
  // The force vector
  DVector globalForceVector(dimension);
  
  // Calculating the force vector
  // from the spring forces
  for (std::size_t i = 0; i < _nodes.size(); ++i)
    globalForceVector(i) = _nodes[i]->getForce();
  
  // Return the force vector
  return globalForceVector;
}

/**
   Get the local forces.
*/
DVector FEM::getLocalForces(const int springID)
{
  Spring *spring = getSpringByID(springID);
  
  // Get the indices of the nodes of the spring
  int node1 = spring->getNode1()->getIndex();
  int node2 = spring->getNode2()->getIndex();
  
  // Extract the displacements of the nodes of the current spring
  const DVector &displacements = *_globalDisplacementVector;
  DVector displacements2({ displacements(node1), displacements(node2) });
  
  // Get the stiffness matrix of the spring
  DMatrix stiffnessMatrix = spring->stiffnessMatrix();
  
  // Calculate the forces at the current spring
  DVector forces = stiffnessMatrix * displacements2;
  
  return forces;
}

/**
   Print the results.
*/
void FEM::printResults()
{
  printGlobalDisplacements();
  printGlobalForces();
  printLocalForcesAtEachElement();
}

/**
   Print the node displacements.
*/
void FEM::printGlobalDisplacements()
{
  std::cout << "Global displacements:" << std::endl << std::endl;
  for (const auto& node : getNodes())
    {
      // Get the id of the node
      int id = node->getID();

      // Get the node displacement
      double displacement = getGlobalDisplacement(id);

      std::cout << "  - node " << id << ": " << displacement << std::endl;
    }
  std::cout << std::endl;
}

/**
   Print the node forces.
*/
void FEM::printGlobalForces()
{
  std::cout << "Global forces:" << std::endl << std::endl;
  for (const auto& node : getNodes())
    {
      // Get the id of the node
      int id = node->getID();

      // Get the force at the given node 
      double force = getGlobalForce(id);

      std::cout << "  - node " << id << ": " << force << std::endl;
    }
  std::cout << std::endl;
}

/**
   Calculate and print the local forces at each element.
*/
void FEM::printLocalForcesAtEachElement()
{
  std::cout << "Local forces at each element:" << std::endl << std::endl;
  for (const auto& spring : getSprings())
    {
      // Get the id of the spring
      int id = spring->getID();

      // Get local forces at the spring
      DVector forces = getLocalForces(id);

      std::cout << "  - element " << id << ": (" << forces(0) << ", " << forces(1) << ")" << std::endl;
    }
  std::cout << std::endl;
}

// =========================================================
// Friends
// ---------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const FEM& fgfem)
{
  // Print nodes
  os << "// Nodes" << std::endl;
  for (const auto& node : fgfem._nodes)
    os << *node << std::endl;
  os << std::endl;

  // Print springs
  os << "// Springs" << std::endl;
  for (const auto& spring : fgfem._springs)
    os << *spring << std::endl;
  os << std::endl;

  return os;
}

} // namespace nsl

/* fin */
