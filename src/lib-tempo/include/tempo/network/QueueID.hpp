#ifndef TEMPO_NETWORK_QUEUE_ID_HPP
#define TEMPO_NETWORK_QUEUE_ID_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <SFML/Network.hpp>

namespace tempo
{
	// QueueID
	// This enum class should contain all the ID's for systems that want to
	// recieve messages from the network stack. These cannot be reused more
	// than once on one instance, but can be used on both a client and 
	// server - not necessarily with the same system on both ends.
	//
	// An example might help:
	// 1) SystemA runs on the server, and SystemB runs on the client.
	// 2) We then define a new sys_msg_id "FOOBAR".
	// 3) On launch of the server, SystemA registers it's message queue to
	//    the network code with ID "FOOBAR", so it can recieve messages that
	//    have the ID "FOOBAR".
	// 4) On launch of the client, SystemB registers it's message queue to
	//    the network code with ID "FOOBAR", so it can recieve messages that
	//    have the ID "FOOBAR".
	// 
	enum QueueID {
		QID_RESERVED_BOTTOM,
		//Start of non-reserved zone
		
		HANDSHAKE,
		PLAYER_UPDATES,
		ENTITY_CREATION,
		COMBO_UPDATES,
		SYSTEM_ATTACK,

		//End of non-reserved zone
		QID_RESERVED_TOP
	};

	enum ComponentID
	{
		CID_RESERVED_BOTTOM,			//0
		CID_RESERVED_ERROR,			//1
		//Start of non-reserved zone

		AOEINDICATOR,				//2
		COMBO,					//3
		GRID_AI,				//4
		HEALTH,					//5
		MODEL,					//6
		PICKUP,					//7
		PLAYER_LOCAL,				//8
		PLAYER_REMOTE,				//9
		STAGE,					//10
		STAGE_POSITION,				//11
		STAGE_ROTATION,				//12
		STAGE_TRANSLATION,			//13
		TRANSFORM,				//14
		WEAPON,					//15
		
		//End of non-reserved zone
		CID_RESERVED_TOP
	};

	sf::Packet& operator <<(sf::Packet& packet, const ComponentID id);
	sf::Packet& operator >>(sf::Packet& packet, ComponentID& id);

	extern std::map<anax::Entity::Id, anax::Entity::Id> servertolocal;
	extern std::map<anax::Entity::Id, anax::Entity::Id> localtoserver;
}

#endif
