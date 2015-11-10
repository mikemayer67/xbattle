#ifndef _TEAM_H_
#define _TEAM_H_

#include <vector>
#include "types.h"
#include "player.h"

class Option;

class Team
{
  public:
    Team(std::string name="");
    Team(const Team &ref, std::string name="");

    std::string name(void) const { return _name; }

    bool parseOption(const Option &opt);

    void addPlayer(PlayerPtr_t &p);

  private:
    std::string _name;       // actually color
    
    int    _armies;          // number of initial armies assigned to team
    int    _militia;         // number of initial militas assigned to team
    int    _bases;           // number of bases assigned to team
    int    _rbases;          // number of remote bases assigned to team

    bool   _hasHorizon;      // limits visibility of board activity
    int    _horizon;         // distance at which board activity can be seen 
    bool   _hidden;          // movement vectors are hidden from enemy
    bool   _basemap;         // bases are not drawn until seen
    bool   _localmap;        // limits visibility of board terrain
    bool   _map;             // terrain not shown until seen

    bool   _allowBuild;      // team can build towns
    int    _buildSegs;       // number of segments for town construction
    int    _buildCost;       // number of troops it costs to build a town
    double _buildCostFrac;   // (cost specified as fraction of max value)
    int    _buildLimit;      // number of towns a team can build
    bool   _allowScuttle;    // team can dismantle a town
    int    _scuttleSegs;     // number of segments for town dismantle (if ~build)
    int    _scuttleCost;     // number of troops it costs to dismantle a town
    double _scuttleCostFrac; // (cost specified as fraction of max value)

    bool   _allowDig;        // team can lower terrain
    int    _digCost;         // number of troops it costs to lower terrain
    double _digCostFrac;     // (cost specified as fraction of max value)
    bool   _allowFill;       // team can raise terrain
    int    _fillCost;        // number of troops it costs to raise terrain
    double _fillCostFrac;    // (cost specified as fraction of max value)

    bool   _hasErode;        // Old movement vectors die off
    int    _erodeGate;       // number of moves before erosion begins
    double _erodeThresh;     // probability of erosion once allowed
    double _decay;           // probability of troops decaying on any given move

    int    _move;            // troop mobility
    int    _digin;           // impact to troop movement into new cells
    int    _forest;          // impact forest has on troop movement
    int    _hills;           // impact hills have on troop movement

    bool   _allowAttack;     // allows use of attack key
    bool   _allowBound;      // allows use of draggable command boundary
    bool   _allowManage;     // allows automated troop management
    bool   _allowMarch;      // allows automatic troop marches
    int    _marchDelay;      // delay in each cell before marching to next
    bool   _allowRepeat;     // allows repeat of last command with mouse button
    bool   _allowReserve;    // allows setting a reserve force with number key

    bool   _disrupt;         // attacks disrupt supply lines
    int    _fight;           // team's attack strength
    bool   _nospigot;        // calls off attack when outnumbered
    int    _nospigotRatio;   //  ratio which defines "outnumbered"

    bool   _allowGuns;       // allows use of artillery
    int    _gunRange;        // range of artillery
    double _gunRangeFrac;    // (range a fraction of board size)
    int    _gunCost;         // number of troops it costs to fire artillery
    int    _gunDamage;       // damage inflicted by artillery fire
    bool   _allowPara;       // allows use of paratroops
    int    _paraRange;       // range of paratroops
    double _paraRangeFrac;   // range of paratroops
    int    _paraCost;        // number of troops it costs to send paratroops
    int    _paraDamage;      // damage inflicted by paratroops 

    bool   _grid;            // display grid
    int    _maxval;          // maximum troops per cell
    bool   _wrap;            // movement wraps around edge of screen

  private:
    std::vector<PlayerPtr_t> _players;
};

typedef shr<Team> TeamPtr_t;

#endif // _TEAM_H_
