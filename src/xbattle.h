#ifndef _XBATTLE_H_
#define _XBATTLE_H_

#include "types.h"
#include "commandline.h"
#include "player.h"
#include "team.h"
#include "teams.h"

class XBattle
{
  public:
    XBattle(const CommandLine &);

    void run(void);
    void edit(std::string xbt);
    void replay(StringList_t displays);

  private:
    bool parseGameOption(const Option &opt);
    bool parseColorOption(const Option &opt);
    bool parseNewPlayer(const Option &opt);

    void add_x_color(std::string color);

  private:

    const CommandLine &_commandLine;

    bool             _drawTroopsAsArea;  // whether to draw troops proportional to area

		int              _boardWidth;
		int              _boardHeight;
    TileShape_t      _tileShape;

    int              _farmRate;          // rate at which troops (slowly) grow
    int              _townDensity;       // density of randomly distributed towns
    int              _rbaseRange;        // min distance between random bases and enemy

    TerrainType_t    _terrainType;
    int              _terrainHeight;     // (or density if type==Forest)
    int              _peaks;             // number of terrain peaks (hills or forest)
    double           _peakBias;          // terrain elevation bias (0-2)
    Palette_t        _terrainColors;     // colors with which to render terrain

    bool             _hasSea;
    int              _seaDepth;          // maximum sea depth
    double           _seaFrac;           // fraction of board covered by sea
    double           _troughBias;        // sea depth bias (0-2)
    Palette_t        _seaColors;         // color with which each sea level is rendered

    unsigned long    _seed;              // srandom seed value
    double           _speed;             // game speed (0.1-20.)
    unsigned long    _loopDelay;         // derived from loopDelay (microsec)
    bool             _step;              // manual board updates only

    bool             _overwrite;         // use terrain from load file
    std::string      _dumpFile;          // xbt file used when a game state is saved during play
    std::string      _storeFile;         // xba file into which to store data for replay
    std::string      _loadFile;          // xbt file used to setup the game

    int              _minLevel;     // if sea, depth of sea (negative), otherwise 0
    int              _maxLevel;     // if terrain, height of hills or density of forest, otherwise 0
    int              _maxViewRange; // maximum range 

  private:

    Team    _allTeams;
    Player  _allPlayers;

    Teams   _teams;
};

#endif // _XBATTLE_H_
