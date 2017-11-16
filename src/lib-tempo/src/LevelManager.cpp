#include <tempo/LevelManager.hpp>
#include <tempo/Tile.hpp>

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
				tiles[i][j] = new Tile( 0);
			}
		}
	}

	Ogre::SceneNode* LevelManager::getFloorNode(){
  		return floor_node;
	}

	void LevelManager::deleteTile(Ogre::SceneManager* scene, Position_t position) {
  		tiles[position.x][position.z]->deleteFloorpiece(scene);
	}

	void LevelManager::createTile(Ogre::SceneManager* scene, Position_t position) {
  		tiles[position.x][position.z]->createFloorpiece(scene);
	}

	void LevelManager::setMaterial(std::string material_name, Position_t position) {
  		tiles[position.x][position.z]->setMaterial(material_name);
	}

	bool LevelManager::placeEntity(EntityID_t id, Position_t position) {
		tiles[position.x][position.z]->placeEntity(id);
	}

	void LevelManager::removeEntity(EntityID_t id, Position_t position) {
		tiles[position.x][position.z]->removeEntity(id);
	}
