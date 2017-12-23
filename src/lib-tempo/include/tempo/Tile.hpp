#ifndef TEMPO_TILE_HPP
#define TEMPO_TILE_HPP

#include <tempo/Types.hpp>
#include <tempo/math/Vector.hpp>
#include <Ogre.h>
#include <unordered_set>

namespace tempo{
	class Tile{
	private:
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

		void setHeight(float height);
		float getHeight();
	};
}
#endif
