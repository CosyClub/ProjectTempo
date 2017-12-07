///////////////////////////////////////////////////////////////////////////////
/// entity/SystemQID.hpp
///
/// Contains an enum, that is used to label messages sent between systems.
////////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_NETWORK_ECS_SYSTEMS_HPP
#define TEMPO_NETWORK_ECS_SYSTEMS_HPP

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
	enum class SystemQID:unsigned short {
		NETWORK_TEST=0,
		ANOTHER_UNUSED_MESSAGE_DELETE_ME=1
	};
}

#endif
