#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStageTranslation.hpp>
#include <tempo/system/SystemLevelManager.hpp>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>

#include <stb_image.hpp>

#include <stdint.h>
#include <iostream>

namespace tempo
{
// SystemLevelManager
SystemLevelManager::SystemLevelManager(anax::World &world, int size)
    : tile_heights(size, std::vector<float>(size))
{
	world.addSystem(this->grid_positions);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			tile_heights[i][j] = NO_TILE;
		}
	}
}

SystemLevelManager::SystemLevelManager(anax::World &world,
                                       const char * heightMap,
                                       const char * zoneMap)
    : tile_heights(100, std::vector<float>(100))
    , player_spawn_zone(100 * 100)
{
	world.addSystem(this->grid_positions);
	loadLevel(heightMap);
	loadZones(zoneMap);
}

bool SystemLevelManager::existsTile(glm::vec2 position)
{
	return existsTile(position.x, position.y);
}

bool SystemLevelManager::existsTile(unsigned int x, unsigned int y)
{
	if (x < 0 || x >= tile_heights.size() || y < 0 || y >= tile_heights[0].size())
		return false;

	return tile_heights[x][y] != NO_TILE;
}

void SystemLevelManager::deleteTile(glm::vec2 position)
{
	tile_heights[position.x][position.y] = NO_TILE;
}

void SystemLevelManager::createTile(glm::vec2 position)
{
	tile_heights[position.x][position.y] = 0;
}

void SystemLevelManager::setHeight(float height, glm::vec2 position)
{
	if (existsTile(position)) {
		tile_heights[position.x][position.y] = height;
	} else {
		std::cout << "Can't set height to non-existent tile" << std::endl;
	}
}

void SystemLevelManager::setHeight(float height, glm::vec2 position, int width, int length)
{
	for (int i = position.x; i < width + position.x; i++) {
		for (int j = position.y; j < length + position.y; j++) {
			this->tile_heights[i][j] = height;
		}
	}
}

float SystemLevelManager::getHeight(int x, int y)
{
	if (existsTile(x, y)) {
		return tile_heights[x][y];
	} else {
		return NO_TILE;
		std::cout << "Can't get height of non-existent tile" << std::endl;
	}
}

void SystemLevelManager::loadLevel(const char *fileName)
{
	int width, height, components;

	uint8_t *pixel_data = (uint8_t *) stbi_load(fileName, &width, &height, &components, 4);
	if (pixel_data == NULL || width < 0 || height < 0 || components < 0) {
		printf("Failed to load level '%s', pixels: %p, width: %i, height: %i, components: %i\n",
		       fileName, pixel_data, width, height, components);
		return;
	}

	// Clear out any existing tiles
	for (unsigned int x = 0; x < tile_heights.size(); ++x) {
		for (unsigned int y = 0; y < tile_heights[x].size(); ++y) {
			tile_heights[x][y] = NO_TILE;
		}
	}

	// Load the new tiles
	for (int y = 0; y < height; y++) {
		int base = width * y * 4;
		for (int x = 0; x < width; x++) {
			uint8_t *pixel = &pixel_data[base + x * 4];

			if (pixel[0] > 0) {
				int height               = (int) (pixel[0] - 127) / 25.6f;
				this->tile_heights[y][x] = height;
			}
		}
	}

	stbi_image_free(pixel_data);
}

void SystemLevelManager::loadZones(const char *fileName)
{
	int width, height, components;

	uint8_t *pixel_data = (uint8_t *) stbi_load(fileName, &width, &height, &components, 4);
	if (pixel_data == NULL || width < 0 || height < 0 || components != 4) {
		printf(
		  "Failed to load level zones '%s', pixels: %p, width: %i, height: %i, components: %i\n",
		  fileName, pixel_data, width, height, components);
		return;
	}

	for (int y = 0; y < height; y++) {
		int base = width * y * 4;  // 4 since 4 color channels
		for (int x = 0; x < width; x++) {
			uint8_t *p = &pixel_data[base + x * 4];

			if (p[1] > 250) {
				this->player_spawn_zone[spawn_zones] = {x, y};
				spawn_zones++;
			}
		}
	}

	stbi_image_free(pixel_data);
}

glm::vec2 SystemLevelManager::spawn()
{
	srand(time(NULL));
	uint32_t random_location = rand() % spawn_zones;
	return player_spawn_zone[random_location];
}

glm::vec2 SystemLevelManager::getWorldSize()
{
	glm::vec2 result;
	result.x = tile_heights.size();
	result.y = tile_heights[0].size();
	return result;
}

void SystemLevelManager::update(float dt)
{
	auto entities = getEntities();

	for (auto &entity : entities) {
		auto &pos = entity.getComponent<ComponentStagePosition>();
		auto &gm  = entity.getComponent<ComponentStageTranslation>();

		glm::vec2 target_tile = pos.getOccupied()[0] + gm.delta;

		bool can_make_move = true;

		if (!existsTile(target_tile)) {
			std::cout << "Tile does not exist!\n";
			can_make_move = false;
		}

		// :TODO:OPT: this might be quite slow - for each entityV
		// check all others, might want to build quadtree / 2d boolean array
		// -> but we only do this check once per move - so might be okay - PROFILE!
		for (auto &collision_candidate : grid_positions.getEntities()) {
			auto &pos_candidate = collision_candidate.getComponent<ComponentStagePosition>();

			// (NAH) ETHEREAL CHECK

			// Terrible hack sorry lads
			if ((pos.getOccupied()[0] + gm.delta).x == pos_candidate.getOccupied()[0].x
			    && (pos.getOccupied()[0] + gm.delta).y == pos_candidate.getOccupied()[0].y) {
				can_make_move = false;
			}
		}

		// :TODO: If multiple entities try to jump into same tile simultaneously
		// should they both bounce back? Should first one moving get priority?
		if (can_make_move) {
			// Then entity should bounce back to where it came from
			pos.setPosition(pos.getOccupied()[0] + gm.delta);
			// gm.delta.x = 0;
			// gm.delta.y = 0;
			gm.delta = {0, 0};
			// Swap motion progress around so we don't have a obvious jump in motion
		}
	}
}
}
