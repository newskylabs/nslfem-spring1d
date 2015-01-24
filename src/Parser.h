// -*- mode: C++ -*-
/**
   Dietrich Bollmann, Kamakura, 2015/01/01
   
   Parser.h

   Class: Parser
   
   Copyright (c) 2015 Dietrich Bollmann
   
   This software may be modified and distributed under the terms
   of the MIT license.  See the LICENSE file for details.
*/

#ifndef __Parser__
#define __Parser__

#include <fstream>
#include <vector>
#include <string>

namespace nsl {

// =========================================================
// class Parser
// ---------------------------------------------------------

class FEM;

class Parser {

private:
  FEM *_fgfem;
  std::vector<std::string> *_files;
  
  std::ifstream *_in;
  std::string _token;

public:
  Parser(FEM *fgfem, const std::vector<std::string> &files);
  ~Parser();

  void parse();

private:
  void open(const std::string &file);
  void close();
  void getNextToken();
  int getNextChar();
  int parseInt();
  double parseDouble();
  void parseSinglelineComment();
  void parseMultilineComment();
  void parseNode();
  void parseSpring();
};

} // namespace nsl

#endif /* defined(__Parser__) */

/* fin */
