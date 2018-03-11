#ifndef TEMPO_COMPONENT_STAGE_ROTATION_HPP
#define TEMPO_COMPONENT_STAGE_ROTATION_HPP

#include <tempo/component/NetworkedComponent.hpp>

#include <anax/Component.hpp>

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

namespace tempo
{
using Facing = glm::ivec2;

extern glm::ivec2 NORTH;
extern glm::ivec2 EAST;
extern glm::ivec2 SOUTH;
extern glm::ivec2 WEST;


struct ComponentStageRotation
    : anax::Component
    , NetworkedComponent {
	Facing facing;
	Facing previousFacing = { 0,0 };

	ComponentStageRotation(Facing f);

	/////
	// Required for inital network sync
	/////
	ComponentStageRotation(sf::Packet p);
	sf::Packet  dumpComponent();
	ComponentID getId();
};

}  // namespace tempo

#endif
