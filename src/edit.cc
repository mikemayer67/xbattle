#include "xbattle.h"

#include <iostream>
#include <sstream>

using namespace std;

void XBattle::edit(string xbt)
{
  string xbt_path = _commandLine.find_file(xbt);

  if(xbt_path.empty())
  {
    if( xbt.size()<4 || xbt.substr(xbt.size()-4)!=".xbt" )
    {
      xbt.append(".xbt");
      xbt_path = _commandLine.find_file(xbt);
    }
  }

  if(xbt_path.empty())
  {
    stringstream err;
    err << "Failed to find xbt file (" << xbt << ") to edit";
    throw runtime_error(err.str());
  }

  cout << endl
    << "Edit still needs to be implemented" << endl
    << " (File to edit is: " << xbt_path << ")" << endl << endl;
}

