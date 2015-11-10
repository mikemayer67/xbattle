#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include "types.h"
#include "option.h"
#include "team.h"
#include "player.h"

#include <map>

class Options
{
  public:
    Options(void);

    void init(const StringList_t &);

  private:
    bool parseGameOption(const Option &opt);
    bool parseNewPlayer(const Option &opt);
    void show_usage(const std::string msg="");

    void add_x_color(std::string color);

  private:
    bool             _area;          // whether to draw troops proportional to area

		int              _boardWidth;
		int              _boardHeight;
    TileShape_t      _tileShape;

    int              _farms;         // rate at which troops (slowly) grow
    int              _towns;         // density of randomly distributed towns

    bool             _hasForest;
    int              _forestLevels;  // maximum forest level (was forest tones in 5.4.1)
    std::vector<RGB> _forestColors;  // color with which each forest level is rendered

    bool             _hasHills;
    int              _hillLevels;    // maximum hill level (was hill tones in 5.4.1)
    std::vector<RGB> _hillColors;    // color with which each hill level is rendered

    bool             _hasSea;
    int              _seaLevels;     // maximum sea depth
    double           _seaFrac;       // sea pervasiveness (0-10)
    std::vector<RGB> _seaColors;     // color with which each sea level is rendered

    bool             _overwrite;     // use terrain from load file

    int              _peaks;         // number of terrain peaks (hills or forest)
    double           _peakBias;      // terrain elevation bias (0-2)
    double           _troughBias;    // sea depth bias (0-2)

    int              _rbaseRange;    // min distance between random bases and enemy

    unsigned long    _seed;          // srandom seed value
    double           _speed;         // game speed (0.1-20.)
    unsigned long    _loopDelay;     // derived from loopDelay (microsec)
    bool             _step;          // manual board updates only

    std::string      _editFile;
    std::string      _dumpFile;
    std::string      _loadFile;
    std::string      _replayFile;
    std::string      _storeFile;

    std::map<std::string,RGB>         _colors;
    std::map<std::string,std::string> _inverseColors;

  private:

    Team         _allTeams;
    Player       _allPlayers;

    std::vector<TeamPtr_t> _teams;
};

#endif // _OPTIONS_H_
