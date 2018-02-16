////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_ENTITY_GRIDMOTION_HPP
#define TEMPO_ENTITY_GRIDMOTION_HPP

#include <anax/World.hpp>
#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/entity/TileMask.hpp>

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

#include <time.h>
#include <limits>
#include <iostream> //TODO do we really need to print things here
#include <SDL.h>

#undef main // SDL likes to define main

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
		glm::vec2 position;

		///< \brief Mask of the tile's that this entity occupies
		TileMask mask;

		///< \brief A tile may be occupied by either:
		/// - A single non-ethereal entity
		/// - Many ethereal entities
		/// This allows, for example, players to move through each other,
		/// but not through collapsed walls/enemies etc.
		bool ethereal;
	public:
		inline const glm::vec2& getPosition(){ return this->position; }
		inline const TileMask& getTileMask(){ return this->mask;     }
		inline const bool      isEthereal (){ return this->ethereal; }

		ComponentGridPosition(glm::vec2 pos    = glm::vec2(0, 0),
		                      TileMask         = tempo::tileMask1by1,
		                      bool is_ethereal = true
		                     );

		ComponentGridPosition(int x, int y,
		                      TileMask         = tempo::tileMask1by1,
		                      bool is_ethereal = true
		                     );

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
		/////////////////////////////////////////////////////////////////////
		ComponentGridMotion(float max_jump_distance = 1.1f,
		                    float movement_speed    = 10.0f,
		                    float max_jump_height   = 1.5f
		                   );

		/////////////////////////////////////////////////////////////////////
		/// \brief Causes the component to represent a motion by specified amounts
		/// Only takes effect if entity is not already in motion
		/////////////////////////////////////////////////////////////////////
		bool beginMovement(int dx, int dy);

		inline bool beginMovement(glm::vec2 delta){ return beginMovement(delta.x, delta.y); }

		/////////////////////////////////////////////////////////////////////
		/// \brief Returns the current movement delta
		/////////////////////////////////////////////////////////////////////
		const glm::vec2& getCurrentMovement();

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

		/////////////////////////////////////////////////////////////////////
		/// \brief Gets value between 0 and 1 indicating how far we have come
		/// in making the motion requested by this component
		/////////////////////////////////////////////////////////////////////
		float getMotionProgress();

		///< \brief How high this entity can jump, determines if it can reach tiles
		float max_jump_height;

		///< \brief Maximum distance (in tiles) this entity can move in a single jump
		float max_jump_distance;

		///< \brief Number of tiles this entity can move per second
		float movement_speed;
	private:
		///< \brief The delta that this entity is trying to move by
	    glm::vec2 delta;

		///< \brief Value between 0 and 1 indicating the progress towards the target
		float motion_progress;

		/// \brief Whether this entity has claimed the target tile as its own,
		/// thus preventing any other entity from moving into it
		bool target_locked;
	};

	/////////////////////////////////////////////////////////////////////
	/// \brief System which has knowledge of the world's geometry.
	/// Hence this system is responsible for rendering the world's tiles,
	/// and managing the movement of entities on the grid
	/////////////////////////////////////////////////////////////////////
	class SystemLevelManager : public anax::System<anax::Requires<ComponentGridPosition,
	                                                              ComponentGridMotion
	                                                              >>
	{
	private:
	  std::vector<std::vector<float>> tile_heights;

		std::vector<glm::vec2> player_spawn_zone;
		uint32_t spawn_zones = 0;

		class GridPositions : public anax::System<anax::Requires<ComponentGridPosition>>{};
		GridPositions grid_positions;

	public:
		/// \brief Value to use as tile height to indicate it does not exist
		static const constexpr float NO_TILE = std::numeric_limits<float>::min();

		SystemLevelManager(anax::World&, int size);
		SystemLevelManager(anax::World&, const char* heightMap, const char* zoneMap);

		bool existsTile(glm::vec2 position);
		bool existsTile(int x, int y);

		void deleteTile(glm::vec2 position);
		void createTile(glm::vec2 position);

		void setHeight(float height, glm::vec2 position);
		void setHeight(float height, glm::vec2 position, int width, int length);
		inline float getHeight(glm::vec2 position){
			return getHeight(position.x, position.y);
		}
		float getHeight(int x, int y);

		void loadLevel(const char* fileName);

		/////////////////////////////////////////////////////////////////////
		/// \brief Returns the width and height of the world, IE: maximum
		/// tile coordinate is 1 less than the returned vector in each dimension
		/////////////////////////////////////////////////////////////////////
		glm::vec2 getWorldSize();

		void loadZones(const char* fileNames);
		glm::vec2 spawn();

		/////////////////////////////////////////////////////////////////////
		/// \brief Handles moving entities with a GridMotionComponent
		/////////////////////////////////////////////////////////////////////
		void update(float dt);
	};
}

#endif
