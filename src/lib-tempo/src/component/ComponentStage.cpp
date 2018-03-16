#include <tempo/component/ComponentStage.hpp>

#include <glm/gtx/string_cast.hpp>
#include <glm/vec2.hpp>

#include <anax/World.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.hpp>

#include <stdint.h>
#include <cstdio>
#include <iostream>
#include <map>

#include <tempo/entity/EntityCreationServer.hpp>

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
	tiles                = &_global_stage;
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
		auto &pos = (*tiles)[i].position;
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
		return (*tiles)[index].height;
	else
		return -10.0f;
}

void ComponentStage::setHeight(glm::ivec2 position, int height)
{
	int index = findIndex(position);
	if (index >= 0)
		(*tiles)[index].height = height;
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


void fake_createButtonGroup(anax::World &           world,
                            std::vector<glm::ivec2> positions,
                            std::vector<glm::ivec2> tiles)
{
	std::cout << "positions = {";
	for (int i = 0; i < positions.size() - 1; i++) {
		std::cout << glm::to_string(positions[i]) << ",";
	}
	std::cout << glm::to_string(positions.back()) << "}" << std::endl;

	std::cout << "tiles = {";
	for (int i = 0; i < tiles.size() - 1; i++) {
		std::cout << glm::to_string(tiles[i]) << ",";
	}
	std::cout << glm::to_string(tiles.back()) << "}" << std::endl;
}

void loadButtons(anax::World &world, const char *tile_map)
{

	int width, height, components;

	uint8_t *pixel_data = (uint8_t *) stbi_load(tile_map, &width, &height, &components, 4);
	if (pixel_data == NULL || width < 0 || height < 0 || components < 0) {
		printf("Failed to load level '%s', pixels: %p, width: %i, height: %i, components: %i\n",
		       tile_map, pixel_data, width, height, components);
		return;
	}

	std::vector<std::vector<glm::ivec2>> buttons(256);
	std::vector<std::vector<glm::ivec2>> walls(256);

	// Load the new tiles
	for (int y = 0; y < height; y++) {
		int base = width * y * 4;
		for (int x = 0; x < width; x++) {
			uint8_t *pixel = &pixel_data[base + x * 4];

			uint8_t r = pixel[0];
			uint8_t g = pixel[1];
			uint8_t b = pixel[2];

			if (b == 0 && r != 0) {
				// this is a button
				// r = index
				// g = level
				buttons[r].push_back(glm::ivec2(y, x));
			} else if (r == 0 && g != 0) {
				// this is a wall
				// g = index
				// b = height
				walls[g].push_back(glm::ivec2(y, x));
			}
		}
	}
	stbi_image_free(pixel_data);

	for (int i = 0; i < 256; i++) {
		if (buttons[i].empty() || walls[i].empty()) {
			continue;
		}

		fake_createButtonGroup(world, buttons[i], walls[i]);
		createButtonGroup(world, buttons[i], walls[i]);
	}
}

}  // namespace tempo
