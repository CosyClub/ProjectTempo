#include <tempo/config.hpp>
#include <tempo/time.hpp>
#include <tempo/networkClient.hpp>

#include <iostream>

namespace tempo
{ // If we want strict spacing I'm going to have to commit the below sin (sorry)

sf::Time timeSyncClient(tempo::Clock *clock)
{
	// Create a socket to the server and connect to it
	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect(NET_ADDR, NET_PORT_TS);
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

void listenForServerUpdates(int port)
{
	// TODO this function
	return;
}

int connectToServer(sf::IpAddress *ipAddress, 
                    unsigned short port,
                    unsigned short ourPort)
{
	// TODO this function
	return 0;
}

int requestRole(tempo::ClientRole roleID, tempo::ClientRoleData *roleData)
{
	// TODO this function
	return 0;
}
	
}
