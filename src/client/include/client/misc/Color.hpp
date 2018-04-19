#ifndef CLIENT_MISC_COLOR
#define CLIENT_MISC_COLOR

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <vector>
#include <queue>

#include <ISceneManager.h>
#include <IVideoDriver.h>

typedef struct {
	irr::video::SColor floor1;
	irr::video::SColor floor2;
	irr::video::SColor attack;
	irr::video::SColor light;
} palette;

namespace client {

	extern palette curr_pallette;
	extern const std::vector<palette> palettes;

	void next_palette(int64_t tick);
}

#endif
