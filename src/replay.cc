#include "xbattle.h"

#include <iostream>
#include <sstream>

using namespace std;

void XBattle::replay(StringList_t displays)
{
  cout << endl
    << "Replay still needs to be implemented" << endl;

  if(displays.empty())
  {
    cout << "   (No additional displays specified)" << endl;
  }
  else
  {
    for(int i=0; i<displays.size(); ++i)
    {
      cout << "   (Remote display #" << i+1 << ": " << displays.at(i) << ")" << endl;
    }
  }
  cout << endl;
}

