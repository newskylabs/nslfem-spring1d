// -*- mode: C++ -*-
/**
   Dietrich Bollmann, Kamakura, 2015/01/01
   
   Parser.cpp

   Class: Parser
   
   Copyright (c) 2015 Dietrich Bollmann
   
   This software may be modified and distributed under the terms
   of the MIT license.  See the LICENSE file for details.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "FEM.h"
#include "Parser.h"

namespace nsl {

// =========================================================
// Class Parser
// ---------------------------------------------------------

Parser::Parser(FEM *fgfem, const std::vector<std::string> &files) : _in(nullptr)
{
  _fgfem = fgfem;
  _files = new std::vector<std::string>(files);
}

Parser::~Parser() 
{
  delete _files;
}

// =========================================================
// Methods
// ---------------------------------------------------------

/**
   Parse a FEM definition file.
*/
void Parser::parse() 
{
  // Print message
  std::cout << "Input files: " << std::endl << std::endl;

  for (const auto &file : *_files)
    {
      // Print message
      std::cout << "  - " << file << std::endl;

      // Open the FEM definition file
      open(file);
      
      // Parse node and spring definitions
      getNextToken();
      while (_token != "eof")
	{
	  if       (_token.compare(0, 2, "//") == 0)  parseSinglelineComment();
	  else if  (_token.compare(0, 2, "/*") == 0)  parseMultilineComment();
	  else if  (_token == "node")                 parseNode();
	  else if  (_token == "spring")               parseSpring();
	  else
	    {
	      std::cerr << "ERROR: Unexpected token: " << _token << std::endl;
	      exit(EXIT_FAILURE);
	    }
	}
      
      // Close the FEM definition file
      close();
    }
  std::cout << std::endl;
}

/**
   Open the FEM definition file.
*/
void Parser::open(const std::string &file) 
{
  // Open the ifstream  
  _in = new std::ifstream(file, std::ios_base::in);
  if (!_in ||
      !_in->good())
    {
      std::cerr << "Error: Could not open input file: " << file << std::endl;
      exit(EXIT_FAILURE);
    }
}

/**
   Close the FEM definition file.
*/
void Parser::close()
{
  _in->close();
  delete _in;
  _in = nullptr;
}

/**
   Get the next token.
*/
void Parser::getNextToken()
{
  if (_in->eof())
    _token = "eof";
  else
    *_in >> _token;

  // DEBUG
  //std::cout << "token: " << _token << std::endl;
}

/**
   Get the next character.
*/
int Parser::getNextChar()
{
  int c;
  if (_in->eof())
    c = -1;
  else
    c = _in->get();

  // DEBUG
  //std::cout << "char: " << _token << std::endl;

  return c;
}

/**
   Parse an integer.
*/
int Parser::parseInt()
{
  int i;
  if (*_in >> i) 
    return i;
  else
    {
      std::cerr << "ERROR: Expected an integer." << std::endl;
      exit(EXIT_FAILURE);
    }
}

/**
   Parse a double.
*/
double Parser::parseDouble()
{
  double d;
  if (*_in >> d) 
    return d;
  else
    {
      std::cerr << "ERROR: Expected a double." << std::endl;
      exit(EXIT_FAILURE);
    }
}

/**
   Parse a multiline comment
*/
void Parser::parseSinglelineComment()
{
  // Skip the rest of the line
  int c;
  c = getNextChar();
  while (c != -1 && c != '\n')
    c = getNextChar();

  // Read next token
  getNextToken();
}

/**
   Parse a multiline comment
*/
void Parser::parseMultilineComment()
{
  // Skip characters until next "*/"
  int c1, c2;
  c1 = getNextChar();
  c2 = getNextChar();
  while (c1 != -1 && c2 != -1 && !(c1 == '*' && c2 == '/'))
    {
      c1 = c2;
      c2 = getNextChar();
    }

  // Get next token
  getNextToken();
}

/**
   Parse a node definition.
*/
void Parser::parseNode()
{
  // Parse node id
  int id = parseInt();
  
  // Add the node
  _fgfem->addNode(id);
  
  // Parse dispacement and force when given
  getNextToken();
  while (_token == "d" || _token == "f")
    {
      if (_token == "d")
	{
	  double displacement = parseDouble();
	  _fgfem->addDisplacement(id, displacement);
	}
      else // _token == "f"
	{
	  double force = parseDouble();
	  _fgfem->addForce(id, force);
	}

      // Get next token
      getNextToken();
    }
}

/**
   Parse a spring definition.
*/
void Parser::parseSpring()
{
  // Parse spring id
  int id = parseInt();
  
  // Parse ids of start ane end nodes
  int node1 = parseInt();
  int node2 = parseInt();
  
  // Parse spring constant
  double springConstant = parseDouble();
  
  // Add the spring
  _fgfem->addSpring(id, node1, node2, springConstant);

  // Get next token
  getNextToken();
}

} // namespace nsl

/* fin */
