#ifndef TEMPO_ENTITY_WEAPON_PICKUP_HPP
#define TEMPO_ENTITY_WEAPON_PICKUP_HPP

#include <anax/Component.hpp>

#include <tempo/component/NetworkedComponent.hpp>

namespace tempo
{

struct ComponentPickup : anax::Component, NetworkedComponent
{
	/////
	// Required for networking
	/////
	ComponentHealth(sf::Packet p);
	sf::Packet dumpComponent();
	ComponentID getId();
}

} // namespace tempo

#endif
