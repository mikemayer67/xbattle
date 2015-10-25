#include "Util.h"

#include <sstream>

using namespace std;

StringList_t split_string(string s, char d)
{
  StringList_t rval;

  istringstream ss(s);

  string x;
  while( std::getline(ss, x, d) ) rval.push_back(x);

  return rval;
}
