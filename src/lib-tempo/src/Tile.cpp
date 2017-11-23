#include <tempo/Tile.hpp>
#include <Ogre.h>


Tile::Tile(Ogre::SceneManager* scene, Ogre::SceneNode* floor_node, Position_t position, float height) {
	this->height = height;
	this->position.x = position.x;
	this->position.z = position.z;

	floorpiece = scene->createEntity("meshes/tile.mesh");
	node_tile = floor_node->createChildSceneNode();
	node_tile->attachObject(floorpiece);
	node_tile->setPosition(position.x, -5+height, position.z);
}

Tile::Tile(float height) {
	this->height = height;
}

void Tile::deleteFloorpiece(Ogre::SceneManager* scene) {
	node_tile->detachObject(floorpiece);
	scene->destroyEntity(floorpiece);
}

void Tile::createFloorpiece(Ogre::SceneManager* scene) {
	floorpiece = scene->createEntity("meshes/tile.mesh");
	node_tile->attachObject(floorpiece);
}

void Tile::setMaterial(std::string material_name) {
	if(floorpiece) {
		floorpiece->setMaterialName(material_name);
	}
}

bool Tile::placeEntity(EntityID_t id) {
	auto result = entities.insert(id);
	return result.second;
}

void Tile::removeEntity(EntityID_t id) {
	if(entities.find(id) != entities.end())
		entities.erase(id);
}

std::unordered_set<EntityID_t> Tile::getEntities(EntityID_t id) {
	std::unordered_set<EntityID_t> result(this->entities);
	return result;
}

void Tile::setHeight(float height) {
	this->height = height;
	if(this->node_tile) {
		this->node_tile->setPosition(this->position.x, -5 + height, this->position.z);
	}
}

float Tile::getHeight() {
	return this->height;
}
