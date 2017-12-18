#ifndef TEMPO_LEVELMANAGER_HPP
#define TEMPO_LEVELMANAGER_HPP

#include <tempo/LevelManager.hpp>
#include <Ogre.h>
#include "Tile.hpp"

namespace tempo{
	class LevelManager {
	private:
		std::vector<std::vector<Tile*>> tiles;
		Ogre::SceneNode* floor_node;
        bool existsTile(Position_t position);

	public:

		LevelManager(Ogre::SceneManager* scene, int size);
		LevelManager(int size);
		LevelManager(Ogre::SceneManager* scene, const char* fileName);


		Ogre::SceneNode* getFloorNode();
		void deleteTile(Ogre::SceneManager* scene, Position_t position);
		void createTile(Ogre::SceneManager* scene, Position_t position);
		void setMaterial(std::string material_name, Position_t position);

		bool placeEntity(EntityID_t id, Position_t position);
		void removeEntity(EntityID_t id, Position_t position);
		std::unordered_set<EntityID_t> getEntities(EntityID_t id, Position_t position);

		void setHeight(float height, Position_t position);
		void setHeight(float height, Position_t position, int width, int length);
		float getHeight(Position_t position);
		void loadLevel(Ogre::SceneManager* scene, const char* fileName, std::vector<std::vector<Tile*>> tiles);
	};
}

#endif