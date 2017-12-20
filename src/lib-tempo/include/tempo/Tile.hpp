#ifndef TEMPO_TILE_HPP
#define TEMPO_TILE_HPP

#include <tempo/Types.hpp>
#include <tempo/math/Vector.hpp>
#include <Ogre.h>
#include <unordered_set>

namespace tempo{
	class Tile{
	private:
		std::unordered_set<EntityID_t> entities;
		float height;
		Vec2s position;
		Ogre::Entity* floorpiece;
		Ogre::SceneNode* node_tile;

	public:

		Tile(Ogre::SceneManager* scene,Ogre::SceneNode* floor_node, Vec2s position, float height);
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
}
#endif
