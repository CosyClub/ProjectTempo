#include <client/misc/RGBtoHSV.hpp>

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

     hsv.Y = M;

     if (hsv.Y == 0)
         hsv.Z = 0;
     else
         hsv.Z = C / hsv.Y;

     return hsv;
  }

  irr::core::vector3df RGBtoHSV(irr::video::SColor rgb)
  {
     return RGBftoHSV(irr::video::SColorf(rgb));
  }

  irr::video::SColorf HSVtoRGBf(irr::core::vector3df hsv)
  {
     irr::video::SColorf rgb;

     irr::f32 C = hsv.Z * hsv.Y;
     irr::f32 H = hsv.X / 60;
     irr::f32 Hmod = H - (2 * (int(H) / 2)); // same as H = fmod(H, 2)
     irr::f32 X = C * (1 - std::abs(Hmod - 1));

     if (H < 1)
         rgb = irr::video::SColorf(C, X, 0);
     else if (H < 2)
         rgb = irr::video::SColorf(X, C, 0);
     else if (H < 3)
         rgb = irr::video::SColorf(0, C, X);
     else if (H < 4)
         rgb = irr::video::SColorf(0, X, C);
     else if (H < 5)
         rgb = irr::video::SColorf(X, 0, C);
     else if (H < 6)
         rgb = irr::video::SColorf(C, 0, X);

     irr::f32 m = hsv.Z - C;
     rgb.r += m;
     rgb.g += m;
     rgb.b += m;

     return rgb;
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
