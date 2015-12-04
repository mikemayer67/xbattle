#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "types.h"
#include "SharePtr.h"

#include <X11/X.h>
#include <X11/Xlib.h>

class Team;
class Option;

class Player
{
  public:
    Player(std::string display="");
    Player(const Player &x, shr<Team> &team, std::string display="");

    std::string name(void) const { return _display_id; }

    bool parseOption(const Option &opt);

  private:
    void open_display(void);

  private:
    std::string  _display_id;
    shr<Team>    _team;

    int          _cellSize;
    DrawMethod_t _draw;
    int          _posX;
    int          _posY;

  private:
    Display     *_display;
};

typedef shr<Player> PlayerPtr_t;

#endif // _PLAYER_H_
