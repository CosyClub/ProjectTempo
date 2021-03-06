#include <tempo/component/ComponentStage.hpp>

#include <glm/vec2.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.hpp>

#include <stdint.h>
#include <cstdio>
#include <iostream>

namespace tempo
{

tileMap _global_stage;
heightDeltaMap _global_heightDelta;
std::string             _global_stage_loaded("None");

void ComponentStage::loadLevel(const char *stage_file)
{

	//Needed so that the player cannot see other feeder areas
	int emptySpace = 40;

	int fwidth = 150;
	int fheight = 40 + emptySpace;
	int feeder_areas = 5;

	if (_global_stage_loaded == std::string(stage_file)) {
		tiles = &_global_stage;
		heightDelta = & _global_heightDelta;
		return;
	}

	if (_global_stage_loaded != "None") {
		std::cout << "More than one stage file loaded, check ComponentStage.cpp" << std::endl;
		std::cout << "Already loaded: " << _global_stage_loaded << std::endl;
		std::cout << "Called to load: " << stage_file << std::endl;

		assert(false);
	}

	int width, height, components;

	uint8_t *pixel_data = (uint8_t *) stbi_load(stage_file, &width, &height, &components, 4);
	if (pixel_data == NULL || width < 0 || height < 0 || components < 0) {
		printf("Failed to load level '%s', pixels: %p, width: %i, height: %i, components: %i\n",
		       stage_file, (void *)pixel_data, width, height, components);
		return;
	}

	_global_stage.reserve(height*width);

	// Load the new tiles
	for (int y = 0; y < height; y++) {
		if(y > fheight * feeder_areas){
			break;
		}
		int base = width * y * 4;
		for (int x = 0; x < width; x++) {

			uint8_t *pixel = &pixel_data[base + x * 4];

			if (pixel[0] > 0) {
				int height = (pixel[0] - 127);
					_global_stage.emplace(glm::ivec2(y, x),
					                      (stage_tile(glm::ivec2(y, x), (float) height)));
			}

			// Repeat tiles for feeder areas
			else if (x < fwidth){
				int modBase = width * (y%fheight) * 4;
				uint8_t *modPixel = &pixel_data[modBase + x * 4];

				if (modPixel[0] > 0) {
					int modHeight =  (modPixel[0] - 127);
					_global_stage.emplace(glm::ivec2(y, x),
					                      (stage_tile(glm::ivec2(y, x), (float) modHeight)));
				}
			}
		}
	}
	tiles = &_global_stage;
	heightDelta = & _global_heightDelta;
	_global_stage_loaded = std::string(stage_file);

	stbi_image_free(pixel_data);
}

ComponentStage::ComponentStage(const char *stage_file)
{
	this->stage_file = std::string(stage_file);
	loadLevel(stage_file);
}

std::vector<stage_tile> ComponentStage::getHeights()
{
	std::vector<stage_tile> vec;
	for (auto a : (*tiles))
	{
		vec.push_back(a.second);
	}
	return vec;
}

float ComponentStage::getHeight(glm::ivec2 position)
{
	auto itr = (*tiles).find(position);
	if (itr == (*tiles).end()) return -10.f;
	return itr->second.height;
}

void ComponentStage::setHeight(glm::ivec2 position, int height)
{
	if ((*tiles).find(position) != (*tiles).end()) (*tiles)[position].height = height;
	(*heightDelta).emplace(position, true);
}

void ComponentStage::setHeight(std::vector<glm::ivec2> positions, int height)
{
	for (auto &position : positions)
	{
		setHeight(position, height);
	}
}

bool ComponentStage::existstTile(glm::ivec2 position)
{
	return (*tiles).find(position) != (*tiles).end();
}

bool ComponentStage::isNavigable(glm::ivec2 position, float height)
{
	auto itr = (*tiles).find(position);
	if (itr != (*tiles).end())
	{
		return abs(itr->second.height - height) < 5;
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

}  // namespace tempo
