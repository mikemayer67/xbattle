#ifndef _XBATTLE_H_
#define _XBATTLE_H_

#include "commandline.h"
#include "options.h"

class XBattle
{
  public:
    XBattle(int argc,const char **argv);

    void run(void);

  private:
    CommandLine _command_line;
    Options     _options;
};

#endif // _XBATTLE_H_
