#include "team.h"
#include "player.h"
#include "option.h"

#include <sstream>

using namespace std;

Team::Team(string name) : //_name(name),
  _armies(0), _militia(0), _bases(0), _rbases(0),
  _hasHorizon(false), _horizon(2), _hidden(false), 
  _basemap(false),_localmap(false), _map(false),
  _allowBuild(false), _buildCost(18), _buildCostFrac(0.),
  _buildSegs(4), _buildLimit(12),
  _allowScuttle(false), _scuttleSegs(5), _scuttleCost(10), _scuttleCostFrac(0.),
  _allowDig(false), _digCost(18), _digCostFrac(0.), 
  _allowFill(false), _fillCost(18), _fillCostFrac(0.),
  _hasErode(false), _erodeGate(20), _erodeThresh(10), _decay(0.),
  _move(3), _digin(0), _forest(0), _hills(0),
  _allowAttack(false), _allowBound(false), _allowManage(false),
  _allowMarch(false), _marchDelay(3),
  _allowRepeat(true), _allowReserve(false),
  _disrupt(false), _fight(5), _nospigot(false), _nospigotRatio(5),
  _allowGuns(false), _gunRange(0), _gunRangeFrac(0.), _gunCost(2), _gunDamage(1),
  _allowPara(false), _paraRange(0), _paraRangeFrac(0.), _paraCost(2), _paraDamage(1),
  _grid(true), _maxval(20), _wrap (false),
  _build_count(0), _troop_count(0), _cell_count(0)
{
}

Team::Team(const Team &x, string name) : //_name(name), 
  _armies(x._armies), _militia(x._militia), _bases(x._bases), _rbases(x._rbases),
  _hasHorizon(x._hasHorizon), _horizon(x._horizon), _hidden(x._hidden),
  _basemap(x._basemap), _localmap(x._localmap), _map(x._map),
  _allowBuild(x._allowBuild), _buildSegs(x._buildSegs), _buildCost(x._buildCost), 
  _buildCostFrac(x._buildCostFrac), _buildLimit(x._buildLimit),
  _allowScuttle(x._allowScuttle), _scuttleSegs(x._scuttleSegs),
  _scuttleCost(x._scuttleCost), _scuttleCostFrac(x._scuttleCostFrac),
  _allowDig(x._allowDig), _digCost(x._digCost), _digCostFrac(x._digCostFrac),
  _allowFill(x._allowFill), _fillCost(x._fillCost), _fillCostFrac(x._fillCostFrac),
  _hasErode(x._hasErode), _erodeGate(x._erodeGate), _erodeThresh(x._erodeThresh), _decay(x._decay),
  _move(x._move), _digin(x._digin), _forest(x._forest), _hills(x._hills),
  _allowAttack(x._allowAttack), _allowBound(x._allowBound), _allowManage(x._allowManage),
  _allowMarch(x._allowMarch), _marchDelay(x._marchDelay),
  _allowRepeat(x._allowRepeat), _allowReserve(x._allowReserve),
  _disrupt(x._disrupt), _fight(x._fight), _nospigot(x._nospigot), _nospigotRatio(x._nospigotRatio),
  _allowGuns(x._allowGuns), _gunRange(x._gunRange), _gunRangeFrac(x._gunRangeFrac),
  _gunCost(x._gunCost), _gunDamage(x._gunDamage), 
  _allowPara(x._allowPara), _paraRange(x._paraRange), _paraCost(x._paraCost), _paraDamage(x._paraDamage),
  _grid(x._grid), _maxval(x._maxval), _wrap(x._wrap),
  _build_count(0), _troop_count(0), _cell_count(0)
{
}

void Team::addPlayer(PlayerPtr_t &p)
{
  for( vector<PlayerPtr_t>::iterator x=_players.begin(); x!=_players.end(); ++x )
  {
    if( (*x)->name() == p->name() )
    {
      stringstream err;
      err << "Cannot add more than one player on display " << p->name() << " to team " << name();
      throw runtime_error(err.str());
    }
  }

  _players.push_back(p);
}

