////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file LevelManager.hpp
/// \author Jamie Terry
/// \date 2017/11/15
/// \brief Contains declaration of LevelManager system and GridPosition component
////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_ENTITY_GRIDMOTION_HPP
#define TEMPO_ENTITY_GRIDMOTION_HPP

#include <OgreVector2.h>

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/Tile.hpp>
#include <tempo/entity/Transform.hpp>

namespace tempo{
	struct ComponentGridPosition : anax::Component {
		Ogre::Vector2    current;
		Ogre::Vector2    target;
		float            motion_progress;

		ComponentGridPosition();
		ComponentGridPosition(Ogre::Vector2 pos);
		ComponentGridPosition(Ogre::Real x, Ogre::Real y);

		/////////////////////////////////////////////////////////////////////
		/// \brief Causes the component to represent a motion by specified amounts
		/// Only takes effect if entity is not already in motion
		/////////////////////////////////////////////////////////////////////
		bool moveBy(Ogre::Real x, Ogre::Real y);
	};

	class SystemLevelManager : public anax::System<anax::Requires<ComponentTransform, ComponentGridPosition>> {
	private:
		std::vector<std::vector<Tile*>> tiles;
		Ogre::SceneNode* floor_node;
        bool existsTile(Position_t position);

	public:

		SystemLevelManager(Ogre::SceneManager* scene, int size);
		SystemLevelManager(int size);
		SystemLevelManager(Ogre::SceneManager* scene, const char* fileName);

		Ogre::SceneNode* getFloorNode();
		void deleteTile(Ogre::SceneManager* scene, Position_t position);
		void createTile(Ogre::SceneManager* scene, Position_t position);
		void setMaterial(std::string material_name, Position_t position);

		bool placeEntity(EntityID_t id, Position_t position);
		void removeEntity(EntityID_t id, Position_t position);
		std::unordered_set<EntityID_t> getEntitiesOnTile(EntityID_t id, Position_t position);

		void setHeight(float height, Position_t position);
		void setHeight(float height, Position_t position, int width, int length);
		float getHeight(Position_t position);
		void loadLevel(Ogre::SceneManager* scene, const char* fileName, std::vector<std::vector<Tile*>> tiles);
		// Dimensions of grid
		int min_x;
		int min_y;
		int max_x;
		int max_y;

		void update(float dt);
	};
}

#endif
