// -*- mode: C++ -*-
/**
   Dietrich Bollmann, Kamakura, 2015/01/01
   
   FEM-test.h

   Unit tests for class: FEM
   
   Copyright (c) 2015 Dietrich Bollmann
   
   This software may be modified and distributed under the terms
   of the MIT license.  See the LICENSE file for details.
*/

#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif

#include <boost/test/unit_test.hpp>

#include <cmath>

#include "Spring.h"

#include "FEM.h"

namespace nsl {

BOOST_AUTO_TEST_SUITE(TestSuite_FEM)

/**
   Add node.
*/ 
void addNode(FEM &fem, std::vector<double> node)
{
  // Add node
  int id = node[0];
  fem.addNode(id);

  for (size_t i = 1; i < node.size() - 1; i += 2)
    {
      // Add displacement, when given
      if (node[i] == 'd')
	{
	  double displacement = node[i+1];
	  fem.addDisplacement(id, displacement);
	}
      
      // Add force, when given
      if (node[i] == 'f')
	{
	  double force = node[i+1];
	  fem.addForce(id, force);
	}
    }
}
		    
/**
  Add spring.
 */
void addSpring(FEM &fem, std::vector<double> spring)
{
  // Unpack spring data
  int id = spring[0];
  int node1 = spring[1];
  int node2 = spring[2];
  double springConstant = spring[3];

  // Add spring
  fem.addSpring(id, node1, node2, springConstant);
}

/**
   Test stiffness matrix.
 */
void testStiffnessMatrix(std::vector<std::vector<double> > nodes, 
			 std::vector<std::vector<double> > springs, 
			 std::vector<std::vector<double> > stiffnessMatrix)
{
  FEM fem;
 
  // Assemble model
  for (auto node : nodes) addNode(fem, node);
  for (auto spring : springs) addSpring(fem, spring);

  // Solve model
  fem.solve();

  DMatrix expectedStiffnessMatrix(stiffnessMatrix);
  DMatrix calculatedStiffnessMatrix = fem.getGlobalStiffnessMatrix();

  double distance = euclideanDistance(expectedStiffnessMatrix, calculatedStiffnessMatrix);
  bool testResult = distance < 1e-20;
      
  BOOST_CHECK( testResult );
        
  if (!testResult)
    std::cout 
      << ">>> Stiffness matrix:" << std::endl << std::endl
      << "expected:"   << std::endl << expectedStiffnessMatrix << std::endl
      << "calculated:" << std::endl << calculatedStiffnessMatrix << std::endl
      << "euclidian distance: " << distance << std::endl
      ;
}
  
BOOST_AUTO_TEST_CASE(Test_stiffnessMatrix) 
{
  // Test case 1
  // 
  // Taken from the book 
  // "A First Course in the Finite element Method, third edition"
  // by Daryl L. Logan,
  // Example 2.1, page 40.
  // 
  testStiffnessMatrix(// Nodes:
		      // node id ['d', <displacement>] ['f', <force>]
		      {{1, 'd', 0},
		       {2, 'd', 0},
		       {3},
		       {4, 'f', 5000}
		      },
		      // Springs:
		      // spring id  node1 node2  spring constant
		      {{1,  1, 3,  1000},
		       {2,  3, 4,  2000},
		       {3,  4, 2,  3000}
		      },
		      // Stiffness matrix:
		      {{ 1000,     0, -1000,     0},
		       {    0,  3000,     0, -3000},
		       {-1000,     0,  3000, -2000},
		       {    0, -3000, -2000,  5000},
		      }
		      );
  
  // Test case 2
  // 
  // Taken from the book 
  // "A First Course in the Finite element Method, third edition"
  // by Daryl L. Logan,
  // Example 2.2, page 43.
  // 
  testStiffnessMatrix(// Nodes:
		      // node id ['d', <displacement>] ['f', <force>]
		      {{1, 'd', 0},
		       {2},
		       {3},
		       {4},
		       {5, 'd', 0.02}
		      },
		      // Springs:
		      // spring id  node1 node2  spring constant
		      {{1,  1, 2,  200},
		       {2,  2, 3,  200},
		       {3,  3, 4,  200},
		       {4,  4, 5,  200}
		      },
		      // Stiffness matrix:
		      {{ 200, -200,    0,    0,    0},
		       {-200,  400, -200,    0,    0},
		       {   0, -200,  400, -200,    0},
		       {   0,    0, -200,  400, -200},
		       {   0,    0,    0, -200,  200},
		      }
		      );

  // Test case 3  
  // 
  // Taken from the book 
  // "A First Course in the Finite element Method, third edition"
  // by Daryl L. Logan,
  // Example 2.3, page 46.
  // 
  testStiffnessMatrix(// Nodes:
		      // node id ['d', <displacement>] ['f', <force>]
		      {{1, 'd', 0},
           	       {2},
           	       {3, 'd', 0},
           	       {4, 'd', 0}
           	      },
		      // Springs:
		      // spring id  node1 node2  spring constant
           	      {{1,  1, 2,  1},
           	       {2,  2, 3,  2},
           	       {3,  2, 4,  3}
           	      },
		      // Stiffness matrix:
		      {{ 1,    -1,  0,  0},
		       {-1, 1+2+3, -2, -3},
		       { 0,    -2,  2,  0},
		       { 0,    -3,  0,  3}
		      }
		      );

  // Test case 4
  // 
  // Taken from the book 
  // "A First Course in the Finite element Method, third edition"
  // by Daryl L. Logan,
  // Example 2.4, page 54.
  // 
  testStiffnessMatrix(// Nodes:
		      // node id ['d', <displacement>] ['f', <force>]
		      {{1, 'd', 0},
		       {2, 'f', 1000}
           	      },
		      // Springs:
		      // spring id  node1 node2  spring constant
           	      {{1,  1, 2,  500}
           	      },
		      // Stiffness matrix:
		      {{ 500, -500},
		       {-500,  500}
		      }
		      );
}

/**
   Test case.
 */
void testCase(std::vector<std::vector<double> > nodes, 
	      std::vector<std::vector<double> > springs, 
	      std::vector<std::vector<double> > displacements,
	      std::vector<std::vector<double> > forces,
	      std::vector<std::vector<double> > localForces)
{
  FEM fem;
 
  // Assemble model
  for (auto node : nodes) addNode(fem, node);
  for (auto spring : springs) addSpring(fem, spring);

  // Solve model
  fem.solve();

  // Check displacements
  for (auto displacementData : displacements)
    {
      int node = displacementData[0];
      double expectedDisplacement = displacementData[1];
      double displacement = fem.getGlobalDisplacement(node);

      bool testResult = (expectedDisplacement == displacement);

      BOOST_CHECK( testResult );
        
      if (!testResult)
	std::cout 
	  << ">>> Displacement at node " << node << ": "
	  << "expected: " << expectedDisplacement << ", "
	  << "calculated: " << displacement << std::endl
	  ;
    }

  // Check forces
  for (auto forceData : forces)
    {
      int node = forceData[0];
      double expectedForce = forceData[1];
      double force = fem.getGlobalForce(node);

      bool testResult = abs(expectedForce - force) < 1e-12;

      BOOST_CHECK( testResult );
        
      if (!testResult)
	std::cout 
	  << ">>> Force at node " << node << ": "
	  << "expected: " << expectedForce << ", "
	  << "calculated: " << force << std::endl
	  ;
    }

  // Check localForces
  for (auto localForceData : localForces)
    {
      int node = localForceData[0];
      DVector expectedLocalForces({localForceData[1], localForceData[2]});
      DVector localForces = fem.getLocalForces(node);

      double distance = euclideanDistance(expectedLocalForces, localForces);
      bool testResult = distance < 1e-12;
      
      BOOST_CHECK( testResult );
        
      if (!testResult)
      	std::cout 
      	  << ">>> Local forces at node " << node << ": "
      	  << "expected: " << expectedLocalForces << ", "
      	  << "calculated: " << localForces << "  "
      	  << "(euclidian distance: " << distance << ")" << std::endl
      	  ;
    }
}

BOOST_AUTO_TEST_CASE(Test_testCases)
{
  // Test case 1
  // 
  // Taken from the book 
  // "A First Course in the Finite element Method, third edition"
  // by Daryl L. Logan,
  // Example 2.1, page 40.
  // 
  testCase(// Nodes:
  	   // node id ['d', <displacement>] ['f', <force>]
	   {{1, 'd', 0},
	    {2, 'd', 0},
	    {3},
	    {4, 'f', 5000}
  	   },
  	   // Springs:
  	   // spring id  node1 node2  spring constant
  	   {{1,  1, 3,  1000},
            {2,  3, 4,  2000},
  	    {3,  4, 2,  3000}
  	   },
	   // Displacements:
	   // node id  displacement
	   {{1, 0},
	    {2, 0},
	    {3, 10.0 / 11.0},
	    {4, 15.0 / 11.0}
	   },
	   // Forces:
	   // node id  force
	   {{1, -10000.0 / 11},
	    {2, -45000.0 / 11},
	    {3,      0.0},
	    {4,  55000.0 / 11}
	   },
	   // Local forces:
	   // spring id  local force node1, node2
	   {{1, -10000.0 / 11,  10000.0 / 11},
	    {2, -10000.0 / 11,  10000.0 / 11},
	    {3,  45000.0 / 11, -45000.0 / 11}
	   }
  	   );
  
  // Test case 2
  // 
  // Taken from the book 
  // "A First Course in the Finite element Method, third edition"
  // by Daryl L. Logan,
  // Example 2.2, page 43.
  // 
  testCase(// Nodes:
	   // node id ['d', <displacement>] ['f', <force>]
	   {{1, 'd', 0},
	    {2},
	    {3},
	    {4},
	    {5, 'd', 0.02}
	   },
	   // Springs:
	   // spring id  node1 node2  spring constant
	   {{1,  1, 2,  200},
	    {2,  2, 3,  200},
	    {3,  3, 4,  200},
	    {4,  4, 5,  200}
	   },
	   // Displacements:
	   // node id  displacement
	   {{1, 0},
	    {2, 0.005},
	    {3, 0.01},
	    {4, 0.015},
	    {5, 0.02}
	   },
	   // Forces:
	   // node id  force
	   {{1, -1},
	    {2,  0},
	    {3,  0},
	    {4,  0},
	    {5,  1}
	   },
	   // Local forces:
	   // spring id  local force node1, node2
	   {{1, -1, 1},
	    {2, -1, 1},
	    {3, -1, 1},
	    {4, -1 ,1}
	   }
  	   );
  
  // Test case 3
  // 
  // Taken from the book 
  // "A First Course in the Finite element Method, third edition"
  // by Daryl L. Logan,
  // Example 2.3, page 46.
  // 
  testCase(// Nodes:
	   // node id ['d', <displacement>] ['f', <force>]
	   {{1, 'd', 0},
	    {2, 'f', 6}, // Force: P = 6
	    {3, 'd', 0},
	    {4, 'd', 0}
	   },
	   // Springs:
	   // spring id  node1 node2  spring constant
	   {{1,  1, 2,  1}, // Spring constant:  k1 = 1
	    {2,  2, 3,  2}, // Spring constant:  k2 = 2
	    {3,  2, 4,  3}  // Spring constant:  k3 = 3
	   },
	   // Displacements:
	   // node id  displacement
	   {{1, 0},
	    {2, 1}, // P / (k1 + k2 + k3) = 6 / (1 + 2 + 3) = 1
	    {3, 0},
	    {4, 0}
	   },
	   // Forces:
	   // node id  force
	   // d2 = P / (k1 + k2 + k3) = 6 / (1 + 2 + 3) = 1
	   {{1, -1}, // F1 = -k1 * d2 = -k1 = -1
	    {2,  6}, // F2 = P = 6
	    {3, -2}, // F3 = -k2 * d2 = -k2 = -2
	    {4, -3}  // F4 = -k3 * d2 = -k3 = -3
	   },
	   // Local forces:
	   // spring id  local force node1, node2
	   // d2 = P / (k1 + k2 + k3) = 6 / (1 + 2 + 3) = 1
	   {{1, -1,  1}, // k1 * d1 + -k1 * d2 = 1 * 0 + -1 * 1 = -1
	    {2,  2, -2}, // k2 * d2 + -k2 * d3 = 2 * 1 + -2 * 0 =  2
	    {3,  3, -3}  // k3 * d2 + -k3 * d4 = 3 * 1 + -3 * 0 =  3
	   }
  	   );
  
  // Test case 4
  // 
  // Taken from the book 
  // "A First Course in the Finite element Method, third edition"
  // by Daryl L. Logan,
  // Example 2.4, page 54.
  // 
  testCase(// Nodes:
	   // node id ['d', <displacement>] ['f', <force>]
	   {{1, 'd', 0},
	    {2, 'f', 1000}
	   },
	   // Springs:
	   // spring id  node1 node2  spring constant
	   {{1,  1, 2,  500}
	   },
	   // Displacements:
	   // node id  displacement
	   {{1, 0},
	    {2, 2}
	   },
	   // Forces:
	   // node id  force
	   {{1, -1000},
	    {2,  1000}
	   },
	   // Local forces:
	   // spring id  local force node1, node2
	   {{1, -1000, 1000}
	   }
  	   );
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace nsl

/* fin */

