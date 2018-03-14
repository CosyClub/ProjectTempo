#include <tempo/component/ComponentStage.hpp>

#include <glm/vec2.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.hpp>

#include <stdint.h>
#include <cstdio>
#include <iostream>

namespace tempo
{

std::vector<stage_tile> _global_stage;
std::string             _global_stage_loaded("None");

void ComponentStage::loadLevel(const char *stage_file)
{
	if (_global_stage_loaded == std::string(stage_file)) {
		tiles = &_global_stage;
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
		       stage_file, pixel_data, width, height, components);
		return;
	}

	// Load the new tiles
	for (int y = 0; y < height; y++) {
		int base = width * y * 4;
		for (int x = 0; x < width; x++) {
			uint8_t *pixel = &pixel_data[base + x * 4];

			if (pixel[0] > 0) {
				int height = (int) (pixel[0] - 127) / 25.6;
				_global_stage.push_back(stage_tile(glm::ivec2(y, x), (float) height));
			}
		}
	}
	_global_stage_loaded = std::string(stage_file);

	stbi_image_free(pixel_data);
}

ComponentStage::ComponentStage(const char *stage_file)
{
	this->stage_file = std::string(stage_file);
	loadLevel(stage_file);
}

inline int ComponentStage::findIndex(glm::ivec2 position)
{
	for (unsigned int i = 0; i < tiles->size(); i++) {
		auto &pos = tiles->at(i).position;
		if (pos == position) {
			return i;
		}
	}

	return -1;
}

std::vector<tempo::stage_tile> ComponentStage::getHeights()
{
	return *tiles;
}

float ComponentStage::getHeight(glm::ivec2 position)
{
	int index = findIndex(position);
	if (index >= 0)
		return tiles->at(index).height;
	else
		return -10.0f;
}

void ComponentStage::setHeight(glm::ivec2 position, int height)
{
	int index = findIndex(position);
	if (index >= 0)
		tiles->at(index).height = height;
}

void ComponentStage::setHeight(std::vector<glm::ivec2> positions, int height)
{
	for (auto &position : positions)
		setHeight(position, height);
}

bool ComponentStage::existstTile(glm::ivec2 position)
{
	int index = findIndex(position);
	if (index >= 0)
		return true;
	else
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
