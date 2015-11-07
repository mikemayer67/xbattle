#ifndef _OPTION_H_
#define _OPTION_H_

#include "types.h"
#include <vector>
#include <map>

#include <limits.h>
#include <float.h>


class Option
{
  public:
    Option(      StringList_t::const_iterator &, 
           const StringList_t::const_iterator &,
           bool  inGroup=false );

    bool operator==(const std::string &key) const
    {
      return _key == key;
    }

    void        validateNoArgs(void) const;

    StringList_t getArgs(int num) const;

    int          getInt      ( int    min=INT_MIN, int    max=INT_MAX   ) const;
    ulong        getULong    ( ulong  min=0,       ulong  max=ULONG_MAX ) const;
    double       getDouble   ( double min=DBL_MIN, double max=DBL_MAX   ) const;
    std::string  getFilename ( std::string defaultValue=""              ) const;
    void         getColor    ( std::vector<RGB> &                       ) const;
    void         getColor    ( std::map<std::string,RGB> &              ) const;

  private:
    std::string         _key;
    StringList_t        _args;
    std::vector<Option> _subOptions;
};

#endif // _OPTION_H_


