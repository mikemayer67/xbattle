////////////////////////////////////////////////////////////////////////////////
// See commandline.h for a description of this class
////////////////////////////////////////////////////////////////////////////////

#include "commandline.h"
#include "config.h"
#include "types.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>
#include <algorithm>

#include <stdlib.h>
#include <unistd.h>

using namespace std;

StringList_t fix_brackets(StringList_t);
void         validate_bracket_nesting(StringList_t);
StringList_t expand_xbo(StringList_t raw,StringSet_t hist,CommandLine &);

class EmptyStringPredicate
{ 
  public: 
    bool operator()(const string &s) 
    { return s.find_first_not_of(" \t")==string::npos; }
};

class IsOptionPredicate
{
  public:
    bool operator()(const string &s)
    { return s.length()>1 && s.at(0)=='-'; }
};

CommandLine::CommandLine(int argc,const char **argv)
{
  if( argc<1 ) throw invalid_argument("Missing process name on invocation");

  // Locate current working directory

  const char *cwd = getcwd(NULL,0);
  _cwd = cwd;
  free((void*)cwd);
  if( _cwd.empty() ) throw runtime_error("Cannot determine current working directory");

  // Locate process binary

  string cmd = argv[0];

  _procDir = _cwd;  // default value - may be changed below
  _proc    = cmd;   // default value - may be changed below

  if(cmd.empty())       throw runtime_error("Cannot find process name in argv");

  if( cmd.find('/') == string::npos )  // need to find executable in PATH
  {
    StringList_t dirlist;

    const char *path = getenv("PATH");
    if( path != NULL )
    {
      istringstream pathstream( path );
      string dir;
      while(std::getline(pathstream,dir,':')) dirlist.push_back(dir);
    }

    for( StringList_t::iterator x=dirlist.begin(); x!=dirlist.end(); ++x )
    {
      string candidate = rel_to_abs( *x + "/" + cmd );
      if( access(candidate.c_str(), X_OK) == 0 )
      {
        cmd = candidate;
        break;
      }
    }
  }

  cmd = rel_to_abs(cmd);

  string::size_type sep = cmd.find_last_of('/');
  if(sep!=string::npos)
  {
    _procDir = cmd.substr(0,sep);
    _proc    = cmd.substr(1+sep);
  }

  if( _procDir.empty() ) _procDir = "/";

  // Build the search path for xbattle rc data files

  _filePath.push_back(_cwd);

  if(getenv("HOME")) 
  {
    _filePath.push_back(string(getenv("HOME"))+"/xbattle");
    _filePath.push_back(string(getenv("HOME"))+"/.xbattle");
    _filePath.push_back(string(getenv("HOME")));
  }

  _filePath.push_back(_procDir);

  _filePath.push_back(_procDir+"/xba");
  _filePath.push_back(_procDir+"/xbo");
  _filePath.push_back(_procDir+"/xbt");

  if( _procDir.rfind("/bin") == _procDir.length()-4 )
  {
    _filePath.push_back(_procDir+"../share");
    _filePath.push_back(_procDir+"../share/xba");
    _filePath.push_back(_procDir+"../share/xbo");
    _filePath.push_back(_procDir+"../share/xbt");
  }
  else
  {
    _filePath.push_back(_procDir+"/share");
    _filePath.push_back(_procDir+"/share/xba");
    _filePath.push_back(_procDir+"/share/xbo");
    _filePath.push_back(_procDir+"/share/xbt");
  }

  // Load argument vector
  StringList_t args;
  args.insert(args.end(), argv+1, argv+argc);

  // Set .xbattlerc or .xbattle as the first file to load
  //   (if found)

  StringList_t cand_rc;
  cand_rc.push_back(".xbattlerc");
  cand_rc.push_back("xbattle.rc");

  for( StringList_t::iterator rc=cand_rc.begin(); rc!=cand_rc.end(); ++rc)
  {
    string xbrc = find_file(*rc);
    if( !xbrc.empty() ) 
    {
      StringList_t rc_args;
      rc_args.push_back("-xbo");
      rc_args.push_back(*rc);
      StringList_t::iterator pos = find_if(args.begin(),args.end(),IsOptionPredicate());
      args.insert( pos, rc_args.begin(), rc_args.end() );
      break;
    }
  }

  // Add spaces around brackets if needed
  args = fix_brackets(args);

  validate_bracket_nesting(args);  // check brackets on command line

  // Expand any option files
  StringSet_t xbo_hist;
  args = expand_xbo(args,xbo_hist,*this);

  validate_bracket_nesting(args);  // recheck brackets after expansion

  // Remove any empty strings from the argument list
  args.erase( remove_if(args.begin(), args.end(), EmptyStringPredicate()), args.end() );

  // Find first option argument (first argument to start with a -)
  StringList_t::const_iterator begin = args.begin();
  StringList_t::const_iterator end   = args.end();
  StringList_t::const_iterator pos   = find_if(begin,end,IsOptionPredicate());

  // Grab all arguments prior to first option

  _args.insert(_args.end(), begin, pos);

  // Grab all options

  while(pos!=end)
  {
    _options.push_back( Option(pos,end) );
  }
}

