#ifndef _OPTION_H_
#define _OPTION_H_

#include "types.h"
#include <vector>
#include <map>

#include <limits.h>
#include <float.h>

#include "color.h"

class Option
{
  public:
    Option(StringList_t::const_iterator &, 
           const StringList_t::const_iterator &,
           bool  inGroup=false );

    bool operator==(std::string key) const;

    const std::string         &key       (void) const { return _key;        }
    const std::vector<Option> &subOptions(void) const { return _subOptions; }

    void        validateNoArgs(void) const;

    StringList_t getArgs(int min=0, int max=INT_MAX) const;

    int          getInt         ( int    min=INT_MIN, int    max=INT_MAX   ) const;
    ulong        getULong       ( ulong  min=0,       ulong  max=ULONG_MAX ) const;
    double       getDouble      ( double min=DBL_MIN, double max=DBL_MAX   ) const;
    double       getProbability ( void                                     ) const;
    std::string  getString      ( void                                     ) const;
    std::string  getFilename    ( std::string defaultValue=""              ) const;
    void         getColor       ( std::vector<RGB> &                       ) const;
    void         getColor       ( std::string &, RGB &, std::string &inv   ) const;

    bool getOptInt    ( int &, int    min=INT_MIN, int    max=INT_MAX   ) const;
    bool getIntOrFrac ( int &, double &, bool optional=false            ) const;

  private:
    std::string         _key;
    StringList_t        _args;
    std::vector<Option> _subOptions;
};

typedef std::vector<Option> OptionList_t;

#endif // _OPTION_H_


