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
	class SystemLevelManager;

	/////////////////////////////////////////////////////////////////////
	/// \brief Stores information regarding an entities position on the grid,
	/// as well as which tiles the entity occupies
	/// \note Cannot directly modify the values of this component, must instead
	/// call methods of SystemLevelManager which will update the fields of this
	/// component, or alternatively have them be modified via a ComponentGridMotion
	/////////////////////////////////////////////////////////////////////
	class ComponentGridPosition : public anax::Component {
		friend SystemLevelManager;
	private:
		///< \brief The position of the center of this entity
		Vec2s position;

		///< \brief Mask of the tile's that this entity occupies
		TileMask mask;
	public:
		inline const Vec2s&    getPosition(){ return this->position; }
		inline const TileMask& getTileMask(){ return this->mask;     }

		ComponentGridPosition(SystemLevelManager& manager, Vec2s pos = {0,0}, TileMask = tempo::tileMask1by1);
		ComponentGridPosition(SystemLevelManager& manager, int x, int y,      TileMask = tempo::tileMask1by1);

		/////////////////////////////////////////////////////////////////////
		/// \brief Returns the mask of tiles that are occupied by both this and the
		/// other entity. Returned TileMask is centred on same position as
		/// this->mask
		/////////////////////////////////////////////////////////////////////
		TileMask isCollidingWith(const ComponentGridPosition& other);
	};

	/////////////////////////////////////////////////////////////////////
	/// \brief Represents the motion of entities on the grid
	/////////////////////////////////////////////////////////////////////
	class ComponentGridMotion : public anax::Component {
		friend SystemLevelManager;
	public:
		/////////////////////////////////////////////////////////////////////
		/// \brief Creates a new component for grid motion, initially represents
		/// no motion in progress.
		/// \param max_jump_height - How high this entity can jump as a part
		/// of a single hop
		/// \todo -> should large entities be able to jump multiple tiles at once?
		/////////////////////////////////////////////////////////////////////
		ComponentGridMotion(float max_jump_height = 1.5f);

		/////////////////////////////////////////////////////////////////////
		/// \brief Causes the component to represent a motion by specified amounts
		/// Only takes effect if entity is not already in motion
		/////////////////////////////////////////////////////////////////////
		bool beginMovement(int dx, int dy);

		inline bool beginMovement(Vec2s delta){ return beginMovement(delta.x, delta.y); }

		/////////////////////////////////////////////////////////////////////
		/// \brief Returns the current movement delta
		/////////////////////////////////////////////////////////////////////
		const Vec2s& getCurrentMovement();

		/////////////////////////////////////////////////////////////////////
		/// \brief Returns true if the entity is currently undergoing some motion
		/////////////////////////////////////////////////////////////////////
		bool isMoving();

		/////////////////////////////////////////////////////////////////////
		/// \brief Returns true if the movement is locked,
		/// IE: the entity must complete the motion in question before some other
		/// can be started. When true, this guaranties that the entity will end up
		/// on the target tile. When false, the movement could fail causing the entity
		/// to return to where it started
		/// \todo :TODO: If we support entities jumping multiple tiles at once
		/// then maybe it should jump back to the tile closest to where it hit the obstacle
		/// rather than all the way back to the start?
		/////////////////////////////////////////////////////////////////////
		bool isMovementLocked();
	private:
		///< \brief The delta that this entity is trying to move by
	  Vec2s delta;

		///< \brief Value between 0 and 1 indicating the progress towards the target
		float motion_progress;

		///< \brief How high this entity can jump, determines if it can reach tiles
		float max_jump_height;

		/// \brief Whether this entity has claimed the target tile as its own,
		/// thus preventing any other entity from moving into it
		bool target_locked;
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

		void setHeight(float height, Vec2s position);
		void setHeight(float height, Vec2s position, int width, int length);
		inline float getHeight(Vec2s position){
			return getHeight(position.x, position.y);
		}
		float getHeight(int x, int y);

		void loadLevel(Ogre::SceneManager* scene, const char* fileName, std::vector<std::vector<Tile*>> tiles);

		/////////////////////////////////////////////////////////////////////
		/// \brief Handles moving entities with a GridMotionComponent
		/////////////////////////////////////////////////////////////////////
		void update(float dt);
	};
}

#endif
