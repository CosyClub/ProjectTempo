////////////////////////////////////////////////////////////////////////////////
/// networkBase.hpp
///
/// Header definitions for use on any Project Tempo program with networking.
/// It is unlikely you will need to include this as the specific client/server
/// header files will include this already automatically.
////////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_NETWORK_BASE_HPP
#define TEMPO_NETWORK_BASE_HPP

#include <SFML/Network.hpp>
#include <SFML/System/Time.hpp>

#include <tempo/entity/SystemQID.hpp>

namespace tempo
{
	////////////////////////////////////////////////////////////////////////
	/// Constant #define's 

	// Default Port for Time Sync (TS) protocol
	#define NET_PORT_TS 1337
	// Default Port for Data (DT) protocol
	#define NET_PORT_DT 1338
	// Default Address - TODO Change this
	#define NET_ADDR "127.0.0.1"
	// Wait time for time sync protocol (millisecs)
	#define TIMESYNC_DELTA 500

	////////////////////////////////////////////////////////////////////////
	/// Client Roles
	
	// Enum with all possible client roles.
	enum ClientRole {
		PLAYER,
		SPECTATOR
	};

	// Struct to contain client role data, for use when a new client is
	// requesting a new role.
	typedef struct {
		char *name;
	} ClientRoleData;

	////////////////////////////////////////////////////////////////////////
	/// Connection Handshake Message ID's
	enum HandshakeID {
		DEFAULT,
		HELLO,
		HELLO_ROG,
		ROLEREQ,
		ROLEREQ_ROG
	};

	////////////////////////////////////////////////////////////////////////
	/// Helper Macros

	// tcpRemoteToStr
	// Returns string with remote host and port of a TCP socket
	//
	// Arguments:
	//         sock - The TCP socket to return the remote host and port of.
	// Returns:
	//         A string with the remote host and port of the given TCP
	//         socket in the form of "HOST:PORT"
	#define tcpRemoteToStr(sock) \
		(sock->getRemoteAddress().toString() + ":" + \
		 std::to_string(client->getRemotePort()))


	////////////////////////////////////////////////////////////////////////
	/// Function Declarations

	// sendMessageToServer
	// Sends a message to the server. No guarentee of delivery given.
	//
	// Arguments:
	//         id      - ID of the System Queue to deliver the message to.
	//         payload - The payload to send to the message queue.
	// Returns:
	//         bool - true if sent, false if unable to send.
	bool sendMessage(tempo::SystemQID id, sf::Packet payload);

	////////////////////////////////////////////////////////////////////////
	/// Utility functions

	// sf::Packet << sf::Packet
	// Concatenates packets together. Adds all data from one packet to the
	// other.
	//
	// Usage:
	// 	sf::Packet p1 << sf::Packet p2
	//
	// Arguments:
	// 	p1	- The packet to which the data is appended
	// 	p2	- The packet to be appended
	// Returns:
	// 	None	- p1 is modified directly
	sf::Packet& operator <<(sf::Packet& p1, sf::Packet& p2);

}

#endif
