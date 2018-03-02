////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_SYSTEM_LEVELMANAGER_HPP
#define TEMPO_SYSTEM_LEVELMANAGER_HPP

#include <anax/World.hpp>
#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/entity/TileMask.hpp>
#include <tempo/component/ComponentStageTranslation.hpp>
#include <tempo/component/ComponentStagePosition.hpp>

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

#include <time.h>
#include <limits>

namespace tempo{
	class SystemLevelManager;

	/////////////////////////////////////////////////////////////////////
	/// \brief System which has knowledge of the world's geometry.
	/// Hence this system is responsible for rendering the world's tiles,
	/// and managing the movement of entities on the grid
	/////////////////////////////////////////////////////////////////////
	class SystemLevelManager : public anax::System<anax::Requires<ComponentStagePosition,
	                                                              ComponentStageTranslation
	                                                              >>
	{
	private:
	  std::vector<std::vector<float>> tile_heights;

		std::vector<glm::vec2> player_spawn_zone;
		uint32_t spawn_zones = 0;

		class GridPositions : public anax::System<anax::Requires<ComponentStagePosition>>{};
		GridPositions grid_positions;

	public:
		/// \brief Value to use as tile height to indicate it does not exist
		static const constexpr float NO_TILE = std::numeric_limits<float>::min();

		SystemLevelManager(anax::World&, int size);
		SystemLevelManager(anax::World&, const char* heightMap, const char* zoneMap);

		bool existsTile(glm::vec2 position);
		bool existsTile(unsigned int x, unsigned int y);

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
