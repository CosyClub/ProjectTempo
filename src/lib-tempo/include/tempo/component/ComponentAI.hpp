#ifndef TEMPO_COMPONENT_GRID_AI_HPP
#define TEMPO_COMPONENT_GRID_AI_HPP

#include <anax/Component.hpp>

#include <tempo/component/NetworkedComponent.hpp>
#include <tempo/network/base.hpp>

#include <glm/vec2.hpp>

namespace tempo
{
struct ComponentAI : anax::Component, NetworkedComponent 
{
	enum MoveType{
		MOVE_NONE,
		MOVE_PATH,
		MOVE_PATROL,
		MOVE_WANDER,
	};

	MoveType type;

	bool hitTeleport;
	bool hitSlide;

	ComponentAI(MoveType m, bool teleport, bool slide);

	std::vector<glm::ivec2> path;

	/////
	// Required for networking
	/////
	ComponentAI(sf::Packet p);
	ComponentID getId();
	sf::Packet  dumpComponent();
};

}  // namespace tempo

#endif
