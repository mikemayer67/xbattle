#include <iostream>
#include <exception>
#include "CommandLine.h"

int main(int argc,const char **argv)
{
  try
  {
    CommandLine cmd_line(argc,argv);
    std::cout << "done" << std::endl;
  }
  catch( std::runtime_error &e )
  {
    std::cerr << std::endl << "Sorry: " << e.what() << std::endl << std::endl;
  }
  return 0;
}
