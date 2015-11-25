#include "option.h"

#include <sstream>
#include <locale>

using namespace std;

#ifndef MAXLEVELS
#define MAXLEVELS 99
#endif

//
// Support functions
//

ulong  str_to_ulong  (string s, string opt);
long   str_to_long   (string s, string opt);
int    str_to_int    (string s, string opt);
double str_to_double (string s, string opt);

template<typename T>
void validateLimits(T v, T min, T max, string opt);

//
// Options methods
//

Option::Option( StringList_t::const_iterator &pos,
                const StringList_t::const_iterator &end,
                bool  inGroup )
{
  _key = *(pos++);

  if( _key[0] != '-' )
    throw runtime_error(string("Invalid _keyion '")+_key+"' (does not start with -)");

  locale loc;
  for(int i=0; i<_key.size(); ++i) _key[i] = tolower(_key[i],loc);

  _key.erase( remove(_key.begin(),_key.end(),'-'), _key.end() );
  _key.erase( remove(_key.begin(),_key.end(),'_'), _key.end() );

  // translate synonyms
  if      ( _key=="foresttones")  _key = "forestlevels";
  else if ( _key=="hilltones")    _key = "hilllevels";
  else if ( _key=="sea")          _key = "seafrac";
  else if ( _key=="sealevels")    _key = "seadepth";
  else if ( _key=="seatones")     _key = "seadepth";

  if(inGroup)
  {
    if( _key=="color" )
      throw runtime_error("The -color option cannot appear within a set of team options");
    if( _key=="colorinverse" )
      throw runtime_error("The -color_inverse option cannot appear within a set of team options");
  }

  while(pos!=end)
  {
    if(*pos=="{")
    {
      if(inGroup)
        throw logic_error("CommandLine constructor failed to catch nested {} group");

      if(!_subOptions.empty())
        throw runtime_error("Only one {} group allowed as argument for any given option");

      StringList_t::const_iterator groupPos = ++pos;
      StringList_t::const_iterator groupEnd =   pos;

      while(groupEnd!=end && *groupEnd!="}") ++groupEnd;

      if(groupEnd==end)
        throw logic_error("ComandLine constructor failed to catch opening { without closing }");

      while(groupPos!=groupEnd)
      {
        _subOptions.push_back( Option(groupPos, groupEnd, true ) );
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

bool Option::operator==(std::string key) const
{
  if( _key == key ) return true;
  if( key.substr(0,2)=="no" )
  {
    key.replace(0,2,"no_");
    if( _key==key ) return true;
  }
  return false;
}

void Option::validateNoArgs(void) const
{
  if( ! _args.empty() )
  {
    stringstream err;
    err 
      << "The -" << _key << " option does not take any arguments";
    throw runtime_error(err.str());
  }
}

StringList_t Option::getArgs(int min, int max) const
{
  if(min>max) throw logic_error("getArgs(min,max) called with min>max");

  if( _args.size() < min || _args.size() > max )
  {
    stringstream err;
    err << "The -" << _key << " option requrires ";
    if(max==1)
    {
      if(min==0) err << "no more than one (optional) argument";
      else       err << "a single argument";
    }
    else if(min==0)      err << "no more than " << max << " arguments";
    else if(min==max)    err << "exactly " << min << " arguments";
    else                 err << "between " << min << " and " << max << " arguments";

    throw runtime_error(err.str());
  }
  return _args;
}

int Option::getInt(int min,int max) const
{
  if( _args.size() != 1 )
  {
    stringstream err;
    err << "The -" << _key << " option requires a single integer argument";
    throw runtime_error(err.str());
  }

  int rval = str_to_int( _args.at(0), _key );
  validateLimits(rval,min,max,_key);

  return rval;
}

ulong Option::getULong(ulong min,ulong max) const
{
  if( _args.size() != 1 )
  {
    stringstream err;
    err << "The -" << _key << " option requires a single integer argument";
    throw runtime_error(err.str());
  }

  ulong rval = str_to_ulong( _args.at(0), _key );
  validateLimits(rval,min,max,_key);

  return rval;
}

double Option::getDouble(double min,double max) const
{
  if( _args.size() != 1 )
  {
    stringstream err;
    err << "The -" << _key << " option requires a single numeric argument";
    throw runtime_error(err.str());
  }

  double rval = str_to_double( _args.at(0), _key );
  validateLimits(rval,min,max,_key);
  string arg = _args.at(0);

  return rval;
}

double Option::getProbability(void) const
{
  double rval = getDouble(0,100.);

  return ( rval>1. ? 0.01*rval : rval );
}

bool Option::getOptInt(int &value, int min, int max) const
{
  if( _args.empty() ) return false;

  value = getInt(min,max);

  return true;
}

bool Option::getIntOrFrac(int &intValue, double &fracValue, bool optional) const
{
  if( _args.empty() && optional) return false;

  if( _args.size() != 1 )
  {
    stringstream err;
    err << "The -" << _key << " option requires either a numeric argument in the range 0-1 or an integer argument greater than 1";
    throw runtime_error(err.str());
  }

  double value = getDouble(0.);
  if(value<1.)
  {
    intValue  = 0;
    fracValue = value;
  }
  else
  {
    intValue = int(value+0.5);
    fracValue = 0.;
  }

  return true;
}

string Option::getString(void) const
{
  if(_args.size()!=1)
  {
    stringstream err;
    err << "The -" << _key << " option accepts a single string argument";
    throw runtime_error(err.str());
  }
  
  return _args.at(0);
}

string Option::getFilename(string defaultValue) const
{
  int n = _args.size();

  if( defaultValue.empty() )
  {
    if(n!=1)
    {
      stringstream err;
      err << "The -" << _key << " option requires a single string argument";
      throw runtime_error(err.str());
    }
  }
  else
  {
    if(n==0) return defaultValue;
    if(n>1)
    {
      stringstream err;
      err << "The -" << _key << " option accepts a single optional string argument";
      throw runtime_error(err.str());
    }
  }
  
  return _args.at(0);
}

void Option::getColor( vector<RGB> &colors ) const
{
  int n = _args.size();

  if( ! (n==2 || n==4) )
  {
    stringstream err;
    err << "The -" << _key << " option requires either" << endl
      << "      an integer argument (level) followed by a color name" << endl
      << "   or 4 integer arguments: level red green blue" << endl;
    throw runtime_error(err.str());
  }
 
  RGB color;
  try
  {
    if( n==2 ) color = RGB( _args.at(1) );
    else       color = RGB( str_to_int(_args.at(1),_key),
                            str_to_int(_args.at(2),_key),
                            str_to_int(_args.at(3),_key) );
  }
  catch(exception &e)
  {
    stringstream err;
    err
      << "Invalid color argument found while parsing the -" << _key
      << " option (" << e.what() << ")";
    throw runtime_error(err.str());
  }

  int level = str_to_int( _args.at(0), _key );

  if(level<1 || level>MAXLEVELS)
  {
    stringstream err;
    err
      << "The level argument provided for the -" << _key << " option is out of range"
      << "  (" << level << " is not between 1 and " << MAXLEVELS << ")";
    throw runtime_error(err.str());
  }

  if( colors.size() < level )
  {
    RGB nullColor;
    while(colors.size() < level-1) colors.push_back( nullColor );
    colors.push_back(color);
  }
  else
  {
    colors.at(level-1) = color;
  }
}

void Option::getColor( string &name, RGB &color, string &inverse ) const
{
  int n = _args.size();

  if( n<4 || n>5 )
  {
    stringstream err;
    err << "The -" << _key << " option requires a color name" << endl
      << "   followed by red, green, and blue component values (0-255)" << endl
      << "   followed by an optional inverse color name";
    throw runtime_error(err.str());
  }

  try
  {
    name = _args.at(0);

    color = RGB( str_to_int(_args.at(1),_key),
                 str_to_int(_args.at(2),_key),
                 str_to_int(_args.at(3),_key) );

    if( n==5 ) inverse = _args.at(4);
  }
  catch(exception e)
  {
    stringstream err;
    err << "The -" << _key << " option failed to parse the color values" << endl
      << "    " << e.what();
    throw runtime_error(err.str());
  }

}

////////////////////////////////////////////////////////////////////////////////
// support functions
////////////////////////////////////////////////////////////////////////////////

ulong str_to_ulong(string s, string opt)
{
  errno=0;
  ulong rval = strtoul(s.c_str(),NULL,10);
  if(errno)
  {
    stringstream err;
    err 
      << "Failed to convert '" << s << "' to an integer while parsing the -" 
      << opt << " option [" << strerror(errno) << "]";
    throw runtime_error(err.str());
  }
  return rval;
}

long str_to_long(string s, string opt)
{
  errno=0;
  long rval = strtol(s.c_str(),NULL,10);
  if(errno)
  {
    stringstream err;
    err 
      << "Failed to convert '" << s << "' to an integer while parsing the -" 
      << opt << " option [" << strerror(errno) << "]";
    throw runtime_error(err.str());
  }
  return rval;
}

int str_to_int(string s, string opt)
{
  long rval = str_to_long(s,opt);

  if(rval<INT_MIN||rval>INT_MAX) 
  {
    stringstream err;
    err 
      << "Failed to convert '" << s << "' to an integer while parsing the -" 
      << opt << " option [" << strerror(ERANGE) << "]";
    throw runtime_error(err.str());
  }

  return int(rval);
}

double str_to_double(string s, string opt)
{
  errno=0;
  double rval = strtod(s.c_str(),NULL);
  if(errno)
  {
    stringstream err;
    err 
      << "Failed to convert '" << s << "' to a number while parsing the -" 
      << opt << " option [" << strerror(errno) << "]";
    throw runtime_error(err.str());
  }
  return rval;
}

template<typename T>
void validateLimits(T v, T min, T max, string opt)
{
  if(v<min)
  {
    stringstream err;
    err 
      << "The argument provided for the -" << opt << " option is too small"
      << "  (it must be no less than " << min << ")";
    throw runtime_error(err.str());
  }
  if(v>max)
  {
    stringstream err;
    err 
      << "The argument provided for the -" << opt << " option is too large"
      << "  (it must be no greater than " << max << ")";
    throw runtime_error(err.str());
  }
}
