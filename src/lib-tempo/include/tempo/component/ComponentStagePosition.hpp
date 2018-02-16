#ifndef TEMPO_COMPONENT_STAGE_POSITION_HPP
#define TEMPO_COMPONENT_STAGE_POSITION_HPP
#include <anax/Component.hpp>

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

namespace tempo{
	struct ComponentStagePosition : anax::Component {
	private:
		// a non empty set of all positions that is occupied by this entity
		// the first element is the origin of the entity
		std::vector<glm::ivec2> occupied;
	public:
		// generate an entity that occupies an inclusive rectangular shape
		// the bottom left is the origin
		ComponentStagePosition(glm::ivec2 bottom_left, glm::ivec2 top_right);
		// generate an entity that only occupies a single square
		ComponentStagePosition(glm::ivec2 position);

		// returns all the positions that this entity occupies
		std::vector<glm::ivec2> getOccupied();
		// returns the origin of the entity
		glm::ivec2 getOrigin();
	};
}

#endif
