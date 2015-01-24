// -*- mode: C++ -*-
/**
   Dietrich Bollmann, Kamakura, 2015/01/01
   
   main.cpp
   
   nslfem-spring1d main file.
   
   Copyright (c) 2015 Dietrich Bollmann
   
   This software may be modified and distributed under the terms
   of the MIT license.  See the LICENSE file for details.
*/

#include "FEM.h"

#include <vector>
#include <string>
#include <iostream>

/**
   Print help text.
 */
void help()
{
  std::cout 
    << "Usage: nslfem-spring1d <fem definition file>" << std::endl
    ;
}

/**
   Main.
 */
int main(int argc, char* argv[])
{
  // List of FEM definition files
  std::vector<std::string> files;

  // Parse command-line arguments
  for(int i = 1; i < argc; ++i )
    {
      if (strcmp(argv[i], "-h") == 0 || 
	  strcmp(argv[i], "--help") == 0)
	{
	  help();
	  exit(EXIT_SUCCESS);
	}
      else 
	files.push_back(argv[i]);
    }
  
  // Header
  std::cout << std::endl << "FEM: Spring Assemblage (1D)" << std::endl << std::endl;
  
  // Processing the input file
  nsl::FEM fem(files);
  fem.solve();
  fem.printResults();

  // Footer
  std::cout << "fin." << std::endl << std::endl;
  
  return 0;
}
  
/* fin */
