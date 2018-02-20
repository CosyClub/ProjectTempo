#ifndef TEMPO_COMPONENT_PLAYERREMOTE_HPP
#define TEMPO_COMPONENT_PLAYERREMOTE_HPP

#include <anax/Component.hpp>
#include <tempo/component/NetworkedComponent.hpp>

namespace tempo
{

struct ComponentPlayerRemote : anax::Component, NetworkedComponent 
{
	// Required for inital network sync	
	ComponentPlayerRemote(sf::Packet p);
	ComponentID getId();
	sf::Packet dumpComponent();
};

} // namespace tempo

#endif
