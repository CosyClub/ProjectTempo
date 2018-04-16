#ifndef CLIENT_MISC_COLOR
#define CLIENT_MISC_COLOR

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <vector>

typedef glm::vec3 palette[3];

// Contains functions for all lighting elements that do not require updating after initialisation

namespace client {
	palette curr_pallette;
	std::queue<palette> palettes;
}

#endif
