#ifndef TEMPO_NETWORK_BASE_HPP
#define TEMPO_NETWORK_BASE_HPP

#include <anax/World.hpp>

#include <tempo/network/queue.hpp>
#include <tempo/network/QueueID.hpp>
#include <tempo/structures.hpp>

#include <SFML/Network.hpp>
#include <SFML/System/Time.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <iostream>

namespace tempo
{
	////////////////////////////////////////////////////////////////////////
	/// Constant #define's 

	// Default Address
	#define DEFAULT_ADDR "0.0.0.0"
	// Default Port for Incoming Socket
        #define DEFAULT_PORT_IN  1337
	// Default Port for the Outgoing Socket
	#define DEFAULT_PORT_OUT 1338
	// Default Port for Time Sync (TS) protocol
	#define DEFAULT_PORT_TS  1339

	// Wait time for time sync protocol (millisecs)
	#define TIMESYNC_DELTA 100
	// Number of times to repeat time sync exchange
	#define TIMESYNC_ITERS 10

	// Reserved client ID for a null client
	#define NO_CLIENT_ID 0
	#define THIS_IS_NOT_THE_CLIENT_YOU_ARE_LOOKING_FOR NO_CLIENT_ID

	////////////////////////////////////////////////////////////////////////
	/// Client ID & Roles
	
	// Enum with all possible client roles.
	enum ClientRole {
		NO_ROLE,
		PLAYER,
		SPECTATOR
	};

	// Struct to contain client role data, for use when a new client is
	// requesting a new role.
	typedef struct {
		std::string name;
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
	/// Sockets & IP Addresses & Ports
	extern sf::UdpSocket sock_i;  // Incoming  Socket
	extern sf::UdpSocket sock_o;  // Outgoing  Socket
	// TCP Time Sync Sockets dealt with in functions dynamically

	static sf::IpAddress addr_l = sf::IpAddress::getLocalAddress();
	extern sf::IpAddress addr_r;  // Remote Address
	extern unsigned short port_ci;  // Client Incoming Port
	extern unsigned short port_co;  // Client Outgoing Port
	extern unsigned short port_si;  // Server Incoming Port
	extern unsigned short port_so;  // Server Outgoing Port
	extern unsigned short port_st;  // Server Time Sync Port

	////////////////////////////////////////////////////////////////////////
	/// Function Declarations

	// bindSocket
	// Binds the game's internal UDP sockets to a given port.
	//
	// Arguments:
	//         socket - Character, one of 'i', 'o' and 'h' for the incoming,
	//                  outgoing and handshake sockets respectively.
	//         port   - The port on which to bind the socket
	// Returns:
	//         bool - true if bound, false if unable to bind (port already
	//                in use elsewhere in system, or socket is already bound
	//                to another port.
	bool bindSocket(char socket, unsigned short port);

	// sortPacket
	// Takes a message packet, and then sorts it into the correct queue
	//
	// Arguments:
	// 	packet	- The packet that needs to be sent to the relevant queue
	// Returns:
	// 	bool - true iff packet sorted correctly, false 
	// 	otherwise.
	bool sortPacket(sf::Packet p);


	////////////////////////////////////////////////////////////////////////
	/// Helper Macros

	// tcpRemoteToStr
	// Returns string with remote host and port of a TCP socket
	//
	// Arguments:
	//         sock	- The TCP socket to return the remote host and port of.
	// Returns:
	//         A string with the remote host and port of the given TCP
	//         socket in the form of "HOST:PORT"
	#define tcpRemoteToStr(sock) \
		(sock->getRemoteAddress().toString() + ":" + \
		 std::to_string(client->getRemotePort()))


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
	
	// sf::Packet << tempo::ClientRoleData
	// sf::Packet >> tempo::ClientRoleData
	//
	// Adds/Removes ClientRoleData from a Packet.
	//
	// Usage:
	// 	sf::Packet p << tempo::ClientRoleData c
	// 	sf::Packet p >> tempo::ClientRoleData c
	sf::Packet& operator <<(sf::Packet &p, const ClientRoleData &c);
	sf::Packet& operator >>(sf::Packet &p, ClientRoleData &c);

	sf::Packet& operator <<(sf::Packet& packet, const anax::Entity::Id id);
	sf::Packet& operator >>(sf::Packet& packet, anax::Entity::Id& id);

	sf::Packet& operator <<(sf::Packet& packet, const sf::Time &t);
	sf::Packet& operator >>(sf::Packet& packet, sf::Time& t);

	sf::Packet& operator <<(sf::Packet& packet, const glm::vec2 &v);
	sf::Packet& operator >>(sf::Packet& packet, glm::vec2& v);

	sf::Packet& operator <<(sf::Packet& packet, const glm::ivec2 &v);
	sf::Packet& operator >>(sf::Packet& packet, glm::ivec2& v);

	sf::Packet& operator <<(sf::Packet& packet, const glm::vec3 &v);
	sf::Packet& operator >>(sf::Packet& packet, glm::vec3& v);

	sf::Packet& operator <<(sf::Packet& packet, const glm::ivec3 &v);
	sf::Packet& operator >>(sf::Packet& packet, glm::ivec3& v);

	sf::Packet splitPacket(sf::Packet& packet, uint32_t size);


}

#endif
