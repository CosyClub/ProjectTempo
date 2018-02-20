#ifndef TEMPO_COMPONENT_GRIDAI_HPP
#define TEMPO_COMPONENT_GRIDAI_HPP

#include <anax/Component.hpp>

#include <tempo/component/NetworkedComponent.hpp>

namespace tempo
{

struct ComponentGridAi : anax::Component, NetworkedComponent
{
	// no state needed for AI (yet)
	
	/////
	// Required for networking
	/////
	ComponentGridAi(sf::Packet p);
	ComponentID getId();
	sf::Packet dumpComponent();
};

} // namespace tempo

#endif
