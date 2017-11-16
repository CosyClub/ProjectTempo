#ifndef TEMPO_TILE
#define TEMPO_TILE

#include <tempo/Types.hpp>
#include <Ogre.h>
#include <unordered_set>

class Tile{
private:
	std::unordered_set<EntityID_t> entities;
	int height;
 	Ogre::Entity* floorpiece;
 	Ogre::SceneNode* node_tile;

public:

	Tile(Ogre::SceneManager* scene,Ogre::SceneNode* floor_node, Position_t position, int height);
	Tile(int height);

	void deleteFloorpiece(Ogre::SceneManager* scene);
	void createFloorpiece(Ogre::SceneManager* scene);
	void setMaterial(std::string material_name);

	bool placeEntity(EntityID_t id);
	void removeEntity(EntityID_t id);
};
#endif
