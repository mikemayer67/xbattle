#include "option.h"

using namespace std;

Option::Option(       StringList_t::const_iterator &pos,
                const StringList_t::const_iterator &end,
                bool  inGroup )
{
  _key = *(pos++);

  if(_key[0]!='-') 
    throw runtime_error(string("Invalid option '")+_key+"' (does not start with -)");

  _key.erase(0,1);

  while(pos<end)
  {
    if(*pos=="{")
    {
      if(inGroup)
        throw logic_error("CommandLine constructor failed to catch nested {} group");

      if(!_optGroup.empty())
        throw runtime_error("Only one {} group allowed as argument for any given option");

      StringList_t::const_iterator groupPos = ++pos;
      StringList_t::const_iterator groupEnd =   pos;

      while(groupEnd!=end && *groupEnd!="}") ++groupEnd;

      if(groupEnd==end)
        throw logic_error("ComandLine constructor failed to catch opening { without closing }");

      while(groupPos!=groupEnd)
      {
        _optGroup.push_back( Option(groupPos, groupEnd, true ) );
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
      _args.push_back(arg);
      ++pos;
    }
  }
}
