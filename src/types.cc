#include "types.h"

#include <sstream>
#include <exception>
#include <math.h>

#include <X11/Xlib.h>

using namespace std;

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
  if(!_ok) throw logic_error("call to blue() on uninitialized RGB object");

  double t = _rgb[0]*_rgb[0] + _rgb[1]*_rgb[1] + _rgb[2]*_rgb[2];
  return int( sqrt(t) + 0.5 );
}
