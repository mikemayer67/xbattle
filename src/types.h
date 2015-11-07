#ifndef _TYPES_H_
#define _TYPES_H_

#include <vector>
#include <set>
#include <string>

typedef unsigned long ulong;

typedef std::vector<std::string> StringList_t;
typedef std::set<std::string>    StringSet_t;

class RGB
{
  public:
    RGB(void) : _ok(false) {}
    RGB(int r,int g,int b);
    RGB(const RGB &x) { *this=x; }
    RGB(std::string);
    RGB(const RGB &a, const RGB &b, double frac);

    const RGB &operator=(const RGB &x);

    operator bool(void) const { return _ok; }

    int red(void)   const;
    int green(void) const;
    int blue(void)  const;
    int gray(void)  const;

  private:
    bool _ok;
    int _rgb[3];
};

typedef enum { Square, Hex, Octagon, Diamond, Triangle } TileShape_t;
typedef enum { Flat, Hills, Forests }                    TerrainType_t;

#endif // _TYPES_H_
