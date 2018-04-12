#include <client/misc/RGBtoHSV.hpp>
#include <algorithm>

/*
Copyright (c) 2014 Adam 'entity' Krupicka
Released under the zlib license (http://zlib.net/zlib_license.html).
*/

/** Converts an RGB color to it's HSV representation
*  @param rgb RGB color to convert
*  @return The HSV encoded color, as an irr::core::vector3df where X is Hue [0, 360); Y is Saturation [0, 1]; Z is Value [0, 1].
*/

namespace client {
	irr::core::vector3df RGBftoHSV(irr::video::SColorf rgb)
	{
     irr::core::vector3df hsv;

     irr::f32 M = std::max(std::max(rgb.getRed(), rgb.getGreen()), std::max(rgb.getGreen(), rgb.getBlue()));
     irr::f32 m = std::min(std::min(rgb.getRed(), rgb.getGreen()), std::min(rgb.getGreen(), rgb.getBlue()));
     irr::f32 C = M - m;

     if (C == 0)
         hsv.X = 0;
     else if (M <= rgb.getRed())
         hsv.X = ((rgb.getGreen() - rgb.getBlue()) / C);
     else if (M <= rgb.getGreen())
         hsv.X = ((rgb.getBlue() - rgb.getRed()) / C) + 2;
     else if (M <= rgb.getBlue())
         hsv.X = ((rgb.getRed() - rgb.getGreen()) / C) + 4;

     hsv.X *= 60;
     if (hsv.X < 0)
         hsv.X += 360;

     hsv.Z = M;

     if (hsv.Z == 0)
         hsv.Y = 0;
     else
         hsv.Y = C / hsv.Z;

     return hsv;
  }

  irr::core::vector3df RGBtoHSV(irr::video::SColor rgb)
  {
     return RGBftoHSV(irr::video::SColorf(rgb));
  }

  irr::video::SColorf HSVtoRGBf(irr::core::vector3df hsv)
  {
	  // Taken from:
	  // https://www.splinter.com.au/converting-hsv-to-rgb-colour-using-c/
	  irr::f32 h = hsv.X;
	  irr::f32 s = hsv.Y;
	  irr::f32 v = hsv.Z;

	  h /= 360.0;

	  int i      = h * 6;
	  irr::f32 f = h * 6 - i;
	  irr::f32 p = v * (1 - s      );
	  irr::f32 q = v * (1 - f*s    );
	  irr::f32 t = v * (1 - (1-f)*s);

	  // -1 and 6 cases are included in case float rounding errors
	  // push us just over the limit, hue and hence i wraps around
	  // as its measured in degrees
	  switch(i){
	  case  6 :
	  case  0 : return irr::video::SColorf(v,t,p);
	  case  1 : return irr::video::SColorf(q,v,p);
	  case  2 : return irr::video::SColorf(p,v,t);
	  case  3 : return irr::video::SColorf(p,q,v);
	  case  4 : return irr::video::SColorf(t,p,v);
	  case -1 :
	  case  5 : return irr::video::SColorf(v,p,q);
	  }
	  return irr::video::SColorf(0,0,0);
  }

  irr::video::SColor HSVtoRGB(irr::core::vector3df hsv)
  {
     return HSVtoRGBf(hsv).toSColor();
  }

  irr::video::SColorf hueShift(irr::video::SColorf col, irr::f32 shift)
  {
     irr::f32 a = col.a;

     irr::core::vector3df hsvCol = RGBftoHSV(col);
     hsvCol.X += shift;
     while (hsvCol.X >= 360)
         hsvCol.X -= 360;
     while (hsvCol.X < 0)
         hsvCol.X += 360;

     irr::video::SColorf comp = HSVtoRGBf(hsvCol);
     comp.a = a;
     return comp;
  }

  irr::video::SColor hueShift(irr::video::SColor col, irr::f32 shift)
  {
     return hueShift(irr::video::SColorf(col), shift).toSColor();
  }
}
