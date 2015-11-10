#include "options.h"

#include <iostream>
#include <sstream>
#include <time.h>

#ifndef MINBOARDSIZE
#define MINBOARDSIZE 5
#endif

using namespace std;

// Support function prototypes

void fillPalette( vector<RGB> &, int n, RGB, RGB );

// Options methods

Options::Options(void) : 
  _area(false),
  _boardWidth(15), _boardHeight(15), _tileShape(Square),
  _farms(0), _towns(0), _rbaseRange(2),
  _hasForest(false),_forestLevels(5),
  _hasHills(false), _hillLevels(5),
  _peaks(4), _peakBias(1), 
  _hasSea(false), _seaFrac(0.), _seaLevels(4), _troughBias(1.),
  _speed(5), _step(false),
  _dumpFile("xbattle.xbt"),
  _seed(time(NULL)+getpid())
{
  _colors["back"]    = RGB( 210, 220, 150);
  _colors["black"]   = RGB(   0,   0,   0);
  _colors["white"]   = RGB( 255, 255, 255);
  _colors["mark"]    = RGB(   0,   0,   0);
  _colors["border"]  = RGB( 192, 192, 192);
  _colors["map"]     = RGB( 255,   0, 255);
  _colors["dark"]    = RGB( 140, 140, 140);
  _colors["light"]   = RGB( 180, 180, 180);
  _colors["fine"]    = RGB( 160, 160, 160);
  _colors["coarse"]  = RGB( 200, 200, 200);
  _colors["gator"]   = RGB(  50, 200, 100);
  _colors["brick"]   = RGB( 200,   0,   0);
  _colors["red"]     = RGB( 255,   0,   0);
  _colors["cyan"]    = RGB( 100, 255, 210);
  _colors["magenta"] = RGB( 255,   0, 255);
  _colors["green"]   = RGB( 130, 255,   0);
  _colors["blue"]    = RGB(   0,   0, 175);
  _colors["yellow"]  = RGB( 250, 250, 130);

  _inverseColors["back"]    =  "black";
  _inverseColors["black"]   =  "white";
  _inverseColors["white"]   =  "black";
  _inverseColors["mark"]    =  "";
  _inverseColors["border"]  =  "";
  _inverseColors["map"]     =  "";
  _inverseColors["dark"]    =  "white";
  _inverseColors["light"]   =  "black";
  _inverseColors["fine"]    =  "white";
  _inverseColors["coarse"]  =  "black";
  _inverseColors["gator"]   =  "white";
  _inverseColors["brick"]   =  "white";
  _inverseColors["red"]     =  "black";
  _inverseColors["cyan"]    =  "black";
  _inverseColors["magenta"] =  "black";
  _inverseColors["green"]   =  "black";
  _inverseColors["blue"]    =  "white";
  _inverseColors["yellow"]  =  "black";
}

void Options::init( const StringList_t &args )
{
  //
  // Walk through the argument list and convert them to known options
  //

  StringList_t::const_iterator pos = args.begin();
  StringList_t::const_iterator end = args.end();

  while(pos<end)
  {
    Option opt(pos,end);
    
    if( opt=="help" ) show_usage();
    try
    {
      bool subOptionsAllowed = false;

      bool        parsed = parseGameOption(opt);
      if(!parsed) parsed = _allPlayers.parseOption(opt);
      if(!parsed) parsed = _allTeams.parseOption(opt);
      if(!parsed)
      {
        parsed = parseNewPlayer(opt);
        subOptionsAllowed = parsed;
      }
      if(!parsed)
      {
        throw runtime_error(string("Unrecognized option -") + opt.key());
      }

      if( ! (subOptionsAllowed || opt.subOptions().empty()) )
      {
        throw runtime_error(string("The -")+opt.key()+" option does not accept suboption gropus");
      }

    }
    catch(const runtime_error &err)
    {
      show_usage(err.what());
    }
  }

  //
  //  Cleanup if needed
  //
  
  if(_hasHills && _hasForest)
    throw runtime_error("Cannot play with both hills and forest enabled");

  while( _seaFrac >= 1. ) _seaFrac/=10.;

  _loopDelay = 25000./_speed + 0.5;

  if(_hasForest) fillPalette(_forestColors, _forestLevels, RGB(175,220,100), RGB(200,175,100));
  if(_hasHills)  fillPalette(_hillColors,   _hillLevels,   RGB(  0,200,  0), RGB(  0,120,  0));
  if(_hasSea)    fillPalette(_seaColors,    _seaLevels,    RGB( 70,132,200), RGB( 30, 65,185));

  for( map<string,string>::iterator ix=_inverseColors.begin(); ix!=_inverseColors.end(); ++ix)
  {
    if( _colors.find(ix->first)  == _colors.end() )  add_x_color(ix->first);
    if( _colors.find(ix->second) == _colors.end() )  add_x_color(ix->second);
  }

  for( map<string,RGB>::iterator x=_colors.begin(); x!=_colors.end(); ++x)
  {
    if( _inverseColors.find(x->first) == _inverseColors.end() )
    {
      if( x->second.gray() >= 128 ) _inverseColors[x->first] = "black";
      else                          _inverseColors[x->first] = "white";
    }
  }
}

