#ifndef TEMPO_COMPONENT_NETWORKED_COMPONENT_HPP
#define TEMPO_COMPONENT_NETWORKED_COMPONENT_HPP

#include <tempo/network/QueueID.hpp>

#include <anax/Component.hpp>

#include <SFML/Network.hpp>

namespace tempo{

struct NetworkedComponent
{
	ComponentID ID;

	virtual sf::Packet dumpComponent() =0;
	virtual void restoreComponent(sf::Packet p) =0;
};

}

#endif
