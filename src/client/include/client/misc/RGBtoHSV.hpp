#ifndef CLIENT_MISC_RGBtoHSV
#define CLIENT_MISC_RGBtoHSV

#include <vector3d.h>
#include <SColor.h>

namespace client{
	irr::core::vector3df RGBftoHSV(irr::video::SColorf rgb);

  irr::core::vector3df RGBtoHSV(irr::video::SColor rgb);

  irr::video::SColorf HSVtoRGBf(irr::core::vector3df hsv);

  irr::video::SColor HSVtoRGB(irr::core::vector3df hsv);

  irr::video::SColorf hueShift(irr::video::SColorf col, irr::f32 shift);

  irr::video::SColor hueShift(irr::video::SColor col, irr::f32 shift);
}

#endif
