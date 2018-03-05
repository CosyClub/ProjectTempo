#ifndef TEMPO_NETWORK_ID_HPP
#define TEMPO_NETWORK_ID_HPP

#include <anax/Component.hpp>
#include <anax/System.hpp>

#include <SFML/Network.hpp>

namespace tempo
{

// Maps that map a server id to a local id, and vice versa
extern std::map<anax::Entity::Id, anax::Entity::Id> servertolocal;
extern std::map<anax::Entity::Id, anax::Entity::Id> localtoserver;

//
// QueueID
// This enum class should contain all the ID's for queues that are used by the
// game to recieve messages from the network stack with. 
//
enum QueueID {
	QID_RESERVED_BOTTOM,
	// Start of non-reserved zone

	HANDSHAKE,
	ENTITY_CREATION,
	MOVEMENT_INTENT_UPDATES,
	MOVEMENT_UPDATES,
	COMBO_UPDATES,
	SYSTEM_ATTACK,

	// End of non-reserved zone
	QID_RESERVED_TOP
};


//
// ComponentID
// Used by entity synchronisation, in order to identify networked components.
//
enum ComponentID {
	CID_RESERVED_BOTTOM,  // 0
	CID_RESERVED_ERROR,   // 1
	// Start of non-reserved zone

	AOEINDICATOR,       // 2
	ATTACK,             // 3
	COMBO,              // 4
	GRID_AI,            // 5
	HEALTH,             // 6
	MODEL,              // 7
	PICKUP,             // 8
	PLAYER_LOCAL,       // 9
	PLAYER_REMOTE,      // 10
	STAGE,              // 11
	STAGE_POSITION,     // 12
	STAGE_ROTATION,     // 13
	STAGE_TRANSLATION,  // 14
	TRANSFORM,          // 15
	WEAPON,             // 16

	// End of non-reserved zone
	CID_RESERVED_TOP    // 17
};

sf::Packet &operator<<(sf::Packet &packet, const ComponentID id);
sf::Packet &operator>>(sf::Packet &packet, ComponentID &id);

}

#endif
