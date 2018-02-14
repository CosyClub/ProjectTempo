////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_SYSTEM_LEVELMANAGER_HPP
#define TEMPO_SYSTEM_LEVELMANAGER_HPP

#include <anax/World.hpp>
#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/entity/TileMask.hpp>
#include <tempo/entity/ComponentGridMotion.hpp>
#include <tempo/entity/ComponentGridPosition.hpp>
#include <tempo/math/Vector.hpp>

#include <time.h>
#include <limits>
#include <iostream> //TODO do we really need to print things here
#include <SDL.h>

#undef main // SDL likes to define main

namespace tempo{
	class SystemLevelManager;

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

		std::vector<Vec2s> player_spawn_zone;
		uint32_t spawn_zones = 0;

		class GridPositions : public anax::System<anax::Requires<ComponentGridPosition>>{};
		GridPositions grid_positions;

	public:
		/// \brief Value to use as tile height to indicate it does not exist
		static const constexpr float NO_TILE = std::numeric_limits<float>::min();

		SystemLevelManager(anax::World&, int size);
		SystemLevelManager(anax::World&, const char* heightMap, const char* zoneMap);

		bool existsTile(Vec2s position);
		bool existsTile(int x, int y);

		void deleteTile(Vec2s position);
		void createTile(Vec2s position);

		void setHeight(float height, Vec2s position);
		void setHeight(float height, Vec2s position, int width, int length);
		inline float getHeight(Vec2s position){
			return getHeight(position.x, position.y);
		}
		float getHeight(int x, int y);

		void loadLevel(const char* fileName);

		/////////////////////////////////////////////////////////////////////
		/// \brief Returns the width and height of the world, IE: maximum
		/// tile coordinate is 1 less than the returned vector in each dimension
		/////////////////////////////////////////////////////////////////////
		Vec2s getWorldSize();

		void loadZones(const char* fileNames);
		Vec2s spawn();

		/////////////////////////////////////////////////////////////////////
		/// \brief Handles moving entities with a GridMotionComponent
		/////////////////////////////////////////////////////////////////////
		void update(float dt);
	};
}

#endif
