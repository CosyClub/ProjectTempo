#ifndef TEMPO_LEVELMANAGER
#define TEMPO_LEVELMANAGER

#include <tempo/LevelManager.hpp>
#include <Ogre.h>
#include "Tile.hpp"

class LevelManager {
private:
  std::vector<std::vector<Tile*>> tiles;
  Ogre::SceneNode* floor_node;

public:

	LevelManager(Ogre::SceneManager* scene, int size);
	LevelManager(int size);

	std::vector<EntityID_t> getEntities(int x, int y);


	Ogre::SceneNode* getFloorNode();
	void deleteTile(Ogre::SceneManager* scene, Position_t position);
	void createTile(Ogre::SceneManager* scene, Position_t position);
	void setMaterial(std::string material_name, Position_t position);

    bool placeEntity(EntityID_t id, Position_t position);
	void removeEntity(EntityID_t id, Position_t position);

};

#endif
