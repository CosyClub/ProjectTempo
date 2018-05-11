#ifndef TEMPO_COMPONENT_PARTY_HPP
#define TEMPO_COMPONENT_PARTY_HPP


#include <anax/Component.hpp>

#include <tempo/component/NetworkedComponent.hpp>

namespace tempo
{

struct ComponentParty : anax::Component {
	int party_number; // Which party is this entity in?
	int entity_index; // Which entity is this within the party?

	ComponentParty();

	ComponentParty(uint32_t party_number);

	ComponentParty(sf::Packet p);

	sf::Packet dumpComponent();

	ComponentID getId();

	// Frees the party slot this entity is occupying allowing it to be reused
	// later by some other client joining
	void freePartySlot();

};
}

#endif
