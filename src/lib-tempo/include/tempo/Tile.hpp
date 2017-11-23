#ifndef TEMPO_TILE
#define TEMPO_TILE

#include <tempo/Types.hpp>
#include <Ogre.h>
#include <unordered_set>

class Tile{
private:
	std::unordered_set<EntityID_t> entities;
	float height;
	Position_t position;
	Ogre::Entity* floorpiece;
	Ogre::SceneNode* node_tile;

public:

	Tile(Ogre::SceneManager* scene,Ogre::SceneNode* floor_node, Position_t position, float height);
	Tile(float height);

	void deleteFloorpiece(Ogre::SceneManager* scene);
	void createFloorpiece(Ogre::SceneManager* scene);
	void setMaterial(std::string material_name);

	bool placeEntity(EntityID_t id);
	void removeEntity(EntityID_t id);
	std::unordered_set<EntityID_t> getEntities(EntityID_t id);

	void setHeight(float height);
	float getHeight();
};
#endif
