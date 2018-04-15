#ifndef TEMPO_COMPONENT_PARTY_HPP
#define TEMPO_COMPONENT_PARTY_HPP


#include <anax/Component.hpp>

#include <tempo/component/NetworkedComponent.hpp>

namespace tempo
{

struct ComponentParty : anax::Component {
	int party_number;

	ComponentParty();

	ComponentParty(sf::Packet p);

	sf::Packet dumpComponent();

	ComponentID getId();

};
}

#endif
