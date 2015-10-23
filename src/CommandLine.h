#ifndef _COMMANDLINE_H_
#define _COMMANDLINE_H_

#include "Util.h"

class CommandLine
{
  public:
    CommandLine(int argc,const char **argv);

    const std::string &proc(void) const { return _proc; }

    std::string find_file( std::string file ) const;
    std::string rel_to_abs( std::string path ) const;

  private:
    std::string _proc;
    std::string _path;
    std::string _cwd;
    StringList  _args;
};


#endif // _COMMANDLINE_H_ 
