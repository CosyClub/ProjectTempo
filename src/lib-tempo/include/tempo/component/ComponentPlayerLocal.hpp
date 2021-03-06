#ifndef TEMPO_COMPONENT_PLAYERLOCAL_HPP
#define TEMPO_COMPONENT_PLAYERLOCAL_HPP

#include <anax/Component.hpp>

#include <tempo/component/NetworkedComponent.hpp>

namespace tempo
{
struct ComponentPlayerLocal : anax::Component, NetworkedComponent {
	
	ComponentPlayerLocal();

	/////
	// Required for networking
	/////
	ComponentPlayerLocal(sf::Packet p);
	ComponentID getId();
	sf::Packet  dumpComponent();
};

}  // namespace tempo

#endif
