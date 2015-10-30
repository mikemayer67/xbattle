#include "options.h"
#include "option.h"

using namespace std;

struct option
{
  string         key;
  StringList_t   args;
  vector<option> subOptions;

  option(      StringList_t::const_iterator &, 
         const StringList_t::const_iterator &,
         bool  inGroup=false );
}


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

////////////////////////////////////////////////////////////////////////////////
// option struct constructor
////////////////////////////////////////////////////////////////////////////////

option::option(       StringList_t::const_iterator &pos,
                const StringList_t::const_iterator &end,
                bool  inGroup )
{
  key = *(pos++);

  if(key[0]!='-') 
    throw runtime_error(string("Invalid option '")+key+"' (does not start with -)");

  key.erase(0,1);

  while(pos<end)
  {
    if(*pos=="{")
    {
      if(inGroup)
        throw logic_error("CommandLine constructor failed to catch nested {} group");

      if(!subOptions.empty())
        throw runtime_error("Only one {} group allowed as argument for any given option");

      StringList_t::const_iterator groupPos = ++pos;
      StringList_t::const_iterator groupEnd =   pos;

      while(groupEnd!=end && *groupEnd!="}") ++groupEnd;

      if(groupEnd==end)
        throw logic_error("ComandLine constructor failed to catch opening { without closing }");

      while(groupPos!=groupEnd)
      {
        subOptions.push_back( Option(groupPos, groupEnd, true ) );
      }

      pos = groupEnd;
      ++pos;
    }
    else if(*pos=="}")
    {
      throw logic_error("CommandLine constructor failed to catch closing } without an opening {");
    }
    else if(pos->at(0)=='-')
    {
      break;
    }
    else
    {
      string arg = *pos;
      args.push_back(arg);
      ++pos;
    }
  }
}
