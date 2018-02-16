#include <tempo/component/ComponentStage.hpp>

#include <glm/vec2.hpp>

#include <SDL.h>
#undef main // SDL defines main, but why do we need to do this?

namespace tempo{
	ComponentStage::ComponentStage(const char* stage_file) {
		SDL_Surface* level = SDL_LoadBMP(stage_file);

		// load tiles, this can only handles stage_files in positive ZZ
		for (int y = 0; y < level->h; y++) {
			for (int x = 0; x < level->w; x++) {
				int bpp = level->format->BytesPerPixel;
				// here p is the address to the pixel we want to retrieve
				Uint8 *p = (Uint8 *)level->pixels + y * level->pitch + x * bpp;
				uint32_t pixel = 0;
				pixel = *p;

				if (pixel > 0) {
					int height = (int)(pixel - 127) / 25.6;
					tiles.push_back(std::make_tuple(glm::ivec2(x, y), height));
				}
			}
		}
	}

	std::vector<std::tuple<glm::ivec2, float>> ComponentStage::getHeights() {
		return tiles;
	}
}
