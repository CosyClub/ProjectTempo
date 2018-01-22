////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file LevelManager.cpp
/// \author Jamie Terry
/// \date 2017/11/15
/// \brief Contains definition of LevelManager functions
////////////////////////////////////////////////////////////////////////////

#include <OgreMath.h>

#include <anax/World.hpp>
#include <tempo/entity/LevelManager.hpp>
#include <iostream>

#include <SDL.h>
#undef main // SDL likes to define main

namespace tempo{
	/////////////////////////////////////////////////////////////
	// ComponentGridPosition
	ComponentGridPosition::ComponentGridPosition(SystemLevelManager& level, Vec2s pos, TileMask mask, bool is_ethereal) :
		ComponentGridPosition(level, pos.x, pos.y, mask, is_ethereal){
		// empty body
	}

	ComponentGridPosition::ComponentGridPosition(SystemLevelManager& level, int x, int y, TileMask mask, bool is_ethereal){
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

	bool ComponentGridMotion::isMoving(){
		return this->motion_progress >= 0.0f;
	}

	bool ComponentGridMotion::isMovementLocked(){
		return this->target_locked;
	}

	/////////////////////////////////////////////////////////////
	// SystemLevelManager
	SystemLevelManager::SystemLevelManager(anax::World& world, Ogre::SceneManager* scene, int size)
		: tiles(size, std::vector<Tile*>(size)) {
		floor_node = scene->getRootSceneNode()->createChildSceneNode();

		for(int i = 0; i < size; i++){
			for(int j = 0; j< size; j++){
				tiles[i][j] = new Tile(scene, floor_node, {i,j}, 0);
			}
		}
	}

	SystemLevelManager::SystemLevelManager(anax::World& world, int size) : tiles(size, std::vector<Tile*>(size)) {

		for(int i = 0; i < size; i++){
			for(int j = 0; j< size; j++){
				tiles[i][j] = new Tile(0);
			}
		}
	}

	SystemLevelManager::SystemLevelManager(anax::World& world, Ogre::SceneManager* scene, const char* fileName) : tiles(100, std::vector<Tile*>(100)) {

		world.addSystem(this->grid_positions);

		loadLevel(scene, fileName, tiles);
	}

	Ogre::SceneNode* SystemLevelManager::getFloorNode(){
		return floor_node;
	}

	bool SystemLevelManager::existsTile(Vec2s position) {
		return existsTile(position.x, position.y);
 	}

	bool SystemLevelManager::existsTile(int x, int y) {
		if( x < 0  || x >= tiles.size() ||
	    	y < 0  || y >= tiles.size() )
			return false;

		if( tiles[x][y])
			return true;
		else
			return false;
 	}

	void SystemLevelManager::deleteTile(Ogre::SceneManager* scene, Vec2s position) {
		if (existsTile(position)) {
			tiles[position.x][position.y]->deleteFloorpiece(scene);
		} else {
			std::cout <<" Can't delete non-existent tile";
		}
	}

	void SystemLevelManager::createTile(Ogre::SceneManager* scene, Vec2s position) {
		tiles[position.x][position.y]->createFloorpiece(scene);
	}

	void SystemLevelManager::setMaterial(std::string material_name, Vec2s position) {
		if (existsTile(position)) {
			tiles[position.x][position.y]->setMaterial(material_name);
		} else {
			std::cout <<" Can't set material to non-existent tile";
		}
	}

	void SystemLevelManager::setHeight(float height, Vec2s position) {
		if (existsTile(position)) {
			tiles[position.x][position.y]->setHeight(height);
		} else {
			std::cout <<" Can't set height to non-existent tile";
		}
	}

	void SystemLevelManager::setHeight(float height, Vec2s position, int width, int length) {
		for(int i = position.x; i < width+position.x; i++){
			for(int j = position.y; j < length+position.y; j++){
				this->tiles[i][j]->setHeight(height);
			}
		}
	}

	float SystemLevelManager::getHeight(int x, int y) {
		if (existsTile(x, y)) {
			return tiles[x][y]->getHeight();
		} else {
			std::cout <<" Can't get height of non-existent tile";
		}
	}

	void SystemLevelManager::loadLevel(Ogre::SceneManager* scene, const char* fileName, std::vector<std::vector<Tile*>> tiles) {

		SDL_Surface* level = SDL_LoadBMP(fileName);

		floor_node = scene->getRootSceneNode()->createChildSceneNode();

		float max_height = 0;
		float min_height = 100000;

		for (int y = 0; y < level->h; y++) {
			for (int x = 0; x < level->w; x++) {

				int bpp = level->format->BytesPerPixel;
				/* Here p is the address to the pixel we want to retrieve */
				Uint8 *p = (Uint8 *)level->pixels + y * level->pitch + x * bpp;
				uint32_t pixel = 0;

				switch (bpp) {
				case 1:
					pixel = *p;
					break;

				case 2:
					pixel = *(uint16_t *)p;
					break;

				case 3:
					if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
						pixel = p[0] << 16 | p[1] << 8 | p[2];
					}
					else {
						pixel = p[0] | p[1] << 8 | p[2] << 16;
					}
					break;

				case 4:
					pixel = *(uint32_t *)p;
					break;

				default:
					pixel = 0;       /* shouldn't happen, but avoids warnings */
				}


				if (pixel > 0) {
					int height = (int) (pixel - 127) / 25.6;
					this->tiles[x][y] = new Tile(scene, floor_node, { x,y }, height);
					if(max_height < height) { max_height = height; }
					if(min_height > height) { min_height = height; }
				}
			}
		}
		printf("Max level height: %f, min: %f\n", max_height, min_height);
	}

	void SystemLevelManager::update(float dt){
		auto entities = getEntities();

		for(auto& entity : entities){
			auto& trans = entity.getComponent<ComponentTransform>();
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

			// :TODO: vector lerps would be nice... (as would swizzles)
			trans.position.x = Ogre::Math::lerp<float>(pos.position.x, pos.position.x + gm.delta.x, gm.motion_progress);
			trans.position.y = current_height;
			trans.position.z = Ogre::Math::lerp<float>(pos.position.y, pos.position.y + gm.delta.y, gm.motion_progress);

			/////////////////////////////////
			// hop effect
			float a = gm.motion_progress - 0.5;
			// Add sinosodial hop motion
			trans.position.y += (-(a*a) + 0.25f) * gm.max_jump_height * 2.0f;
			// Add motion to get between current tile height and target
			trans.position.y += Ogre::Math::lerp(0.0f, delta_height, gm.motion_progress);
		}
	}
}
