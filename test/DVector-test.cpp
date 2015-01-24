// -*- mode: C++ -*-
/**
   Dietrich Bollmann, Kamakura, 2015/01/01
   
   DVector-test.h

   Unit tests for class: DVector
   
   Copyright (c) 2015 Dietrich Bollmann
   
   This software may be modified and distributed under the terms
   of the MIT license.  See the LICENSE file for details.
*/

#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE Main
#endif
#include <boost/test/unit_test.hpp>

#include "DVector.h"

namespace nsl {

BOOST_AUTO_TEST_SUITE(TestSuite_DVector)

// Test equality operator
bool test_equality(const std::vector<double> &left, 
		   const std::vector<double> &right)
{
  DVector vLeft(left);
  DVector vRight(right);

  return (vLeft == vRight);
}

BOOST_AUTO_TEST_CASE(Test_DVector_equality_operator)
{
  // equal
  BOOST_REQUIRE(   test_equality( {},        {}        ) );
  BOOST_REQUIRE(   test_equality( {1},       {1}       ) );
  BOOST_REQUIRE(   test_equality( {1, 2},    {1, 2}    ) );
  BOOST_REQUIRE(   test_equality( {1, 2, 3}, {1, 2, 3} ) );

  // different size
  BOOST_REQUIRE( ! test_equality( {},        {1}       ) );
  BOOST_REQUIRE( ! test_equality( {1},       {}        ) );
  BOOST_REQUIRE( ! test_equality( {1},       {1, 2}    ) );
  BOOST_REQUIRE( ! test_equality( {1, 2},    {1}       ) );

  // different elements
  BOOST_REQUIRE( ! test_equality( {1},       {2}       ) );
  BOOST_REQUIRE( ! test_equality( {1, 2},    {3, 2}    ) );
  BOOST_REQUIRE( ! test_equality( {1, 2},    {1, 3}    ) );
  BOOST_REQUIRE( ! test_equality( {1, 2, 3}, {4, 2, 3} ) );
  BOOST_REQUIRE( ! test_equality( {1, 2, 3}, {1, 4, 3} ) );
  BOOST_REQUIRE( ! test_equality( {1, 2, 3}, {1, 2, 4} ) );
}

// Test multiplication
BOOST_AUTO_TEST_CASE(Test_DVector_multiplication_operator)
{
  BOOST_REQUIRE( DVector( (std::vector<double>){4} ) * DVector( (std::vector<double>){7} ) == 28 );
  BOOST_REQUIRE( DVector(    {3, 4} ) * DVector(    {6, 7} ) == 46 );
  BOOST_REQUIRE( DVector( {2, 3, 4} ) * DVector( {5, 6, 7} ) == 56 );
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace nsl

/* fin */
