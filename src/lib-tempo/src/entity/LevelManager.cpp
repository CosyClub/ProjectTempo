////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file LevelManager.cpp
/// \author Jamie Terry
/// \date 2017/11/15
/// \brief Contains definition of LevelManager functions
////////////////////////////////////////////////////////////////////////////

#include <OgreMath.h>

#include <tempo/entity/LevelManager.hpp>
#include <iostream>

#include <SDL.h>
#undef main // SDL likes to define main

namespace tempo{
	ComponentGridPosition::ComponentGridPosition() : ComponentGridPosition(0,0){
		// empty body
	}

	ComponentGridPosition::ComponentGridPosition(Ogre::Vector2 pos) : ComponentGridPosition(pos.x, pos.y){
		// empty body
	}

	ComponentGridPosition::ComponentGridPosition(Ogre::Real x, Ogre::Real y){
		this->position = Ogre::Vector2(x,y);
	}

	ComponentGridMotion::ComponentGridMotion() {
		this->delta  = Ogre::Vector2(0,0);
		this->motion_progress = 0;
		this->max_jump_height = 1.0f;
		this->target_locked = false;
	}

	bool ComponentGridMotion::moveBy(Ogre::Real dx, Ogre::Real dy){
		if(this->motion_progress != 0){
			return false;
		}

		this->delta.x = dx;
		this->delta.y = dy;
		return true;
	}

	SystemLevelManager::SystemLevelManager(Ogre::SceneManager* scene, int size) : tiles(size, std::vector<Tile*>(size)) {
		floor_node = scene->getRootSceneNode()->createChildSceneNode();

		for(int i = 0; i < size; i++){
			for(int j = 0; j< size; j++){
				tiles[i][j] = new Tile(scene, floor_node, {i,j}, 0);
			}
		}
	}

	SystemLevelManager::SystemLevelManager(int size) : tiles(size, std::vector<Tile*>(size)) {

		for(int i = 0; i < size; i++){
			for(int j = 0; j< size; j++){
				tiles[i][j] = new Tile(0);
			}
		}
	}

	SystemLevelManager::SystemLevelManager(Ogre::SceneManager* scene, const char* fileName) : tiles(100, std::vector<Tile*>(100)) {

		loadLevel(scene, fileName, tiles);
	}

	Ogre::SceneNode* SystemLevelManager::getFloorNode(){
		return floor_node;
	}

	bool SystemLevelManager::existsTile(Position_t position) {
		// :TODO: define integer vector type rather than using position_t?
		return existsTile(position.x, position.z);
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

	void SystemLevelManager::deleteTile(Ogre::SceneManager* scene, Position_t position) {
		if (existsTile(position)) {
			tiles[position.x][position.z]->deleteFloorpiece(scene);
		} else {
			std::cout <<" Can't delete non-existent tile";
		}
	}

	void SystemLevelManager::createTile(Ogre::SceneManager* scene, Position_t position) {
		tiles[position.x][position.z]->createFloorpiece(scene);
	}

	void SystemLevelManager::setMaterial(std::string material_name, Position_t position) {
		if (existsTile(position)) {
			tiles[position.x][position.z]->setMaterial(material_name);
		} else {
			std::cout <<" Can't set material to non-existent tile";
		}
	}

	bool SystemLevelManager::placeEntity(EntityID_t id, Position_t position) {
		if (existsTile(position)) {
			return tiles[position.x][position.z]->placeEntity(id);
		} else {
			return false;
			std::cout <<" Can't put entity on non-existent tile";
		}
	}

	void SystemLevelManager::removeEntity(EntityID_t id, Position_t position) {
		if (existsTile(position)) {
			tiles[position.x][position.z]->removeEntity(id);
		} else {
			std::cout <<" Can't remove entity from non-existent tile";
		}
	}

	std::unordered_set<EntityID_t> SystemLevelManager::getEntitiesOnTile(EntityID_t id, Position_t position) {
		if (existsTile(position)) {
			return tiles[position.x][position.z]->getEntities(id);
		} else {
			std::unordered_set<EntityID_t> empty_set;
			return empty_set;
			std::cout <<" Can't get entities from non-existent tile";
		}
	}

	void SystemLevelManager::setHeight(float height, Position_t position) {
		if (existsTile(position)) {
			tiles[position.x][position.z]->setHeight(height);
		} else {
			std::cout <<" Can't set height to non-existent tile";
		}
	}

	void SystemLevelManager::setHeight(float height, Position_t position, int width, int length) {
		for(int i = position.x; i < width+position.x; i++){
			for(int j = position.z; j < length+position.z; j++){
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
			auto& gm    = entity.getComponent<ComponentGridMotion>();
			if(gm.delta.x == 0 && gm.delta.y == 0){ continue; }

			auto& trans = entity.getComponent<ComponentTransform>();
			auto& pos   = entity.getComponent<ComponentGridPosition>();

			// :TODO: Should entities have different movement speeds?
			//        if so, add move_speed ComponentGridMotion and use
			//        rather than constant value here
			gm.motion_progress += dt * 10.0f;
			if(gm.motion_progress >= 1){
				pos.position += gm.delta;
				gm.delta = Ogre::Vector2(0,0);
				gm.motion_progress = 0;
				gm.target_locked   = false;
			}

			float current_height = this->getHeight(pos.position);
			float target_height  = this->getHeight(pos.position + gm.delta);
			float delta_height   = target_height - current_height;

			/////////////////////////////////
			// Work out if entity is allowed to go to its current target
			// This depends on:
			// - Delta height between tiles
			// - If target tile is already occupied by another entity
			if(gm.motion_progress >= 0.5f && !gm.target_locked){
				bool can_make_move = true;

				// Check height difference
				if(abs(delta_height) > gm.max_jump_height){
					can_make_move = false;
				}

				// :TODO: add check for another entity already occupying the target tile

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
			trans.position.x = Ogre::Math::lerp(pos.position.x, pos.position.x + gm.delta.x, gm.motion_progress);
			trans.position.y = current_height;
			trans.position.z = Ogre::Math::lerp(pos.position.y, pos.position.y + gm.delta.y, gm.motion_progress);

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
