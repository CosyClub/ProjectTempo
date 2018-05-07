#ifndef TEMPO_COMPONENT_RESPAWN_HPP
#define TEMPO_COMPONENT_RESPAWN_HPP

#include <anax/Component.hpp>

#include <tempo/component/NetworkedComponent.hpp>

#include <glm/vec2.hpp>

namespace tempo
{
struct ComponentRespawn : anax::Component, NetworkedComponent {
	
	glm::ivec2 spawn_location;

	ComponentRespawn(glm::ivec2 spawn_location);

	/////
	// Required for networking
	/////
	ComponentRespawn(sf::Packet p);
	ComponentID getId();
	sf::Packet  dumpComponent();
};

}  // namespace tempo

#endif
