#ifndef TEMPO_COMPONENT_TEAM_HPP
#define TEMPO_COMPONENT_TEAM_HPP

#include <tempo/component/NetworkedComponent.hpp>

#include <anax/Component.hpp>

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

namespace tempo
{
using Facing = glm::ivec2;

enum Team{
	GOODGUYS,
	BADGUYS,
};

struct ComponentTeam : NetworkedComponent {
	Team team;

	ComponentTeam(Team t);

	/////
	// Required for inital network sync
	/////
	ComponentTeam(sf::Packet p);
	sf::Packet  dumpComponent();
	ComponentID getId();
};

}  // namespace tempo

#endif
