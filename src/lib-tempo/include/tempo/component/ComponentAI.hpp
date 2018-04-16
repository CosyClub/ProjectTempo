#ifndef TEMPO_COMPONENT_GRID_AI_HPP
#define TEMPO_COMPONENT_GRID_AI_HPP

#include <anax/Component.hpp>

#include <tempo/component/NetworkedComponent.hpp>
#include <tempo/network/base.hpp>

#include <glm/vec2.hpp>

namespace tempo
{

enum MoveType{
	MOVE_NONE,
	MOVE_STRAIGHT,
	MOVE_PATH,
	MOVE_PATROL,
	MOVE_WANDER,
	MOVE_SNAKE,
};

struct ComponentAI : anax::Component, NetworkedComponent 
{
	MoveType type;
	glm::ivec2 prevPos;

	bool hitTeleport;
	bool hitSlide;

	int index;

	ComponentAI(MoveType m, bool teleport, bool slide, int index);
	ComponentAI(MoveType m, bool teleport, bool slide);
	ComponentAI(MoveType m, bool teleport, bool slide, std::deque<glm::ivec2> p);


	std::deque<glm::ivec2> path;

	/////
	// Required for networking
	/////
	ComponentAI(sf::Packet p);
	ComponentID getId();
	sf::Packet  dumpComponent();
};

}  // namespace tempo

#endif
