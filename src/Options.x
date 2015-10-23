#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <vector>
#include <string>

typedef std::vector<std::string> Strings;
typedef Strings::iterator        StringIter;


class CommandLineArgs : Strings
{
  public:
    CommandLineArgs(int argc,const char **argv);

    const std::string &proc(void) const { return _proc; }

  private:
    std::string _proc;
};

class Options
{
  public:
    Options(void) {}

    virtual bool parse( CommandLineArgs &args ) = 0;

    unsigned int countParameters( StringIter arg, StringIter end )




class DeprecatedOptions : public Options
{
  public:
    DeprecatedOptions(void)
    {
      

		{"-border",		 1, FALSE,  0,  0.0},
		{"-dump",		 1, FALSE,  0,  0.0},
		{"-sea_block",		 0, FALSE,  0,  0.0},
		{"-stipple",		 9, FALSE,  0,  0.0},
}

class PlayerOptions
{
		{"-cell",		 1, FALSE, 45, 45.0},
		{"-xpos",		 1, FALSE,  0,  0.0},
		{"-ypos",		 1, FALSE,  0,  0.0},
}

class SideOptions
{
  public:
  protected:
		{"-attack",		 0, FALSE,  0,  0.0},
		{"-basemap",		 0, FALSE,  0,  0.0},
		{"-bound",		 0, FALSE,  0,  0.0},
		{"-build",		 1, FALSE,  4,  4.0},
		{"-build_cost",		 1, FALSE, 18, 18.0},
		{"-build_limit",	 1, FALSE, 12, 12.0},
		{"-decay",		 1, FALSE,  0,  0.0},
		{"-dig",		99, FALSE,  1,  1.0},
		{"-dig_cost",		 1, FALSE, 18, 18.0},
		{"-digin",		 1, FALSE,  0,  0.0},
		{"-disrupt",		 0, FALSE,  0,  0.0},
		{"-draw",		 1, FALSE,  0,  0.0},
		{"-erode",		 1, FALSE, 20, 20.0},
		{"-erode_thresh",	 1, FALSE, 10, 10.0},
		{"-fight",		 1, FALSE,  5,  5.0},
		{"-fill",		99, FALSE,  1,  1.0},
		{"-fill_cost",		 1, FALSE, 18, 18.0},
		{"-grid",		 0, TRUE,   0,  0.0},
		{"-guns",		 1, FALSE,  0,  0.0},
		{"-guns_cost",		 1, FALSE,  2,  2.0},
		{"-guns_damage",	 1, FALSE,  1,  1.0},
		{"-hidden",		 0, FALSE,  0,  0.0},
		{"-horizon",		99, FALSE,  2,  2.0},
		{"-localmap",		 0, FALSE,  0,  0.0},
		{"-manage",		 0, FALSE,  0,  0.0},
		{"-manpos",		 0, FALSE,  0,  0.0},
		{"-map",		 0, FALSE,  0,  0.0},
		{"-march",		 1, FALSE,  3,  3.0},
		{"-maxval",		 1, FALSE, 20, 20.0},
		{"-militia",		 1, FALSE,  0,  0.0},
		{"-move",		 1, FALSE,  3,  3.0},
		{"-nospigot",		99, FALSE,  5,  5.0},
		{"-para",		 1, FALSE,  0,  0.0},
		{"-para_cost",		 1, FALSE,  3,  3.0},
		{"-para_damage",	 1, FALSE,  1,  1.0},
		{"-rbases",		 1, FALSE,  0,  0.0},
		{"-repeat",		 0, TRUE,   0,  0.0},
		{"-reserve",		 0, FALSE,  0,  0.0},
		{"-scuttle",		99, FALSE,  5,  5.0},
		{"-scuttle_cost",	 1, FALSE, 10,  1.0},
		{"-wrap",		 0, FALSE,  0,  0.0}};
};

class GameOptions
{
		{"-area",		 0, FALSE,  0,  0.0},
		{"-armies",		 1, FALSE,  0,  0.0},
		{"-bases",		 1, FALSE,  0,  0.0},
		{"-board",		 1, FALSE, 15, 15.0},
		{"-boardx",		 1, FALSE, 15, 15.0},
		{"-boardy",		 1, FALSE, 15, 15.0},
		{"-cell",		 1, FALSE, 45, 45.0},
		{"-color",		 4, FALSE,  0,  0.0},
		{"-color_inverse",	 2, FALSE,  0,  0.0},
		{"-diamond",		 0, FALSE,  0,  0.0},
		{"-edit",		99, FALSE,  0,  0.0},
		{"-farms",		 1, FALSE,  0,  0.0},
		{"-forest",		 1, FALSE,  0,  0.0},
		{"-forest_color",	 4, FALSE,  0,  0.0},
		{"-forest_tones",	 1, FALSE,  5,  5.0},
		{"-hex",		 0, FALSE,  0,  0.0},
		{"-hills",		 1, FALSE,  0,  0.0},
		{"-hill_color",		 4, FALSE,  0,  0.0},
		{"-hill_tones",		 1, FALSE,  5,  5.0},
		{"-load",		99, FALSE,  0,  0.0},
		{"-octagon",		 0, FALSE,  0,  0.0},
		{"-options",		99, FALSE,  0,  0.0},
		{"-overwrite",		 0, FALSE,  0,  0.0},
		{"-peaks",		 1, FALSE,  4,  4.0},
		{"-peak_bias",		 1, FALSE,  1,  1.0},
		{"-rbase_range",	 1, FALSE,  2,  2.0},
		{"-replay",		99, FALSE,  0,  0.0},
		{"-sea",		 1, FALSE,  0,  0.0},
		{"-sea_color",		 4, FALSE,  0,  0.0},
		{"-sea_tones",		 1, FALSE,  4,  4.0},
		{"-sea_value",		 1, FALSE,  0,  0.8284},
		{"-seed",		 1, FALSE,  0,  0.0},
		{"-speed",		 1, FALSE,  5,  5.0},
		{"-square",		 0, FALSE,  0,  0.0},
		{"-step",		 0, FALSE,  0,  0.0},
		{"-store",		99, FALSE,  0,  0.0},
		{"-towns",		 1, FALSE,  0,  0.0},
		{"-triangle",		 0, FALSE,  0,  0.0},
		{"-trough_bias",	 1, FALSE,  1,  1.0},

char *Usage[] = {
  "USAGE: xbattle <args>",
  "-<c1>           <display>         display name",
  "-<c1>_<c2>      <display>         display name",
  "-area                             troop strength proportional to area",
  "-attack                           allow use of attack key",
  "-armies         <int>             place ordered armies",
  "-basemap                          use map scheme, bases visible",
  "-bases          <int>             place ordered bases",
  "-board          <int>             size of board (square)",
  "-boardx         <int>             width of board (in cells)",
  "-boardy         <int>             height of board (in cells)",
  "-border         <int>             border around board",
  "-bound                            allow drag-bounding direction sets",
  "-build          <int>             build cities, <int> segments to complete",
  "-build_cost     <int>             cost to build city segment",
  "-build_limit    <int>             limit cities each side can build",
  "-cell           <int>             diameter of cell",
  "-color          <str> <r><g><b>   set RGB values for color <str>",
  "-color_inverse  <str> <str2>      set color <str> inverse to <str2>",
  "-decay          <int>             make troops slowly die off",
  "-diamond                          use diamond tiling",
  "-dig            [int]             allow terrain lowering, [int] steps",
  "-dig_cost       <int>             cost of each dig step",
  "-digin          <int>             provide entrenchment",
  "-disrupt                          allow enemies to break supply lines",
  "-draw           <int>             specify a troop drawing method",
  "-dump           <file>            dump configuration to <file>",
  "-edit           [file]            interactively edit xbattle board",
  "-erode          <int>             make unused paths erode",
  "-erode_thresh   <int>             threshold for erosion",
  "-farms          <int>             troops slowly grow",
  "-fight          <int>             intensity of fighting",
  "-fill           [int]             allow terrain raising, [int] steps",
  "-fill_cost      <int>             cost of each fill step",
  "-forest         <int>             density of forest",
  "-forest_color   <int> <r><g><b>   RGB values for forest level <int>",
  "-forest_tones   <int>             number of allowable forest levels",
  "-grid                             show grid",
  "-guns           <int>             range of artillery",
  "-guns_cost      <int>             cost of each artillery shell",
  "-guns_damage    <int>             damage done by each artillery shell",
  "-help                             print this message",
  "-hex                              use hexagonal tiling",
  "-hidden                           can't see enemy direction vectors",
  "-hills          <int>             slope of hills",
  "-hill_color     <int> <r><g><b>   RGB values for hill level <int>",
  "-hill_tones     <int>             number of allowable hill levels",
  "-horizon        [int]             can't see enemy past [int] cells",
  "-load           [file]            load board from [file]",
  "-localmap                         use map scheme, disappearing terrain",
  "-manage                           allow managed control of operations",
  "-manpos                           enable manual positioning of board",
  "-map                              use basic map scheme",
  "-march          <int>             number of delays between marches",
  "-maxval         <int>             maximum cell troop capacity",
  "-militia        <int>             density of randomly distributed troops",
  "-move           <int>             speed of troop flow",
  "-nospigot       [int]             cease attack if enemy/you ratio > [int]",
  "-octagon                          use octagonal/square tiling",
  "-options        <file>            read xbattle options from <file>",
  "-opt_file.xbo                     shorthand for -options opt_file.xbo",
  "-overwrite                        just use terrain from loaded file",
  "-para           <int>             range of paratroopers",
  "-para_cost      <int>             cost of each paratrooper",
  "-para_damage    <int>             invading strength of each paratrooper",
  "-peaks          <int>             number of terrain peaks (and troughs)",
  "-peak_bias      <float>           peak distribution bias (0.0-2.0)",
  "-rbases         <int>             number of randomly distributed bases",
  "-rbase_range    <int>             minimum distance of rbase from enemy base",
  "-repeat                           allow repeat of last mouse command",
  "-replay         [file]            replay stored game from [file]",
  "-reserve                          allow reserve of troops",
  "-scuttle        [int]             enable city scuttling, [int] multiples",
  "-scuttle_cost   <int>             cost of scuttle",
  "-sea            <int>             pervasiveness (and levels) of sea",
  "-sea_block                        use block-fills rather than hue-fills",
  "-sea_color      <int> <r><g><b>   RGB values for sea level <int>",
  "-sea_tones      <int>             number of allowable sea levels",
  "-sea_value      <float>           darkness of seas for b/w games",
  "-seed           <int>             random number generator seed",
  "-speed          <int>             speed of updates",
  "-square                           use square tiling",
  "-stipple        <str> 8*<hex>     stipple (b/w) pattern for color <str>",
  "-store          [file]            store game in [file] for later replay",
  "-towns          <int>             density of randomly distributed towns",
  "-triangle                         use triangular tiling",
  "-trough_bias    <float>           trough distribution bias (0.0-2.0)",
  "-xpos           <int>             x position of board on display",
  "-ypos           <int>             y position of board on display",
  "-wrap                             allow wrapping around edges of board",
}

#endif // _OPTIONS_H_
