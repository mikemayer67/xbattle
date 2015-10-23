#include <iostream>
#include <sstream>
#include <exception>

#include "CommandLine.h"

using namespace std;

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
    istringstream env_path(getenv("PATH"));
    string cand_dir;
    while( std::getline(env_path, cand_dir, ':') )
    {
      string cand_file = rel_to_abs( cand_dir + "/" + cmd );
      if( access(cand_file.c_str(), X_OK) == 0 )
      {
        cmd = cand_file;
        break;
      }
    }
  }

  cmd = rel_to_abs(cmd);

  string::size_type sep = cmd.find_last_of('/');
  _path = cmd.substr(0,sep);
  _proc = cmd.substr(1+sep);

  if( _path.empty() ) _path = "/";

  //vector<string> args = read_command_line(argc,argv);
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

//
// Support functions
//
//
//vector<string> read_command_line(int argc, const char **argv)
//{
//  vector<string> rval;
//
//  bool inGroup = false;
//
//  for(int i=1; i<argc; ++i)
//  {
//    string arg = argv[i];
//
//    int arglen = arg.length();
//    if( arglen==0 ) continue;  // skip empty arguments
//
//    string xbo_filename;
//    
//    if( arg[0] == '{' )
//    {
//      if( inGroup )
//        throw runtime_error( "Cannot nest {} groups on command line or in xbo files" );
//
//      inGroup = true;
//      rval.push_back(arg);
//    }
//    else if( arg[0] == '}' )
//    {
//      if( !inGroup )
//        throw runtime_error( "Attempt to close {} group withouth opening one" );
//
//      inGroup = false;
//      rval.push_back(arg);
//    }
//    else if( arg == "-options" || arg == "-xbo" )
//    {
//      if( i==argc-1 )
//      {
//        stringstrream err;
//        err << arg << " option requires a (filename) argument";
//        throw runtime_error( arg.str().c_str() );
//      }
//      xbo_filename = argv[++i];
//    }
//    else if( arglen>5 && arg[0]=='-' && string(arg,arglen-4)==".xbo" )
//    {
//      xbo_filename = string(arg,1);
//    }
//    else
//    {
//      rval.push_back(arg);
//    }
//
//    if( ! xbo_filenme.empty() )
//    {
//      set<vector> loaded_xbo_files;
//      vector<string> xbo_args = load_xbo_file(xbo_filename, loaded_xbo_files);
//
//      rval.insert( args.end(), xbo_args.begin(), xbo_args.end() );
//    }
//  }
//
//  return rval;
//}
