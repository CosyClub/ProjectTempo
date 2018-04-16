#ifndef CLIENT_MISC_COLOR
#define CLIENT_MISC_COLOR

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <vector>
#include <queue>

#include <ISceneManager.h>
#include <IVideoDriver.h>

typedef struct{
	irr::video::SColor floor;
	irr::video::SColor floor2;
	irr::video::SColor light;
} palette;

namespace client {
	palette curr_pallette;
	std::queue<palette> palettes;

	void init_palettes();
}

#endif