bool Team::parseOption(const Option &opt)
{
  bool rval = true;

  if     ( opt=="armies")        { _armies  = opt.getInt(1); }
  else if( opt=="militia")       { _militia = opt.getInt(1); }
  else if( opt=="bases" )        { _bases   = opt.getInt(1); }
  else if( opt=="rbases" )       { _rbases  = opt.getInt(1); }

  else if( opt=="horizon" )      { opt.getOptInt( _horizon, 1 );                       _hasHorizon=true;    } 
  else if( opt=="nohorizon" )    { opt.validateNoArgs();                               _hasHorizon=false;   } 
  else if( opt=="hidden" )       { opt.validateNoArgs();                               _hidden=true;        }
  else if( opt=="nohidden" )     { opt.validateNoArgs();                               _hidden=false;       }
  else if( opt=="unhidden" )     { opt.validateNoArgs();                               _hidden=false;       }

  else if( opt=="basemap" )      { opt.validateNoArgs();                               _basemap=true;       } 
  else if( opt=="localmap" )     { opt.validateNoArgs();                               _localmap=true;      } 
  else if( opt=="map" )          { opt.validateNoArgs();                               _map=true;           } 

  else if( opt=="build" )        { opt.getOptInt( _buildSegs, 1 );                     _allowBuild=true;    }
  else if( opt=="nobuild" )      { opt.getOptInt( _buildSegs, 1 );                     _allowBuild=false;   }
  else if( opt=="buildcost" )    { opt.getIntOrFrac( _buildCost, _buildCostFrac );     _allowBuild=true;    }
  else if( opt=="buildlimit" )   { _buildLimit = opt.getInt(1);                        _allowBuild=true;    }
  else if( opt=="scuttle" )      { opt.getOptInt( _scuttleSegs, 1);                    _allowScuttle=true;  }
  else if( opt=="noscuttle" )    { opt.validateNoArgs();                               _allowScuttle=false; }
  else if( opt=="scuttlecost" )  { opt.getIntOrFrac( _scuttleCost, _scuttleCostFrac ); _allowScuttle=true;  }

  else if( opt=="dig" )          { opt.validateNoArgs();                               _allowDig=true;      }
  else if( opt=="nodig" )        { opt.validateNoArgs();                               _allowDig=false;     }
  else if( opt=="digcost" )      { opt.getIntOrFrac( _digCost, _digCostFrac );         _allowDig=true;      }
  else if( opt=="fill" )         { opt.validateNoArgs();                               _allowFill=true;     }
  else if( opt=="nofill" )       { opt.validateNoArgs();                               _allowFill=false;    }
  else if( opt=="fillcost" )     { opt.getIntOrFrac( _fillCost, _fillCostFrac );       _allowFill=true;     }

  else if( opt=="erode" )        { opt.getOptInt( _erodeGate, 0 );                     _hasErode=true;      }
  else if( opt=="noerode" )      { opt.validateNoArgs();                               _hasErode=false;     }
  else if( opt=="erodethresh" )  { _erodeThresh = opt.getProbability();                _hasErode=true;      }
  else if( opt=="decay" )        { _decay       = opt.getProbability();                                     }
  else if( opt=="nodecay" )      { _decay = 0.;                                                             }

  else if( opt=="move" )         { _move   = opt.getInt(1,10); }
  else if( opt=="digin" )        { _digin  = opt.getInt(1,10); }
  else if( opt=="forest" )       { _forest = opt.getInt(1,10); }
  else if( opt=="hills" )        { _hills  = opt.getInt(1,10); }

  else if( opt=="attack" )       { opt.validateNoArgs();                               _allowAttack=true;   }
  else if( opt=="noattack" )     { opt.validateNoArgs();                               _allowAttack=false;  }
  else if( opt=="bound" )        { opt.validateNoArgs();                               _allowBound=true;    }
  else if( opt=="nobound" )      { opt.validateNoArgs();                               _allowBound=false;   }
  else if( opt=="manage" )       { opt.validateNoArgs();                               _allowManage=true;   }
  else if( opt=="nomanage" )     { opt.validateNoArgs();                               _allowManage=false;  }
  else if( opt=="march" )        { opt.getOptInt(_marchDelay, 1 );                     _allowMarch=true;    }
  else if( opt=="nomarch" )      { opt.validateNoArgs();                               _allowMarch=false;   }
  else if( opt=="repeat" )       { opt.validateNoArgs();                               _allowRepeat=true;   }
  else if( opt=="norepeat" )     { opt.validateNoArgs();                               _allowRepeat=false;  }
  else if( opt=="reserve" )      { opt.validateNoArgs();                               _allowReserve=true;  }
  else if( opt=="noreserve" )    { opt.validateNoArgs();                               _allowReserve=false; }

  else if( opt=="disrupt" )      { opt.validateNoArgs();                               _disrupt=true;       }
  else if( opt=="nodisrupt" )    { opt.validateNoArgs();                               _disrupt=false;      }
  else if( opt=="fight" )        { _fight = opt.getInt(1,10);                                               }
  else if( opt=="nospigot" )     { opt.getOptInt(_nospigotRatio);                      _nospigot=true;      }
  else if( opt=="spigot" )       { opt.validateNoArgs();                               _nospigot=false;     }

  else if( opt=="guns" )         { opt.getIntOrFrac(_gunRange,_gunRangeFrac,true);     _allowGuns=true;     }
  else if( opt=="noguns" )       { opt.validateNoArgs();                               _allowGuns=false;    }
  else if( opt=="gunscost" )     { _gunCost = opt.getInt(1);                           _allowGuns=true;     }
  else if( opt=="gunsdamage" )   { _gunDamage = opt.getInt(1);                         _allowGuns=true;     }
  else if( opt=="para" )         { opt.getIntOrFrac(_paraRange,_paraRangeFrac,true);   _allowPara=true;     }
  else if( opt=="nopara" )       { opt.validateNoArgs();                               _allowPara=false;    }
  else if( opt=="paracost" )     { _paraCost = opt.getInt(1);                          _allowPara=true;     }
  else if( opt=="paradamage" )   { _paraDamage = opt.getInt(1);                        _allowPara=true;     }

  else if( opt=="grid" )         { opt.validateNoArgs();                               _grid=true;          }
  else if( opt=="nogrid" )       { opt.validateNoArgs();                               _grid=false;         }
  else if( opt=="maxval" )       { _maxval = opt.getInt(1);                                                 }
  else if( opt=="wrap" )         { opt.validateNoArgs();                               _wrap = true;        }
  else if( opt=="nowrap" )       { opt.validateNoArgs();                               _wrap = false;       }

  else { rval = false; }

  return rval;
}

void Team::validateDrawMethod(TileShape_t shape)
{
  for(vector<PlayerPtr_t>::iterator x=_players.begin(); x!=_players.end(); ++x)
    (*x)->validateDrawMethod(shape);
}
