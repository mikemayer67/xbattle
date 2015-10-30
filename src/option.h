#ifndef _OPTION_H_
#define _OPTION_H_

#include "types.h"

class Option
{
  public:
    Option(      StringList_t::const_iterator &, 
           const StringList_t::const_iterator &,
           bool  inGroup=false );

  private:
    std::string         _key;
    StringList_t        _args;
    std::vector<Option> _optGroup;
};

#endif // _OPTION_H_
