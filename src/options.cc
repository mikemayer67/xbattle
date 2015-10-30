#include "options.h"
#include "option.h"

using namespace std;

void Options::init( const StringList_t &args )
{
  //
  // Walk through the argument list and convert them to known options
  //

  StringList_t::const_iterator pos = args.begin();
  StringList_t::const_iterator end = args.end();

  while(pos<end)
  {
    Option opt(pos,end);
  }
}
