// -*- mode: C++ -*-
/**
   Dietrich Bollmann, Kamakura, 2015/01/01
   
   DMatrix-test.h

   Unit tests for class: DMatrix
   
   Copyright (c) 2015 Dietrich Bollmann
   
   This software may be modified and distributed under the terms
   of the MIT license.  See the LICENSE file for details.
*/

#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "DMatrix.h"

namespace nsl {

BOOST_AUTO_TEST_SUITE(TestSuite_DMatrix)

// Test equality operator
bool test_equality(const std::vector<std::vector<double> > &left, 
		   const std::vector<std::vector<double> > &right)
{
  DMatrix vLeft(left);
  DMatrix vRight(right);

  return (vLeft == vRight);
}

BOOST_AUTO_TEST_CASE(Test_DMatrix_equality_operator)
{
  // equal
  BOOST_REQUIRE(   test_equality( {},          {}          ) );
  BOOST_REQUIRE(   test_equality( {{}},        {{}}        ) );
  BOOST_REQUIRE(   test_equality( {{1}},       {{1}}       ) );

  BOOST_REQUIRE(   test_equality( {{1, 2}},    {{1, 2}}    ) );
  BOOST_REQUIRE(   test_equality( {{1}, {2}},  {{1}, {2}}  ) );

  BOOST_REQUIRE(   test_equality( {{1, 2, 3}}, {{1, 2, 3}}         ) );
  BOOST_REQUIRE(   test_equality( {{1}, {2}, {3}}, {{1}, {2}, {3}} ) );

  BOOST_REQUIRE(   test_equality( {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 
				  {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}  ) );

  // different dimensions
  BOOST_REQUIRE( ! test_equality( {{}},        {{1}}       ) );
  BOOST_REQUIRE( ! test_equality( {{1}},       {{}}        ) );
  BOOST_REQUIRE( ! test_equality( {{1}},       {{1, 2}}    ) );
  BOOST_REQUIRE( ! test_equality( {{1, 2}},    {{1}}       ) );
  BOOST_REQUIRE( ! test_equality( {{1, 2}},    {{1}, {2}}  ) );
  BOOST_REQUIRE( ! test_equality( {{1}, {2}},  {{1, 2}}    ) );

  // different elements
  BOOST_REQUIRE( ! test_equality( {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 
				  {{0, 2, 3}, {4, 5, 6}, {7, 8, 9}}  ) );
  BOOST_REQUIRE( ! test_equality( {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 
				  {{1, 2, 3}, {4, 0, 6}, {7, 8, 9}}  ) );
  BOOST_REQUIRE( ! test_equality( {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, 
				  {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}}  ) );
}

// Test matrix vector multiplication
bool test_mvmult(const std::vector<std::vector<double> > &m, 
		 const std::vector<double> &v,
		 const std::vector<double> &r)
{
  DMatrix m2(m);
  DVector v2(v);
  DVector r2(r);

  return (m2 * v2 == r2);
}

// Test matrix vector multiplication
bool test_mvmult2(const std::vector<std::vector<double> > &m, 
		  const std::vector<double> &v,
		  const std::vector<double> &r)
{
  DMatrix m2(m);
  DVector v2(v);
  DVector r2(r);

  double ed = euclideanDistance((m2 * v2), r2);
  
  // DEBUG
  //| std::cout << "euclidian distance: " << ed << std::endl;
  
  return (ed < 1e-10);
}

BOOST_AUTO_TEST_CASE(Test_DVector_multiplication_operator)
{
  BOOST_REQUIRE( test_mvmult(                                {},         {},            {} ) );
  BOOST_REQUIRE( test_mvmult(                             {{2}},        {3},           {6} ) );
  BOOST_REQUIRE( test_mvmult(                          {{1, 2}},     {3, 4},          {11} ) );
  BOOST_REQUIRE( test_mvmult(                        {{1}, {2}},        {3},        {3, 6} ) );
  BOOST_REQUIRE( test_mvmult(                  {{1, 2}, {3, 4}},     {5, 6},      {17, 39} ) );
  BOOST_REQUIRE( test_mvmult( {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}},  {1, 2, 3},  {14, 32, 50} ) );

  BOOST_REQUIRE( test_mvmult2( {{ 1000,     0, -1000,     0},
 	                        {    0,  3000,     0, -3000},
			        {-1000,     0,  3000, -2000},
			        {    0, -3000, -2000,  5000}},
                               { 0, 0, 10.0/11.0, 15.0/11.0 },
                               { -10000.0/11.0, -45000.0/11.0, 0, 55000.0/11.0 } ) );
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace nsl

/* fin */
