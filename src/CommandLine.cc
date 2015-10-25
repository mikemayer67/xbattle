////////////////////////////////////////////////////////////////////////////////
// See CommandLin.h for a description of this class
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <sstream>
#include <exception>

#include "CommandLine.h"

using namespace std;

StringList_t fix_brackets(StringList_t raw);
StringList_t expand_xbo(StringList_t raw,StringSet_t hist,CommandLine &);

CommandLine::CommandLine(int argc,const char **argv)
{
  if( argc<1 ) throw invalid_argument("Missing process name on invocation");

  //
  // Locate current working directory
  //

  const char *cwd = getcwd(NULL,0);
  _cwd = cwd;
  free((void*)cwd);
  if( _cwd.empty() ) throw runtime_error("Cannot determine current working directory");

  //
  // Locate process binary
  //

  string cmd = argv[0];

  if(cmd.empty())       throw runtime_error("Cannot find process name in argv");

  if( cmd.find('/') == string::npos )  // need to find executable in PATH
  {
    StringList_t path = split_string( getenv("PATH"), ':' );
    for( StringList_t::iterator x=path.begin(); x!=path.end(); ++x )
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
  _path = cmd.substr(0,sep);
  _proc = cmd.substr(1+sep);

  if( _path.empty() ) _path = "/";

  //
  // Build the search path for xbattle data files
  //

  _filePath.push_back(_cwd);

  if(getenv("HOME")) 
  {
    _filePath.push_back(string(getenv("HOME"))+"/xbattle");
    _filePath.push_back(string(getenv("HOME"))+"/.xbattle");
  }

  _filePath.push_back(_path);
  _filePath.push_back(_path+"/xba");
  _filePath.push_back(_path+"/xbo");
  _filePath.push_back(_path+"/xbt");

  _filePath.push_back(_path+"/share");
  _filePath.push_back(_path+"/share/xba");
  _filePath.push_back(_path+"/share/xbo");
  _filePath.push_back(_path+"/share/xbt");

  if( _path.rfind("/bin") == _path.length()-4 )
  {
    _filePath.push_back(_path+"../share");
    _filePath.push_back(_path+"../share/xba");
    _filePath.push_back(_path+"../share/xbo");
    _filePath.push_back(_path+"../share/xbt");
  }

  //
  // Load argument vector
  //

  _args.insert(_args.end(), argv+1, argv+argc );

  //
  // Add spaces around brackets if needed
  //

  _args = fix_brackets(_args);

  //
  // Expand any option files
  //
 
  StringSet_t xbo_hist;
  _args = expand_xbo(_args,xbo_hist,*this);
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
      rval = file;
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

  return rval;
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
      string xbo_path = cl.find_file(xbo_file);
      if( xbo_path.empty() && xbo_file.rfind(".xbo")!=xbo_file.length()-4 ) 
      {
        xbo_path = cl.find_file(xbo_file + ".xbo");
      }
      if( xbo_path.empty() ) throw runtime_error(string("Cannot find option file '")+xbo_file+"'");

      if(hist.find(xbo_path)!=hist.end())
        throw runtime_error(string("Cannot recursively include option files (")+xbo_file+")");

      hist.insert(xbo_path);

      ifstream src(xbo_path);
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
      args = expand_xbo(args,hist,cl);

      rval.insert(rval.end(),args.begin(),args.end());
    }
  }

  return rval;
}
