#include <tempo/config.hpp>
#include <tempo/time.hpp>
#include <tempo/networkClient.hpp>

#include <iostream>

namespace tempo
{

sf::Time timeSyncClient(tempo::Clock *clock)
{
	// Create a socket to the server and connect to it
	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect(addr_r, port_st);
	if (status != sf::Socket::Done) {
		std::cout << "Error binding socket" << std::endl;
		return sf::Time::Zero;
	}

	// Initialise time sync protocol variables
	sf::Int64 t0 = clock->get_time().asMicroseconds();
	sf::Int64 t1;
	sf::Int64 t2;
	sf::Int64 t3;

	// Start time sync exchange
	sf::Packet packet;
	packet << t0;
	status = socket.send(packet);
	if (status != sf::Socket::Done) {
		std::cout << "Error sending packet" << std::endl;
		return sf::Time::Zero;
	}

	status = socket.receive(packet);
	if (status != sf::Socket::Done) {
		std::cout << "Error recieving packet" << std::endl;
		return sf::Time::Zero;
	}

	// End time sync exchange, calculate delay in last tranmission from 
	// time sync server.
	t3 = clock->get_time().asMicroseconds();
	packet >> t1 >> t2;
	sf::Int64 delay  = ((t3 - t0) - (t2 - t1)) / 2;

	// Return current time
	return sf::microseconds(t2 + delay);
}

bool sendMessage(tempo::SystemQID id, 
                 sf::Packet payload, 
                 bool isHandshake = false) 
{
	sf::Packet message;

	// Construct message
	message << id;
	message << payload;

	// Send message
	if (isHandshake) {
		return sock_o.send(message, addr_r, port_sh) == sf::Socket::Done;
	} else {
		return sock_o.send(message, addr_r, port_si) == sf::Socket::Done;
	}
}


void listenForServerUpdates()
{
	// Bind to port
	if (!bindSocket('i', port_ci)) {
		std::cout << "Could not bind port %d, used to listen for "
		          << "server updates." << std::endl;
		return;
	}

	// TODO Implement me!

	return;
}

uint32_t handshakeHello()
{
	// Package up payload
	sf::Packet packet;
	packet << static_cast<uint32_t>(HandshakeID::HELLO);
	packet << port_ci;

	// Send HELLO
	sock_o.send(packet, addr_r, port_sh);

	// Recieve HELLO_ROG
	sf::IpAddress sender;
	unsigned short port;
	sock_o.receive(packet, sender, port);

	// Extract Data
	uint32_t id = NO_CLIENT_ID;
	packet >> id;
	packet >> port_si;
	packet >> port_st;
	// TODO Extract entire level data and initialise level

	return id;
}

void handshakeRoleReq(uint32_t id, ClientRole roleID, ClientRoleData &roleData)
{
	if (sock_o.getLocalPort() == 0) {
		if (!bindSocket('o', port_co)) {
			std::cout << "Could not bind socket on port " << port_co
			          << " to connect to server." << std::endl;
			return 0;
		}
	}
	
	// TODO Implement me!
	
	return;
}
	
void connectToAndSyncWithServer(ClientRole roleID, ClientRoleData &roleData)
{
	// Bind outgoing port if not bound
	if (sock_o.getLocalPort() == 0) {
		if (!bindSocket('o', port_co)) {
			std::cout << "Could not bind socket on port " << port_co
			          << " to connect to and sync with server." 
			          << std::endl;
			return;
		}
	}

	// Ensure server update lisener has started - i.e. it's socket is open
	if (sock_i.getLocalPort() == 0) {
		std::cout << "Looks like the listener thread hasn't started, "
		          << "or didn't bind it's socket correctly. Will not "
		          << "connect to server without this!" << std:endl;
		return;
	}

	uint32_t id = handshakeHello();

	return;
}


}
