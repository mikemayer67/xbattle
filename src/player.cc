#include "player.h"
#include "team.h"
#include "option.h"

#include <sstream>

using namespace std;

Player::Player(string display) : 
  _display_id(display), _cellSize(45), _draw(DrawSimple), _posX(-1), _posY(-1) 
{
  open_display();
}

Player::Player(const Player &x, shr<Team> &team, string display ) : 
  _display_id(display), _team(team), 
  _cellSize(x._cellSize), _draw(x._draw), _posX(x._posX), _posY(x._posY)
{
  open_display();
}

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

void Player::open_display(void)
{
  if(_display_id.empty()) // not a real player
  {
    _display=NULL;
    return;
  }

  _display = XOpenDisplay(_display_id.c_str());

  if( _display==NULL )
  {
    stringstream err;
    err << "Failed to open an X display on " << _display_id;
    throw runtime_error(err.str());
  }
}
