#ifndef TEMPO_COMPONENT_STAGE_POSITION_HPP
#define TEMPO_COMPONENT_STAGE_POSITION_HPP
#include <anax/Component.hpp>

#include <tempo/component/NetworkedComponent.hpp>

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

namespace tempo
{
	
struct ComponentStagePosition : anax::Component, NetworkedComponent
{
	public:
		// A non empty set of all positions that is occupied by this 
		// entity. The first element is the origin of the entity.
		std::vector<glm::ivec2> occupied;

	public:
		// Generate an entity that occupies an inclusive rectangular 
		// shape the bottom left is the origin.
		ComponentStagePosition(glm::ivec2 bottom_left, glm::ivec2 top_right);
		// Generate an entity that only occupies a single square.
		ComponentStagePosition(glm::ivec2 position);
	
		// DO NOT CALL: Moves the occupied position by a delta
		void movePosition(glm::ivec2 delta);
		// DO NOT CALL: Sets the occupied positions
		void setPosition(glm::ivec2 bottom_left, glm::ivec2 top_right);
		void setPosition(glm::ivec2 position);

		// Returns all the positions that this entity occupies.
		std::vector<glm::ivec2> getOccupied();
		// Returns the origin of the entity.
		glm::ivec2 getOrigin();

		/////
		// Required for inital network sync
		/////
		ComponentStagePosition(sf::Packet p);
		sf::Packet dumpComponent();
		ComponentID getId();
};

} // namespace tempo

#endif
