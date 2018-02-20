#ifndef TEMPO_COMPONENT_PLAYERREMOTE_HPP
#define TEMPO_COMPONENT_PLAYERREMOTE_HPP

#include <anax/Component.hpp>

#include <tempo/component/NetworkedComponent.hpp>

namespace tempo
{

struct ComponentPlayerRemote : anax::Component, NetworkedComponent 
{
	ComponentPlayerRemote();

	/////
	// Required for networking
	/////
	ComponentPlayerRemote(sf::Packet p);
	ComponentID getId();
	sf::Packet dumpComponent();
};

} // namespace tempo

#endif
