#include "xbattle.h"

#include <iostream>
#include <sstream>
#include <exception>

#define MINBOARDSIZE 5

using namespace std;

XBattle::XBattle(const CommandLine &cl) : _commandLine(cl),
  _drawTroopsAsArea(false), _boardWidth(15), _boardHeight(15), _tileShape(Square),
  _farmRate(0), _townDensity(0), _rbaseRange(2),
  _terrainType(Flat), _terrainHeight(5), _peaks(4),_peakBias(1.),
  _hasSea(false), _seaDepth(4), _seaFrac(0.), _troughBias(1.),
  _seed( time(NULL) + getpid() ),
  _speed(5), _step(false),
  _overwrite(false), _dumpFile("xbattle.xbt"),
  _minLevel(0), _maxLevel(0), _maxViewRange(0)
{
  Color::add( "black",     0,   0,   0 );
  Color::add( "white",   255, 255, 255, "black" );
  Color::add( "back",    210, 220, 150, "black" );
  Color::add( "mark",      0,   0,   0 );
  Color::add( "border",  192, 192, 192 );
  Color::add( "map",     255,   0, 255 );
  Color::add( "dark",    140, 140, 140, "white" );
  Color::add( "light",   180, 180, 180, "black" );
  Color::add( "gator",    50, 200, 100, "white" );
  Color::add( "brick",   200,   0,   0, "white" );
  Color::add( "red",     255,   0,   0, "black" );
  Color::add( "cyan",    100, 255, 210, "black" );
  Color::add( "magenta", 255,   0, 255, "black" );
  Color::add( "green",   130, 255,   0, "black" );
  Color::add( "blue",      0,   0, 175, "white" );
  Color::add( "yellow",  250, 250, 130, "black" );

  Color::setInverseFor("black","white");

  // determine terrin type

  StringSet_t keys = cl.optionKeys();
  for(StringSet_t::const_iterator key=keys.begin(); key!=keys.end(); ++key)
  {
    TerrainType_t type(Flat);
    if     ( key->substr(0,4)=="hill"  ) type=Hills;
    else if( key->substr(0,6)=="forest") type=Forest;
    if( type!=Flat )
    {
      if( _terrainType!=Flat && _terrainType!=type )
        throw runtime_error("Cannot specify both hill and forrest options");

      _terrainType=type;
    }
  }
  
  // loop through options and set game, team, and player attributes accordingly

  const OptionList_t &opts = cl.options();
  for( OptionList_t::const_iterator opt=opts.begin(); opt!=opts.end(); ++opt )
  {
    bool subOptionsAllowed=false;

    bool        parsed = parseGameOption(*opt);
    if(!parsed) parsed = parseColorOption(*opt);
    if(!parsed) parsed = _allPlayers.parseOption(*opt);
    if(!parsed) parsed = _allTeams.parseOption(*opt);
    if(!parsed && Color::exists(opt->key()))
    {
      parsed = parseNewPlayer(*opt);
      subOptionsAllowed = parsed;
    }

    if(!parsed)
      throw runtime_error(string("Unrecognized option -") + opt->key());

    if( ! (subOptionsAllowed || opt->subOptions().empty()) )
    {
      stringstream err;
      err << "The -" << opt->key() << " option does not accept suboption groups";
      throw runtime_error(err.str());
    }
  }

  // complete terrain palettes

  if( _terrainType==Hills ) 
  {
    Color::fillPalette( _terrainColors, _terrainHeight, RGB(175,220,100), RGB(200,175,100) );
  }
  else if( _terrainType==Forest )
  {
    Color::fillPalette( _terrainColors, _terrainHeight, RGB(0,200,0), RGB(0,120,0) );
  }

  if( _hasSea )
  {
    Color::fillPalette( _seaColors, _seaDepth, RGB(70,132,200), RGB(30,65,185) );
    while(_seaFrac>=1.) _seaFrac/=10.;
  }

  //
  // Compute derived option values
  //
  //
  
  _loopDelay = 25000./_speed + 0.5;

  _teams.resolveFractionalCosts();
  _teams.resolveFractionalDistance( min(_boardWidth, _boardHeight) );

  _maxViewRange = _teams.maxViewRange();
}

