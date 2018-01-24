////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file LevelManager.cpp
/// \author Jamie Terry
/// \date 2017/11/15
/// \brief Contains definition of LevelManager functions
////////////////////////////////////////////////////////////////////////////

#include <anax/World.hpp>
#include <tempo/entity/LevelManager.hpp>
#include <iostream>

#include <SDL.h>
#undef main // SDL likes to define main

namespace tempo{
	/////////////////////////////////////////////////////////////
	// ComponentGridPosition
	ComponentGridPosition::ComponentGridPosition(Vec2s pos, TileMask mask, bool is_ethereal) :
		ComponentGridPosition(pos.x, pos.y, mask, is_ethereal){
		// empty body
	}

	ComponentGridPosition::ComponentGridPosition(int x, int y, TileMask mask, bool is_ethereal){
		// level not currently used -> but when we want to do more efficient lookup
		// structure we need to insert entity in it based off of its position
		this->position = {x,y};
		this->mask     = mask;
		this->ethereal = is_ethereal;
	}

	TileMask ComponentGridPosition::isCollidingWith(const ComponentGridPosition& other){
		return this->mask.isCollidingWith(other.mask, other.position - this->position);
	}

	/////////////////////////////////////////////////////////////
	// ComponentGridMotion
	ComponentGridMotion::ComponentGridMotion(float max_jump_distance,
	                                         float movement_speed,
	                                         float max_jump_height){
		this->delta             = {0,0};
		this->motion_progress   = 0;
		this->max_jump_height   = max_jump_height;
		this->max_jump_distance = max_jump_distance;
		this->movement_speed    = movement_speed;
		this->target_locked     = false;
	}

	bool ComponentGridMotion::beginMovement(int dx, int dy){
		if(this->motion_progress != 0){ return false; }

		if((dx*dx + dy*dy) > (this->max_jump_distance * this->max_jump_distance)){
			return false;
		}

		this->delta.x = dx;
		this->delta.y = dy;
		return true;
	}

	const Vec2s& ComponentGridMotion::getCurrentMovement(){
		return this->delta;
	}

	float ComponentGridMotion::getMotionProgress(){
		return this->motion_progress;
	}

	bool ComponentGridMotion::isMoving(){
		return this->motion_progress >= 0.0f;
	}

	bool ComponentGridMotion::isMovementLocked(){
		return this->target_locked;
	}

	/////////////////////////////////////////////////////////////
	// SystemLevelManager
	SystemLevelManager::SystemLevelManager(anax::World& world, int size)
		: tile_heights(size, std::vector<float>(size)) {
		world.addSystem(this->grid_positions);
		for(int i = 0; i < size; i++){
			for(int j = 0; j< size; j++){
				tile_heights[i][j] = NO_TILE;
			}
		}
	}

	SystemLevelManager::SystemLevelManager(anax::World& world,
	                                       const char* heightMap,
	                                       const char* zoneMap)
		: tile_heights(100, std::vector<float>(100)),
		  player_spawn_zone(100*100) {
		world.addSystem(this->grid_positions);
		loadLevel(heightMap);
		loadZones(zoneMap);
	}

	bool SystemLevelManager::existsTile(Vec2s position) {
		return existsTile(position.x, position.y);
 	}

	bool SystemLevelManager::existsTile(int x, int y) {
		if( x < 0  || x >= tile_heights.size() ||
	    	y < 0  || y >= tile_heights.size() )
			return false;

		return tile_heights[x][y] != NO_TILE;
 	}

	void SystemLevelManager::deleteTile(Vec2s position) {
		tile_heights[position.x][position.y] = NO_TILE;
	}

	void SystemLevelManager::createTile(Vec2s position) {
		tile_heights[position.x][position.y] = 0;
	}

	void SystemLevelManager::setHeight(float height, Vec2s position) {
		if (existsTile(position)) {
			tile_heights[position.x][position.y] = height;
		} else {
			std::cout <<" Can't set height to non-existent tile";
		}
	}

	void SystemLevelManager::setHeight(float height, Vec2s position, int width, int length) {
		for(int i = position.x; i < width+position.x; i++){
			for(int j = position.y; j < length+position.y; j++){
				this->tile_heights[i][j] = height;
			}
		}
	}

	float SystemLevelManager::getHeight(int x, int y) {
		if (existsTile(x, y)) {
			return tile_heights[x][y];
		} else {
			std::cout <<" Can't get height of non-existent tile";
		}
	}

