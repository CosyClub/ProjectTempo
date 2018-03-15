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
	DISCONNECT,
	ENTITY_CREATION,
	ENTITY_DELETION,
	MOVEMENT_INTENT_UPDATES,
	MOVEMENT_UPDATES,
	COMBO_UPDATES,
	SYSTEM_ATTACK,
	SYSTEM_HEALTH,

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
	BUTTON_GROUP,       // 4
	COMBO,              // 5
	AI,                 // 6
	HEALTH,             // 7
	MODEL,              // 8
	PICKUP,             // 9
	PLAYER_LOCAL,       // 10
	PLAYER_REMOTE,      // 11
	STAGE,              // 12
	STAGE_POSITION,     // 13
	STAGE_ROTATION,     // 14
	STAGE_TRANSLATION,  // 15
	TEAM,               // 16
	TRANSFORM,          // 17
	WEAPON,             // 18

	// End of non-reserved zone
	CID_RESERVED_TOP    // 19
};

sf::Packet &operator<<(sf::Packet &packet, const ComponentID id);
sf::Packet &operator>>(sf::Packet &packet, ComponentID &id);
}

#endif