bool XBattle::parseGameOption(const Option &opt)
{
  bool rval = true;

  if     ( opt=="area" )          { opt.validateNoArgs(); _drawTroopsAsArea = true;  }
  else if( opt=="noarea" )        { opt.validateNoArgs(); _drawTroopsAsArea = false; }
  else if( opt=="overwrite" )     { opt.validateNoArgs(); _overwrite        = true;  }
  else if( opt=="nooverwrite" )   { opt.validateNoArgs(); _overwrite        = false; }
  else if( opt=="step" )          { opt.validateNoArgs(); _step             = true;  }
  else if( opt=="nostep" )        { opt.validateNoArgs(); _step             = false; }

  else if( opt=="diamond" )       { opt.validateNoArgs(); _tileShape = Diamond;  }
  else if( opt=="hex" )           { opt.validateNoArgs(); _tileShape = Hex;      }
  else if( opt=="octagon" )       { opt.validateNoArgs(); _tileShape = Octagon;  }
  else if( opt=="square" )        { opt.validateNoArgs(); _tileShape = Square;   }
  else if( opt=="triangle" )      { opt.validateNoArgs(); _tileShape = Triangle; }

  else if( opt=="seed" )          { _seed         = opt.getULong();  }
  else if( opt=="speed" )         { _speed        = opt.getDouble(0.1,20.); }

  else if( opt=="boardx" )        { _boardWidth   = opt.getInt(MINBOARDSIZE); }
  else if( opt=="boardy" )        { _boardHeight  = opt.getInt(MINBOARDSIZE); }
  else if( opt=="board" )         { _boardWidth   = _boardHeight = opt.getInt(MINBOARDSIZE); }

  else if( opt=="farms" )         { _farmRate     = opt.getInt(1); }
  else if( opt=="towns" )         { _townDensity  = opt.getInt(1); }
  else if( opt=="rbaserange" )    { _rbaseRange   = opt.getInt(0); }

  else if( opt=="dump" )          { _dumpFile     = opt.getFilename();              }
  else if( opt=="load" )          { _loadFile     = opt.getFilename("xbattle.xbt"); }
  else if( opt=="store" )         { _storeFile    = opt.getFilename("xbattle.xba"); }

  else if( opt=="forestlevels" )  { _terrainHeight = opt.getInt(2);         }
  else if( opt=="hilllevels" )    { _terrainHeight = opt.getInt(2);         }
  else if( opt=="peaks" )         { _peaks         = opt.getInt(1);         }
  else if( opt=="peakbias" )      { _peakBias      = opt.getDouble(0.,2.);  }

  else if( opt=="seadepth" )      { _seaDepth     = opt.getInt(2);        _hasSea = true; }
  else if( opt=="seafrac" )       { _seaFrac      = opt.getDouble(0.,.9); _hasSea = true; }
  else if( opt=="troughbias" )    { _troughBias   = opt.getDouble(0.,2.); _hasSea = true; }

  else if( opt=="hillcolor" )     { opt.getColor(_terrainColors); }
  else if( opt=="forestcolor" )   { opt.getColor(_terrainColors); }
  else if( opt=="seacolor" )      { opt.getColor(_seaColors);     }
  
  else { rval = false; }

  return rval;
}

bool XBattle::parseColorOption(const Option &opt)
{
  bool rval = false;
  if(opt == "color")
  {
    string name,inverse;
    RGB    color;
    opt.getColor(name,color,inverse);

    Color::add(name,color,inverse);

    rval = true;
  }
  else if(opt == "colorinverse" )
  {
    StringList_t args = opt.getArgs(2);
    Color::setInverseFor(args.at(0), args.at(1));
    rval = true;
  }
  return rval;
}

bool XBattle::parseNewPlayer(const Option &opt)
{
  bool rval=true;

  string team_name   = opt.key();
  string player_name = opt.getString();

  TeamPtr_t   team;
  for( vector<TeamPtr_t>::iterator t=_teams.begin(); team.isNull() && t!=_teams.end(); ++t )
  {
    if( (*t)->name() == team_name ) team = *t;
  }
  if( team.isNull() )
  {
    team = new Team(_allTeams, team_name);
    _teams.push_back(team);
  }

  PlayerPtr_t player(0);
  
  if( player_name != "you" )
  {
    string::size_type pos = player_name.find_last_of(':');
    if( pos==string::npos )
    {
      if( player_name.find_first_not_of("0123456789.",pos) != string::npos )
      {
        stringstream err;
        err << "'" << player_name.substr(pos) << "' is not a valid screen display identifier";
        throw(err.str());
      }
    }
    player = new Player(_allPlayers, team, player_name);
    team->addPlayer(player);
  }

  const vector<Option> &so = opt.subOptions();
  for( vector<Option>::const_iterator opt=so.begin(); opt!=so.end(); ++opt )
  {
    bool parsed = false;
    if( player.isSet() ) { parsed = player->parseOption(*opt); }
    if( !parsed        ) { parsed = team->parseOption(*opt);   }

    if( !parsed )
    {
      stringstream err;
      err << "The -" << opt->key() << " option is not a valid team or player option";
      throw runtime_error(err.str());
    }
  }

  return rval;
}

void XBattle::run(void)
{
  if( _teams.empty() )
    throw runtime_error("Must specify at least one team");
  
  std::cout << "done" << std::endl;
}
