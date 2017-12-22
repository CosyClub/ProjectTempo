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

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/Tile.hpp>
#include <tempo/entity/Transform.hpp>
#include <tempo/entity/TileMask.hpp>
#include <tempo/math/Vector.hpp>

namespace tempo{

	/////////////////////////////////////////////////////////////////////
	/// \brief Stores information regarding an entities position on the grid,
	/// and its current motion
	/////////////////////////////////////////////////////////////////////
	struct ComponentGridPosition : anax::Component {
		///< \brief The position of the center of this entity
	  Vec2s position;

		ComponentGridPosition();
		ComponentGridPosition(Vec2s pos);
		ComponentGridPosition(int x, int y);
	};

	/////////////////////////////////////////////////////////////////////
	/// \brief Represents the motion of entities on the grid
	/////////////////////////////////////////////////////////////////////
	struct ComponentGridMotion : anax::Component {
		ComponentGridMotion();

		///< \brief The delta that this entity is trying to move by
	  Vec2s delta;

		///< \brief Value between 0 and 1 indicating the progress towards the target
		float motion_progress;

		///< \brief How high this entity can jump, determines if it can reach tiles
		float max_jump_height;

		/// \brief Whether this entity has claimed the target tile as its own,
		/// thus preventing any other entity from moving into it
		bool target_locked;

		/////////////////////////////////////////////////////////////////////
		/// \brief Causes the component to represent a motion by specified amounts
		/// Only takes effect if entity is not already in motion
		/////////////////////////////////////////////////////////////////////
		inline bool moveBy(Vec2s delta){ return moveBy(delta.x, delta.y); }
		bool moveBy(int dx, int dy);
	};

	/////////////////////////////////////////////////////////////////////
	/// \brief System which has knowledge of the world's geometry.
	/// Hence this system is responsible for rendering the world's tiles,
	/// and managing the movement of entities on the grid
	/////////////////////////////////////////////////////////////////////
	class SystemLevelManager : public anax::System<anax::Requires<ComponentTransform,
	                                                              ComponentGridPosition,
	                                                              ComponentGridMotion
	                                                              >>
	{
	private:
		std::vector<std::vector<Tile*>> tiles;
		Ogre::SceneNode* floor_node;

	public:

		SystemLevelManager(Ogre::SceneManager* scene, int size);
		SystemLevelManager(int size);
		SystemLevelManager(Ogre::SceneManager* scene, const char* fileName);

		bool existsTile(Vec2s position);
		bool existsTile(int x, int y);

		Ogre::SceneNode* getFloorNode();
		void deleteTile(Ogre::SceneManager* scene, Vec2s position);
		void createTile(Ogre::SceneManager* scene, Vec2s position);
		void setMaterial(std::string material_name, Vec2s position);

		bool placeEntity(EntityID_t id, Vec2s position);
		void removeEntity(EntityID_t id, Vec2s position);
		std::unordered_set<EntityID_t> getEntitiesOnTile(EntityID_t id, Vec2s position);

		void setHeight(float height, Vec2s position);
		void setHeight(float height, Vec2s position, int width, int length);
		inline float getHeight(Vec2s position){
			return getHeight(position.x, position.y);
		}
		float getHeight(int x, int y);

		void loadLevel(Ogre::SceneManager* scene, const char* fileName, std::vector<std::vector<Tile*>> tiles);

		void update(float dt);
	};
}

#endif