bool CommandLine::optionWasSet(std::string s) const
{
  for(OptionList_t::const_iterator x=_options.begin(); x!=_options.end(); ++x)
  {
    if( *x == s ) return true;

    const OptionList_t subopt = x->subOptions();
    for(OptionList_t::const_iterator xx=subopt.begin(); xx!=subopt.end(); ++xx)
    {
      if( *xx == s ) return true;
    }
  }

  return false;
}

StringSet_t CommandLine::optionKeys(void) const
{
  StringSet_t rval;

  for(OptionList_t::const_iterator x=_options.begin(); x!=_options.end(); ++x)
  {
    rval.insert(x->key());

    const OptionList_t subopt = x->subOptions();
    for(OptionList_t::const_iterator xx=subopt.begin(); xx!=subopt.end(); ++xx)
    {
      rval.insert(xx->key());
    }
  }

  return rval;
}

string CommandLine::rel_to_abs( string path ) const
{
  string rval = path;
  if( path.empty() || path.front() != '/' )
  {
    rval.insert(0,1,'/');
    rval.insert(0,_cwd);
  }

  return rval;
}

string CommandLine::find_file( string file ) const
{
  string rval;

  if( file.length()>1 )
  {
    if( file[0]=='/' )
    {
      if( access(file.c_str(),R_OK)==0 ) rval=file;
    }
    else
    {
      for( StringList_t::const_iterator x=_filePath.begin(); rval.empty() && x!=_filePath.end(); ++x)
      {
        string cand_file = *x + "/" + file;
        if( access(cand_file.c_str(),R_OK)==0 ) rval=cand_file;
      }
    }
  }

  return rval;
}

////////////////////////////////////////////////////////////////////////////////
// Support Functions
////////////////////////////////////////////////////////////////////////////////

StringList_t fix_brackets(StringList_t raw)
{
  StringList_t rval;

  for(StringList_t::iterator x=raw.begin(); x!=raw.end(); ++x)
  {
    string arg = *x;
    string::iterator a=arg.begin();
    for(string::iterator b=a; b<arg.end(); ++b)
    {
      if(*b=='{' || *b=='}')
      {
        if(a<b) rval.push_back(string(a,b));
        a=b;
        ++a;
        rval.push_back(string(b,a));
      }
    }
    if(a<arg.end()) rval.push_back(string(a,arg.end()));
  }

  validate_bracket_nesting(rval);

  return rval;
}

void validate_bracket_nesting(StringList_t args)
{
  bool inGroup=false;
  for(StringList_t::iterator x=args.begin(); x!=args.end(); ++x)
  {
    if(*x=="{")
    {
      if(inGroup) throw runtime_error("Nested {} brackets are not allowed");
      inGroup=true;
    }
    else if(*x=="}")
    {
      if(!inGroup) throw runtime_error("Closing } bracket found without an opening { bracket");
      inGroup=false;
    }
  }

  if(inGroup) throw runtime_error("Opening { bracket found without a closing } bracket");
}

StringList_t expand_xbo(StringList_t raw,StringSet_t hist,CommandLine &cl)
{
  StringList_t rval;

  for(StringList_t::iterator x=raw.begin(); x!=raw.end(); ++x)
  {
    string arg = *x;
    string xbo_file;

    if(arg=="-options" || arg=="-xbo")
    {
      ++x;
      if(x==raw.end()) throw runtime_error(string(arg) + " must be followed by a filename");
      xbo_file = *x;
    }
    else if( arg.length()>5 && arg[0]=='-' && arg.rfind(".xbo")==arg.length()-4 )
    {
      xbo_file = arg.substr(1);
    }

    if( xbo_file.empty() )
    {
      rval.push_back(arg);
    }
    else
    {
      string xbo_procDir = cl.find_file(xbo_file);
      if( xbo_procDir.empty() && xbo_file.rfind(".xbo")!=xbo_file.length()-4 ) 
      {
        xbo_procDir = cl.find_file(xbo_file + ".xbo");
      }
      if( xbo_procDir.empty() ) throw runtime_error(string("Cannot find option file '")+xbo_file+"'");

      if(hist.find(xbo_procDir)!=hist.end())
        throw runtime_error(string("Cannot recursively include option files (")+xbo_file+")");

      hist.insert(xbo_procDir);

      ifstream src(xbo_procDir);
      if(!src) throw runtime_error(string("Cannot open options file '") + xbo_file + "'");

      StringList_t args;
      string line;
      while(src >> line)
      {
        string::size_type p = line.find_first_of('#');
        if(p!=string::npos) line.erase(p);

        istringstream ss(line);
    
        while( ss >> arg ) 
        {
          args.push_back(arg);
        }
      }
      src.close();

      args = fix_brackets(args);

      // check brackets within a given option file
      validate_bracket_nesting(args);

      args = expand_xbo(args,hist,cl);

      rval.insert(rval.end(),args.begin(),args.end());
    }
  }

  return rval;
}
