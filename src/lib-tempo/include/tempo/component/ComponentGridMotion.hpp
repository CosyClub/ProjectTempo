////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_COMPONENT_GRIDMOTION_HPP
#define TEMPO_COMPONENT_GRIDMOTION_HPP

#include <anax/World.hpp>
#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/entity/TileMask.hpp>
#include <tempo/math/Vector.hpp>

#include <time.h>
#include <limits>
#include <iostream> //TODO do we really need to print things here
#include <SDL.h>

#undef main // SDL likes to define main

namespace tempo{

	/////////////////////////////////////////////////////////////////////
	/// \brief Represents the motion of entities on the grid
	/////////////////////////////////////////////////////////////////////
	class ComponentGridMotion : public anax::Component {
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

		void setDelta(Vec2s delta);

		void setMotionProgress(float motion_progress);

		void setTargetLocked(bool tragte_locked);

	private:
		///< \brief The delta that this entity is trying to move by
	  Vec2s delta;

		///< \brief Value between 0 and 1 indicating the progress towards the target
		float motion_progress;

		/// \brief Whether this entity has claimed the target tile as its own,
		/// thus preventing any other entity from moving into it
		bool target_locked;
	};
}

#endif
