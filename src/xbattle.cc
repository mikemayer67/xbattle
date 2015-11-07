#include "xbattle.h"

#include <iostream>
#include <exception>

int main(int argc,const char **argv)
{
  try
  {
    XBattle xbattle(argc,argv);
    xbattle.run();
  }
  catch( std::exception &e )
  {
    std::cerr << std::endl << "Sorry: " << e.what() << std::endl << std::endl;
  }
  return 0;
}


XBattle::XBattle(int argc,const char **argv)
  : _command_line( argc, argv)
{
  _options.init( _command_line.args() );
}

void XBattle::run(void)
{
  std::cout << "done" << std::endl;
}
