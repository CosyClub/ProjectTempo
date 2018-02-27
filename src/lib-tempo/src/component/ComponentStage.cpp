#include <tempo/component/ComponentStage.hpp>

#include <glm/vec2.hpp>

#include <SDL.h>
#undef main // SDL defines main, but why do we need to do this?

namespace tempo {

void ComponentStage::loadLevel(const char* stage_file)
{
	SDL_Surface* level = SDL_LoadBMP(stage_file);

	// Load tiles, this can only handles stage_files in positive ZZ
	for (int y = 0; y < level->h; y++) {
		for (int x = 0; x < level->w; x++) {
			int bpp = level->format->BytesPerPixel;
			// Here p is the address to the pixel we want to retrieve
			Uint8 *p = (Uint8 *)level->pixels + y
			                  * level->pitch + x * bpp;
			uint32_t pixel = 0;
			pixel = *p;

			if (pixel > 0) {
				int height = (int)(pixel - 127) / 25.6;
				tiles.push_back(std::make_tuple(
							glm::ivec2(x, y),
							height));
			}
		}
	}
}

ComponentStage::ComponentStage(const char* stage_file)
{
	this->stage_file = std::string(stage_file);
	loadLevel(stage_file);
}

stage_tiles ComponentStage::getHeights()
{
	return tiles;
}

float ComponentStage::getHeight(glm::ivec2 position)
{
	for (unsigned int i = 0; i < tiles.size(); i++) {
		std::tuple<glm::ivec2, float> tile = tiles[i];
		glm::ivec2 pos = std::get<0>(tile);
		if (pos == position) {
			return std::get<1>(tile);
		}
	}

	return -10.0f;
}

bool ComponentStage::existstTile(glm::ivec2 position)
{
	for (unsigned int i = 0; i < tiles.size(); i++) {
		std::tuple<glm::ivec2, float> tile = tiles[i];
		glm::ivec2 pos = std::get<0>(tile);
		if (pos == position) {
			return true;
		}
	}

	return false;
}

/////
// Required for networking
/////
ComponentStage::ComponentStage(sf::Packet p)
{
	p >> stage_file;
	loadLevel(stage_file.c_str());
}

ComponentID ComponentStage::getId()
{
	return ComponentID::STAGE;
}

sf::Packet ComponentStage::dumpComponent()
{
	sf::Packet p;
	p << stage_file;
	return p;
}

} // namespace tempo
