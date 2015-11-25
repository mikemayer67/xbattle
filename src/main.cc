#include "xbattle.h"

#include <iostream>
#include <sstream>

using namespace std;

void show_usage(void);

int main(int argc,const char **argv)
{
  bool rc=0;
  try
  {
    CommandLine commandLine(argc,argv);

    if( commandLine.optionWasSet("help") ) show_usage();

    XBattle xbattle(commandLine);

    StringList_t args = commandLine.args();
    string action;
    if(!args.empty())
    {
      action=args.at(0);
      args.erase(args.begin());
    }

    if( action=="edit" )
    {
      string xbt;
      switch(args.size())
      {
        case 0: xbt="xbattle.xbt"; break;
        case 1: xbt=args.at(0);    break;
        default:
          throw runtime_error("edit only allows one (optional) argument");
          break;
      }

      xbattle.edit(xbt);
    }
    else if( action=="replay" )
    {
      xbattle.replay(args);
    }
    else if( action=="play" || action.empty() )
    {
      if(!args.empty())
        throw runtime_error("Only edit and replay allow unnamed arguments");

      xbattle.run();
    }
    else
    {
      stringstream err;
      err << "Unknown game action (" << action << ")";
      throw runtime_error(err.str());
    }
  }
  catch( std::exception &e )
  {
    std::cerr << std::endl << "Sorry: " << e.what() << std::endl << std::endl
      << "Try xbattle -help for information on running xbattle" << std::endl << std::endl;
    rc = 1;
  }
  return rc;
}

void show_usage(void)
{
  cout << "Add show_usge function" << endl;
  exit(1);
}
