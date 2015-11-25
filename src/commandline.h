#ifndef _COMMANDLINE_H_
#define _COMMANDLINE_H_

////////////////////////////////////////////////////////////////////////////////
// CommandLine
//   Used to parse the command line to:
//   Extract the executable name
//   Attempt to infer the path to the executable
//   Expand out all of the arguements passed to the executable
//     Add spaces around curly brackets ( i.e. { and } )
//       ( this allows -junk{stuff and more} to be read as -junk { stuff and more } )
//     Recursively expands the contents of option files
//       recognizing -options xbo_file, -xbo xbo_file, or -xbo_file
//       where for -options and -xbo, the .xbo suffix is not require in the argument
//         All the following are equivalent
//           -options junk.xbo
//           -options junk
//           -xbo junk.xbo
//           -xbo junk
//           -junk.xbo
//         but not
//           -junk (there is not way to tell this is an xbo_file)
//
//   It does NOT, attempt to interpret these arguments.
//     This is handled by the Options class (and its cohorts).
////////////////////////////////////////////////////////////////////////////////

#include "types.h"
#include "option.h"

class CommandLine
{
  public:
    CommandLine(int argc,const char **argv);

    const std::string  &proc(void)    const { return _proc; }
    const StringList_t &args(void)    const { return _args; }
    const OptionList_t &options(void) const { return _options; }

    StringSet_t optionKeys(void) const;
    bool        optionWasSet( std::string s ) const;

    std::string find_file ( std::string file ) const;
    std::string rel_to_abs( std::string path ) const;

  private:
    std::string  _proc;
    StringList_t _args;
    OptionList_t _options;

    std::string  _procDir;
    std::string  _cwd;

    StringList_t _filePath;
};


#endif // _COMMANDLINE_H_ 
