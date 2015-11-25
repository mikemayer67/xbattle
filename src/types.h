#ifndef _TYPES_H_
#define _TYPES_H_

#include <vector>
#include <set>
#include <string>

typedef unsigned long ulong;

typedef std::vector<std::string> StringList_t;
typedef std::set<std::string>    StringSet_t;

typedef enum { Square, Hex, Octagon, Diamond, Triangle } TileShape_t;
typedef enum { Flat, Hills, Forest }                     TerrainType_t;

typedef enum 
{ DrawSimple,    // Erase circle to terrain color and redraw (fast/flicker)
  DrawBacking,   // Same as FastRedraw, but in backpane with copy to window
  DrawPixmap,    // Render cell as pixmap and copy to window (only works w/ squares)
  DrawWindow,    // Copy cell + surrounding to pixmap, FastRedraw, copy back
  DrawMasking    // Complete redraw of cell in pixmap (cumbersome, but thorough)
} DrawMethod_t;

#endif // _TYPES_H_
