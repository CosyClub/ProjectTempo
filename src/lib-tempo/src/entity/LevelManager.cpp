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

	ComponentGridPosition::ComponentGridPosition(Vec2s pos) : ComponentGridPosition(pos.x, pos.y){
		// empty body
	}

	ComponentGridPosition::ComponentGridPosition(int x, int y){
		this->position = {x,y};
	}

	ComponentGridMotion::ComponentGridMotion() {
		this->delta  = {0,0};
		this->motion_progress = 0;
		this->max_jump_height = 1.0f;
		this->target_locked = false;
	}

	bool ComponentGridMotion::moveBy(int dx, int dy){
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
				tiles[i][j] = new Tile({i,j}, 0);
			}
		}
	}

	// SystemLevelManager::SystemLevelManager(Ogre::SceneManager* scene, sf::Packet packet) {
	// 	char *heightMap, *zoneMap;
	// 	SystemLevelManager(scene, heightMap, zoneMap);
	// }

	SystemLevelManager::SystemLevelManager(Ogre::SceneManager* scene, const char* heightMap, const char* zoneMap) : tiles(100, std::vector<Tile*>(100)), player_spawn_zone(100*100) {
		loadLevel(scene, heightMap);
		loadZones(zoneMap);
	}

	SystemLevelManager::SystemLevelManager(const char* heightMap, const char* zoneMap) : tiles(100, std::vector<Tile*>(100)), player_spawn_zone(100*100) {
		//loadLevel(heightMap);
		loadZones(zoneMap);
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

	bool SystemLevelManager::placeEntity(EntityID_t id, Vec2s position) {
		if (existsTile(position)) {
			return tiles[position.x][position.y]->placeEntity(id);
		} else {
			return false;
			std::cout <<" Can't put entity on non-existent tile";
		}
	}

	void SystemLevelManager::removeEntity(EntityID_t id, Vec2s position) {
		if (existsTile(position)) {
			tiles[position.x][position.y]->removeEntity(id);
		} else {
			std::cout <<" Can't remove entity from non-existent tile";
		}
	}

	std::unordered_set<EntityID_t> SystemLevelManager::getEntitiesOnTile(EntityID_t id, Vec2s position) {
		if (existsTile(position)) {
			return tiles[position.x][position.y]->getEntities(id);
		} else {
			std::unordered_set<EntityID_t> empty_set;
			return empty_set;
			std::cout <<" Can't get entities from non-existent tile";
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

	void SystemLevelManager::loadLevel(Ogre::SceneManager* scene, const char* fileName) {

		SDL_Surface* level = SDL_LoadBMP(fileName);

		floor_node = scene->getRootSceneNode()->createChildSceneNode();

		for (int y = 0; y < level->h; y++) {
			for (int x = 0; x < level->w; x++) {

				int bpp = level->format->BytesPerPixel;
				/* Here p is the address to the pixel we want to retrieve */
				Uint8 *p = (Uint8 *)level->pixels + y * level->pitch + x * bpp;
				uint32_t pixel = 0;

				pixel = *p;

				if (pixel > 0) {
					int height = (int) (pixel - 127) / 25.6;
					this->tiles[x][y] = new Tile(scene, floor_node, { x,y }, height);
				}
			}
		}
	}

	void SystemLevelManager::loadLevel(const char* fileName) {

		SDL_Surface* level = SDL_LoadBMP(fileName);

		for (int y = 0; y < level->h; y++) {
			for (int x = 0; x < level->w; x++) {

				int bpp = level->format->BytesPerPixel;
				/* Here p is the address to the pixel we want to retrieve */
				Uint8 *p = (Uint8 *)level->pixels + y * level->pitch + x * bpp;
				uint32_t pixel = 0;

				pixel = *p;

				if (pixel > 0) {
					int height = (int) (pixel - 127) / 25.6;
					this->tiles[x][y] = new Tile({ x,y }, height);
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

	void SystemLevelManager::update(float dt){
		auto entities = getEntities();

		for(auto& entity : entities){
			auto& trans = entity.getComponent<ComponentTransform>();
			auto& pos   = entity.getComponent<ComponentGridPosition>();
			auto& gm    = entity.getComponent<ComponentGridMotion>();

			// :TODO: Should entities have different movement speeds?
			//        if so, add move_speed ComponentGridMotion and use
			//        rather than constant value here
			if(gm.delta != Vec2s::Origin){
				gm.motion_progress += dt * 10.0f;
				if(gm.motion_progress >= 1){
					pos.position += gm.delta;
					gm.delta = {0,0};
					gm.motion_progress = 0;
					gm.target_locked   = false;
				}
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
