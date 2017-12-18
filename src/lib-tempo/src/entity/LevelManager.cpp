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
		this->current = Ogre::Vector2(x,y);
		this->target  = Ogre::Vector2(x,y);
		this->motion_progress = 0;
	}

	bool ComponentGridPosition::moveBy(Ogre::Real x, Ogre::Real y){
		if(this->motion_progress != 0){
			return false;
		}

		this->target.x = this->current.x + x;
		this->target.y = this->current.y + y;
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
		if( position.x < 0  || position.x >= tiles.size() ||
	    	position.z < 0  || position.z >= tiles.size() )
			return false;

		if( tiles[position.x][position.z])
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

	float SystemLevelManager::getHeight(Position_t position) {
		if (existsTile(position)) {
			return tiles[position.x][position.z]->getHeight();
		} else {
			std::cout <<" Can't get height of non-existent tile";
		}
	}

	void SystemLevelManager::loadLevel(Ogre::SceneManager* scene, const char* fileName, std::vector<std::vector<Tile*>> tiles) {

		SDL_Surface* level = SDL_LoadBMP(fileName);

		floor_node = scene->getRootSceneNode()->createChildSceneNode();

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
				}
			}
		}
	}

	void SystemLevelManager::update(float dt){
		auto entities = getEntities();

		for(auto& entity : entities){
			auto& trans = entity.getComponent<ComponentTransform>();
			auto& gm    = entity.getComponent<ComponentGridPosition>();

			if(gm.current != gm.target){
				gm.motion_progress += dt / 0.1f;
				if(gm.motion_progress >= 1){
					gm.current = gm.target;
					gm.motion_progress = 0;
				}
			}

			trans.position.x = Ogre::Math::lerp(gm.current.x, gm.target.x, gm.motion_progress);
			trans.position.z = Ogre::Math::lerp(gm.current.y, gm.target.y, gm.motion_progress);;

			// hop effect
			float a = gm.motion_progress - 0.5;
			trans.position.y = (-(a*a) + 0.25f) * 2.0f;

			if(trans.position.x < this->min_x){ trans.position.x = this->min_x; }
			if(trans.position.x > this->max_x){ trans.position.x = this->max_x; }
			if(trans.position.z < this->min_y){ trans.position.z = this->min_y; }
			if(trans.position.z > this->max_y){ trans.position.z = this->max_y; }
		}
	}
}
