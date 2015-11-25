#ifndef _COLOR_H_
#define _COLOR_H_

#include <string>
#include <vector>
#include <map>

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

    bool operator==(const RGB &x) const
    {
      if( _rgb[0]!=x._rgb[0] ) return false;
      if( _rgb[1]!=x._rgb[1] ) return false;
      if( _rgb[2]!=x._rgb[2] ) return false;
      return true;
    }

    bool operator !=(const RGB &x) const { return ! (*this==x); }

    int red(void)   const;
    int green(void) const;
    int blue(void)  const;
    int gray(void)  const;

  private:
    bool _ok;
    int _rgb[3];
};

typedef std::vector<RGB> Palette_t;

namespace Color
{
  void add(std::string name, int r, int g, int b, std::string inverse="");
  void add(std::string name, const RGB &rgb, std::string inverse="");

  bool       exists(std::string name);
  const RGB &get(std::string name);

  bool       hasInverseFor(std::string name);
  const RGB &getInverseFor(std::string name);
  void       setInverseFor(std::string name, std::string inverse_name);

  void    fillPalette(Palette_t &palette, int levels, RGB min, RGB max );

  void    validateInverseColors(void);

  typedef std::map<std::string,RGB>          Map_t;
  typedef Map_t::iterator                    MapIter_t;
  typedef std::pair<std::string,RGB>         MapValue_t;

  typedef std::map<std::string,std::string>  Inverse_t;
  typedef Inverse_t::iterator                InverseIter_t;
  typedef std::pair<std::string,std::string> InverseValue_t;
};

#endif // _COLOR_H_
