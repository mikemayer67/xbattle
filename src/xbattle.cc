#include <iostream>
#include "CommandLine.h"

int main(int argc,const char **argv)
{
  CommandLine cmd_line(argc,argv);
  std::cout << "done" << std::endl;
  return 0;
}
