#include "player.h"
#include "team.h"
#include "option.h"

using namespace std;

Player::Player(string name) : 
  _name(name), _cellSize(45), _draw(DrawSimple), _posX(-1), _posY(-1) 
{}

Player::Player(shr<Team> &team, string name ) : 
  _name(name), _team(team), _cellSize(45), _draw(DrawSimple), _posX(-1), _posY(-1)
{}

bool Player::parseOption(const Option &opt)
{
  bool rval = true;

  if     ( opt=="cell" )   { _cellSize = opt.getInt(15,100);            }
  else if( opt=="xpos" )   { _posX     = opt.getInt(0);                 }
  else if( opt=="ypos" )   { _posY     = opt.getInt(0);                 }

  else if( opt=="manpos" ) { opt.validateNoArgs();  _posX = _posY = -1; }

  else if( opt=="draw" )
  {
    switch( opt.getInt(0,4) )
    {
      case 0:  _draw = DrawSimple;  break;
      case 1:  _draw = DrawBacking; break;
      case 2:  _draw = DrawPixmap;  break;
      case 3:  _draw = DrawWindow;  break;
      case 4:  _draw = DrawMasking; break;
      default: _draw = DrawSimple;  break; // but should never see this
    }
  }

  else { rval = false; }

  return rval;
}
