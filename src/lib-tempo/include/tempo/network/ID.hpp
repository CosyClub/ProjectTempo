#ifndef TEMPO_NETWORK_ID_HPP
#define TEMPO_NETWORK_ID_HPP

#include <anax/Component.hpp>
#include <anax/Entity.hpp>
#include <anax/System.hpp>

#include <SFML/Network.hpp>

#define SERVERTOLOCAL(ID)                                      \
	std::map<anax::Entity::Id, anax::Entity::Id>::iterator \
		it = tempo::servertolocal.find(ID);            \
	if (it != tempo::servertolocal.end()) {                \
		ID = tempo::servertolocal[ID];                 \
	} else {                                               \
		ID = anax::Entity::Id();                       \
	}

#define LOCALTOSERVER(ID)                                      \
	std::map<anax::Entity::Id, anax::Entity::Id>::iterator \
	 it = tempo::localtoserver.find(ID);                   \
	if (it != tempo::localtoserver.end()) {                \
		ID = tempo::localtoserver[ID];                 \
	} else {                                               \
		ID = anax::Entity::Id();                       \
	}

namespace tempo
{
// Maps that map a server id to a local id, and vice versa
extern std::map<anax::Entity::Id, anax::Entity::Id> servertolocal;
extern std::map<anax::Entity::Id, anax::Entity::Id> localtoserver;

//
// QueueID
// This enum class should contain all the ID's for queues that are used by the
// game to receive messages from the network stack with.
//
enum QueueID {
	QID_RESERVED_BOTTOM,
	// Start of non-reserved zone

	HEARTBEAT,
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
	PARTY,              // 9
	PICKUP,             // 10
	PLAYER_LOCAL,       // 11
	PLAYER_REMOTE,      // 12
	RESPAWN,            // 13
	SPIKES,             // 14
	STAGE,              // 15
	STAGE_POSITION,     // 16
	STAGE_ROTATION,     // 17
	STAGE_TRANSLATION,  // 18
	TEAM,               // 19
	TRANSFORM,          // 20
	WEAPON,             // 21

	// End of non-reserved zone
	CID_RESERVED_TOP    // 22
};

sf::Packet &operator<<(sf::Packet &packet, const ComponentID id);
sf::Packet &operator>>(sf::Packet &packet, ComponentID &id);
}

#endif
