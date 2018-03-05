#ifndef TEMPO_ENTITY_WEAPON_PICKUP_HPP
#define TEMPO_ENTITY_WEAPON_PICKUP_HPP

#include <anax/Component.hpp>

#include <tempo/component/NetworkedComponent.hpp>

namespace tempo
{
struct ComponentPickup
    : anax::Component
    , NetworkedComponent {
	/////
	// Required for networking
	/////
	ComponentPickup(sf::Packet p);
	ComponentID getId();
	sf::Packet  dumpComponent();
};

}  // namespace tempo

#endif
