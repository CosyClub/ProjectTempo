#include <tempo/LevelManager.hpp>
#include <tempo/Tile.hpp>
#include <iostream>

namespace tempo {
	LevelManager::LevelManager(Ogre::SceneManager* scene, int size) : tiles(size, std::vector<Tile*>(size)) {
		floor_node = scene->getRootSceneNode()->createChildSceneNode();

		for(int i = 0; i < size; i++){
			for(int j = 0; j< size; j++){
				tiles[i][j] = new Tile(scene, floor_node, {i,j}, 0);
			}
		}
	}

	LevelManager::LevelManager(int size) : tiles(size, std::vector<Tile*>(size)) {

		for(int i = 0; i < size; i++){
			for(int j = 0; j< size; j++){
				tiles[i][j] = new Tile(0);
			}
		}
	}

	LevelManager::LevelManager(Ogre::SceneManager* scene, std::string fileName) : tiles(100, std::vector<Tile*>(100)) {

		loadLevel(scene, fileName, tiles);
	}

	Ogre::SceneNode* LevelManager::getFloorNode(){
		return floor_node;
	}

	bool LevelManager::existsTile(Position_t position) {
		if( position.x < 0  || position.x >= tiles.size() ||
	    	position.z < 0  || position.z >= tiles.size() )
			return false;

		if( tiles[position.x][position.z])
			return true;
		else
			return false;
 	}

	void LevelManager::deleteTile(Ogre::SceneManager* scene, Position_t position) {
		if (existsTile(position)) {
			tiles[position.x][position.z]->deleteFloorpiece(scene);
		} else {
			std::cout <<" Can't delete non-existent tile";
		}
	}

	void LevelManager::createTile(Ogre::SceneManager* scene, Position_t position) {
		tiles[position.x][position.z]->createFloorpiece(scene);
	}

	void LevelManager::setMaterial(std::string material_name, Position_t position) {
		if (existsTile(position)) {
			tiles[position.x][position.z]->setMaterial(material_name);
		} else {
			std::cout <<" Can't set material to non-existent tile";
		}
	}

	bool LevelManager::placeEntity(EntityID_t id, Position_t position) {
		if (existsTile(position)) {
			return tiles[position.x][position.z]->placeEntity(id);
		} else {
			return false;
			std::cout <<" Can't put entity on non-existent tile";
		}
	}

	void LevelManager::removeEntity(EntityID_t id, Position_t position) {
		if (existsTile(position)) {
			tiles[position.x][position.z]->removeEntity(id);
		} else {
			std::cout <<" Can't remove entity from non-existent tile";
		}
	}

	std::unordered_set<EntityID_t> LevelManager::getEntities(EntityID_t id, Position_t position) {
		if (existsTile(position)) {
			return tiles[position.x][position.z]->getEntities(id);
		} else {
			std::unordered_set<EntityID_t> empty_set;
			return empty_set;
			std::cout <<" Can't get entities from non-existent tile";
		}
	}

	void LevelManager::setHeight(float height, Position_t position) {
		if (existsTile(position)) {
			tiles[position.x][position.z]->setHeight(height);
		} else {
			std::cout <<" Can't set height to non-existent tile";
		}
	}

	void LevelManager::setHeight(float height, Position_t position, int width, int length) {
		for(int i = position.x; i < width+position.x; i++){
			for(int j = position.z; j < length+position.z; j++){
				this->tiles[i][j]->setHeight(height);
			}
		}
	}

	float LevelManager::getHeight(Position_t position) {
		if (existsTile(position)) {
			return tiles[position.x][position.z]->getHeight();
		} else {
			std::cout <<" Can't get height of non-existent tile";
		}
	}

	void LevelManager::loadLevel(Ogre::SceneManager* scene, std::string fileName, std::vector<std::vector<Tile*>> tiles) {

		floor_node = scene->getRootSceneNode()->createChildSceneNode();

		char ch;
		std::fstream fin(fileName, std::fstream::in);

		int filex = 0;
		int filey = 0;

		while (fin >> std::noskipws >> ch) {

			if (ch == '\n') {
				filex = 0;
				filey++;
			}

			else if (ch == '0') {
				filex++;
			}

			else if (ch == '1') {
				tiles[filex][filey] = new Tile(scene, floor_node, { filex,filey }, 0);
				filex++;
			}

			else if (ch == '2') {
				tiles[filex][filey] = new Tile(scene, floor_node, { filex,filey }, 5);
				filex++;
			}

		}
	}
}
