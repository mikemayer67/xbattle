#include "color.h"

#include <sstream>
#include <exception>
#include <math.h>

#include <X11/Xlib.h>

using namespace std;

// 
// RGB Class
//

RGB::RGB(int r, int g, int b)
{
  if(r<0 || r>255)
  {
    stringstream err;
    err << "red component (" << r << ") is not in the range 0-255";
    throw runtime_error(err.str());
  }
  if(g<0 || g>255)
  {
    stringstream err;
    err << "green component (" << g << ") is not in the range 0-255";
    throw runtime_error(err.str());
  }
  if(b<0 || b>255)
  {
    stringstream err;
    err << "blue component (" << b << ") is not in the range 0-255";
    throw runtime_error(err.str());
  }

  _rgb[0] = r;
  _rgb[1] = g;
  _rgb[2] = b;
  _ok     = true;
}

RGB::RGB(const RGB &a, const RGB &b, double frac)
{
  if(!a._ok) throw logic_error("attempt to interpolate an uninitialized RGB object");
  if(!b._ok) throw logic_error("attempt to interpolate an uninitialized RGB object");
  
  for(int i=0; i<3; ++i)
  {
    _rgb[i] = (1.-frac)*a._rgb[i] + frac*b._rgb[i] + 0.5;
  }
  _ok = true;
}

RGB::RGB(string name)
{
  Display  *display = XOpenDisplay("");
  int       screen  = DefaultScreen(display);
  Colormap  cmap    = DefaultColormap(display,screen);

  XColor color;
  if(!XParseColor(display,cmap,name.c_str(),&color))
    throw runtime_error(string("Unable to parse color name: ")+name);

  _rgb[0] = color.red   >> 8;
  _rgb[1] = color.green >> 8;
  _rgb[2] = color.blue  >> 8;
  _ok     = true;
}

const RGB &RGB::operator=(const RGB &x)
{
  _ok=x._ok; 
  if(_ok) memcpy(_rgb,x._rgb,3*sizeof(int));
  return *this; 
}

int RGB::red(void) const
{
  if(!_ok) throw logic_error("call to red() on uninitialized RGB object");
  return _rgb[0];
}

int RGB::green(void) const
{
  if(!_ok) throw logic_error("call to green() on uninitialized RGB object");
  return _rgb[1];
}

int RGB::blue(void) const
{
  if(!_ok) throw logic_error("call to blue() on uninitialized RGB object");
  return _rgb[2];
}

int RGB::gray(void) const
{
  if(!_ok) throw logic_error("call to gray() on uninitialized RGB object");

  double t = _rgb[0]*_rgb[0] + _rgb[1]*_rgb[1] + _rgb[2]*_rgb[2];
  return int( sqrt(t) + 0.5 );
}

//
// Color namespace
//

Color::Map_t     gColors;
Color::Inverse_t gInverse;

void Color::add(string name,int r,int g,int b, string inverse)
{
  Color::add(name, RGB(r,g,b), inverse);
}

void Color::add(string name, const RGB &rgb, string inverse)
{
  Color::Map_t::iterator x=gColors.find(name);
  if( x == gColors.end() )
  {
    Color::MapValue_t value(name,rgb);

    pair<Color::MapIter_t, bool> rc = gColors.insert(value);

    if(! rc.second )
    {
      stringstream err;
      err << "failed to insert new color '" << name << "' into the color map";
      throw runtime_error(err.str());
    }
  }
  else
  {
    gColors[name] = rgb;
  }

  if( ! inverse.empty() ) Color::setInverseFor(name,inverse);
}

void Color::setInverseFor(std::string name,std::string inv)
{
  if( ! Color::exists(inv) )
  {
    stringstream err;
    err << "cannot use undefined color (" << inv << ") as inverse for " << name;
    throw runtime_error(err.str());
  }

  Color::Inverse_t::iterator x=gInverse.find(name);

  if( x==gInverse.end() )
  {
    Color::InverseValue_t value(name,inv);
  
    pair<Color::InverseIter_t, bool> rc = gInverse.insert(value);
    if(! rc.second )
    {
      stringstream err;
      err << "failed to insert inverse for color '" << name << "' into the inverse color map";
      throw runtime_error(err.str());
    }
  }
  else
  {
    gInverse[name] = inv;
  }

}

bool Color::exists(std::string name)
{
  try
  {
    if( gColors.find(name)==gColors.end() )
    {
      RGB rgb(name);
      string inverse = (rgb.gray() >= 128 ? "black" : "white");
      add(name,rgb,inverse);
    }
  }
  catch(...)
  {
    return false;
  }
  return true;
}

const RGB &Color::get(std::string name)
{
  Color::MapIter_t rval = gColors.find(name);
  if(rval==gColors.end())
  {
    stringstream err;
    err << "attemtped to access the uninitialized color '" << name << "'";
    throw logic_error(err.str());
  }

  return rval->second;
}

bool Color::hasInverseFor(string name)
{
  return( gInverse.find(name)!=gInverse.end() );
}

const RGB &Color::getInverseFor(std::string name)
{
  Color::InverseIter_t inv = gInverse.find(name);
  if(inv==gInverse.end())
  {
    stringstream err;
    err << "attemtped to access the undefined inverse for color '" << name << "'";
    throw logic_error(err.str());
  }

  return Color::get(inv->second);
}

void Color::fillPalette(Palette_t &palette, int levels, RGB min, RGB max )
{
  for(int i=palette.size(); i<levels; ++i) palette.push_back(RGB());

  if( ! palette.at(0)        ) palette.at(0)        = min;
  if( ! palette.at(levels-1) ) palette.at(levels-1) = max;

  for(int i=1; i<levels-1; ++i)
  {
    if( !palette.at(i) )
    {
      int j=i+1;
      while(!palette.at(j)) ++j;

      min = palette.at(i-1);
      max = palette.at(j);

      for(int k=i; k<j; ++k)
      {
        double frac = (1.+k-i)/(1.+j-i);
        palette.at(k) = RGB( min, max, frac );
      }
      i=j;
    }
  }
}
