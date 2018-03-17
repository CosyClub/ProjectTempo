#ifndef TEMPO_COMPONENT_NETWORKED_COMPONENT_HPP
#define TEMPO_COMPONENT_NETWORKED_COMPONENT_HPP

#include <tempo/network/ID.hpp>

#include <anax/Component.hpp>

#include <SFML/Network.hpp>

namespace tempo
{
struct NetworkedComponent : anax::Component{
	virtual ComponentID getId()         = 0;
	virtual sf::Packet  dumpComponent() = 0;
};
}

#endif
