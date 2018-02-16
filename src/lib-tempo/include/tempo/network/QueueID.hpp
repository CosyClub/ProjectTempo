#ifndef TEMPO_NETWORK_ECS_SYSTEMS_HPP
#define TEMPO_NETWORK_ECS_SYSTEMS_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <SFML/Network.hpp>

namespace tempo
{
	// SystemQID
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
	enum SystemQID {
		QID_RESERVED_BOTTOM,
		//Start of non-reserved zone
		
		PLAYER_UPDATES,
		ENTITY_CREATION,

		//End of non-reserved zone
		QID_RESERVED_TOP
	};

	enum Component_ID
	{
		CID_RESERVED_BOTTOM,
		//Start of non-reserved zone
		
		//End of non-reserved zone
		CID_RESERVED_TOP
	};

	extern void (*restore_pointer[Component_ID::CID_RESERVED_TOP])(anax::Entity& e, sf::Packet p);

	extern sf::Packet (*dump_pointer[Component_ID::CID_RESERVED_TOP])(anax::Entity& e);
}

#endif