	void SystemLevelManager::loadLevel(const char* fileName) {

		SDL_Surface* level = SDL_LoadBMP(fileName);

		// Clear out any existing tiles
		for(int x = 0; x < tile_heights.size(); ++x){
			for(int y = 0; y < tile_heights[x].size(); ++y){
				tile_heights[x][y] = NO_TILE;
			}
		}

		// Load the new tiles
		for (int y = 0; y < level->h; y++) {
			for (int x = 0; x < level->w; x++) {

				int bpp = level->format->BytesPerPixel;
				/* Here p is the address to the pixel we want to retrieve */
				Uint8 *p = (Uint8 *)level->pixels + y * level->pitch + x * bpp;
				uint32_t pixel = 0;

				pixel = *p;

				if (pixel > 0) {
					int height = (int) (pixel - 127) / 25.6;
					this->tile_heights[x][y] = height;
				}
			}
		}
	}

	void SystemLevelManager::loadZones(const char* fileName) {

		SDL_Surface* level = SDL_LoadBMP(fileName);

		for (int y = 0; y < level->h; y++) {
			for (int x = 0; x < level->w; x++) {

				int bpp = level->format->BytesPerPixel;
				/* Here p is the address to the pixel we want to retrieve */
				Uint8 *p = (Uint8 *)level->pixels + y * level->pitch + x * bpp;
				uint32_t pixel = 0;

				pixel = p[0] | p[1] << 8 | p[2] << 16;

				//std::cout << (int) p[0]<<" "<<(int) p[1]<<" "<< (int) p[2]<<std::endl;

				if (p[1] > 250) {
					this->player_spawn_zone[spawn_zones] = {x,y};
					spawn_zones++;
				}
			}
		}

	}

	Vec2s SystemLevelManager::spawn() {
		srand (time(NULL));
		uint32_t random_location = rand() % spawn_zones;
		return player_spawn_zone[random_location];
	}

	Vec2s SystemLevelManager::getWorldSize(){
		Vec2s result;
		result.x = tile_heights.size();
		result.y = tile_heights[0].size();
		return result;
	}

	void SystemLevelManager::update(float dt){
		auto entities = getEntities();

		for(auto& entity : entities){
			auto& pos   = entity.getComponent<ComponentGridPosition>();
			auto& gm    = entity.getComponent<ComponentGridMotion>();

			if(gm.delta != Vec2s::Origin){
				gm.motion_progress += (dt * gm.movement_speed) / (float)tempo::length(gm.delta);
				if(gm.motion_progress >= 1){
					pos.position       += gm.delta;
					gm.delta           =  {0,0};
					gm.motion_progress =  0;
					gm.target_locked   =  false;
				}
			}

			Vec2s target_tile = pos.position + gm.delta;

			float current_height = this->getHeight(pos.position);
			float target_height  = this->getHeight(target_tile);
			float delta_height   = target_height - current_height;

			/////////////////////////////////
			// Work out if entity is allowed to go to its current target
			// This depends on:
			// - Delta height between tiles
			// - If target tile is already occupied by another entity
			if(gm.motion_progress >= 0.5f && !gm.isMovementLocked()){
				bool can_make_move = true;

				// Check height difference
				if(abs(delta_height) > gm.max_jump_height){
					can_make_move = false;
				}

				// :TODO:OPT: this might be quite slow - for each entity
				// check all others, might want to build quadtree / 2d boolean array
				// -> but we only do this check once per move - so might be okay - PROFILE!
				for(auto& collision_candidate : grid_positions.getEntities()){
					auto& pos_candidate = collision_candidate.getComponent<ComponentGridPosition>();

					if(pos_candidate.ethereal && pos.ethereal){ continue; }

					Vec2s candidate_delta = pos_candidate.position - target_tile;
					//printf("Found candidate: %i, %i\n", candidate_delta.x, candidate_delta.y);

					if(pos.mask.isCollidingWith(pos_candidate.mask, candidate_delta)){
						can_make_move = false;
					}
				}

				// :TODO: If multiple entities try to jump into same tile simultaneously
				// should they both bounce back? Should first one moving get priority?

				if(!can_make_move){
					// Then entity should bounce back to where it came from
					pos.position += gm.delta;
					gm.delta     *= -1;
					// Swap motion progress around so we don't have a obvious jump in motion
					gm.motion_progress = 1 - gm.motion_progress;
					gm.target_locked = true;
				} else {
					// :TODO: claim the target tile with level manager
					gm.target_locked = true;
				}
			}
		}
	}
}