bool Options::parseGameOption(const Option &opt)
{
  bool rval = true;

  if     ( opt=="area" )          { opt.validateNoArgs(); _area      = true;     }
  else if( opt=="noarea" )        { opt.validateNoArgs(); _area      = false;    }
  else if( opt=="overwrite" )     { opt.validateNoArgs(); _overwrite = true;     }
  else if( opt=="nooverwrite" )   { opt.validateNoArgs(); _overwrite = false;    }
  else if( opt=="step" )          { opt.validateNoArgs(); _step      = true;     }
  else if( opt=="nostep" )        { opt.validateNoArgs(); _step      = false;    }

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

  else if( opt=="farms" )         { _farms        = opt.getInt(1); }
  else if( opt=="towns" )         { _towns        = opt.getInt(1); }
  else if( opt=="rbase_range" )   { _rbaseRange   = opt.getInt(0); }

  else if( opt=="dump" )          { _dumpFile     = opt.getFilename();              }
  else if( opt=="edit" )          { _editFile     = opt.getFilename("xbattle.xbt"); }
  else if( opt=="load" )          { _loadFile     = opt.getFilename("xbattle.xbt"); }
  else if( opt=="replay" )        { _replayFile   = opt.getFilename("xbattle.xbt"); }
  else if( opt=="store" )         { _storeFile    = opt.getFilename("xbattle.xbt"); }

  else if( opt=="forest_levels" ) { _forestLevels = opt.getInt(2); _hasForest = true; }
  else if( opt=="forest_tones" )  { _forestLevels = opt.getInt(2); _hasForest = true; }
  else if( opt=="hill_levels" )   { _hillLevels   = opt.getInt(2); _hasHills  = true; }
  else if( opt=="hill_tones" )    { _hillLevels   = opt.getInt(2); _hasHills  = true; }
  else if( opt=="peaks" )         { _peaks        = opt.getInt(1);                    }
  else if( opt=="peak_bias" )     { _peakBias     = opt.getDouble(0.,2.);             }

  else if( opt=="sea_levels" )    { _seaLevels    = opt.getInt(2);        _hasSea = true; }
  else if( opt=="sea_tones" )     { _seaLevels    = opt.getInt(2);        _hasSea = true; }
  else if( opt=="sea" )           { _seaFrac      = opt.getDouble(0.,9.); _hasSea = true; }
  else if( opt=="sea_frac" )      { _seaFrac      = opt.getDouble(0.,.9); _hasSea = true; }
  else if( opt=="trough_bias" )   { _troughBias   = opt.getDouble(0.,2.); _hasSea = true; }

  else if( opt=="hill_color" )    { opt.getColor(_hillColors);   _hasHills  = true; }
  else if( opt=="forest_color" )  { opt.getColor(_forestColors); _hasForest = true; }
  else if( opt=="sea_color" )     { opt.getColor(_seaColors);    _hasSea    = true; }
  
  else if( opt=="color" )         { opt.getColor(_colors); }
  else if( opt=="color_inverse")
  {
    StringList_t args = opt.getArgs(2);
    _inverseColors[args.at(0)] = args.at(1);
  }

  else { rval = false; }

  return rval;
}

bool Options::parseNewPlayer(const Option &opt)
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

  PlayerPtr_t player = new Player(_allPlayers, team, player_name);
  team->addPlayer(player);

  const vector<Option> &so = opt.subOptions();
  for( vector<Option>::const_iterator opt=so.begin(); opt!=so.end(); ++opt )
  {
    if( ! ( player->parseOption(*opt) || team->parseOption(*opt) ) )
    {
      stringstream err;
      err << "The -" << opt->key() << " option is not a valid team or player option";
      throw runtime_error(err.str());
    }
  }

  return rval;
}

void Options::show_usage(const string msg)
{
  ostream *s = &cout;

  bool isErr = !msg.empty();

  if(isErr)
  {
    s = &cerr;
    *s << endl << msg << endl;
  }

  *s << endl
    << "help text goes here..." << endl
    << endl;

  exit( isErr ? 1 : 0 );
}

void Options::add_x_color(string color)
{
  if(color.empty()) return;

  _colors[color] = RGB(color);
}

////////////////////////////////////////////////////////////////////////////////
// Support functions
////////////////////////////////////////////////////////////////////////////////

void fillPalette( vector<RGB> &colors, int levels, RGB min, RGB max )
{
  for(int i=colors.size(); i<levels; ++i) colors.push_back(RGB());

  if( ! colors.at(0)        ) colors.at(0)        = min;
  if( ! colors.at(levels-1) ) colors.at(levels-1) = max;

  for(int i=1; i<levels-1; ++i)
  {
    if( !colors.at(i) )
    {
      int j=i+1;
      while(!colors.at(j)) ++j;

      min = colors.at(i-1);
      max = colors.at(j);

      for(int k=i; k<j; ++k)
      {
        double frac = (1.+k-i)/(1.+j-i);
        colors.at(k) = RGB( min, max, frac );
      }
      i=j;
    }
  }
}

