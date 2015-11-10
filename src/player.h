#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "types.h"
#include "SharePtr.h"

class Team;
class Option;

class Player
{
  public:
    Player(std::string name="");
    Player(const Player &x, shr<Team> &team, std::string name="");

    std::string name(void) const { return _name; }

    bool parseOption(const Option &opt);

  private:
    std::string  _name;  // actually name of the display
    shr<Team>    _team;

    int          _cellSize;
    DrawMethod_t _draw;
    int          _posX;
    int          _posY;
};

typedef shr<Player> PlayerPtr_t;

#endif // _PLAYER_H_
