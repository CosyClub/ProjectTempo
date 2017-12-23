#include <tempo/Tile.hpp>
#include <Ogre.h>

namespace tempo{
	Tile::Tile(Ogre::SceneManager* scene, Ogre::SceneNode* floor_node, Vec2s position, float height) {
		this->height = height;
		this->position.x = position.x;
		this->position.y = position.y;

		floorpiece = scene->createEntity("meshes/tile.mesh");
		node_tile = floor_node->createChildSceneNode();
		node_tile->attachObject(floorpiece);
		node_tile->setPosition(position.x, -5+height, position.y);
	}

	Tile::Tile(Vec2s position, float height) {
		this->height = height;
		this->position.x = position.x;
		this->position.y = position.y;
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
			this->node_tile->setPosition(this->position.x, -5 + height, this->position.y);
		}
	}

	float Tile::getHeight() {
		return this->height;
	}
}
