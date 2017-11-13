////////////////////////////////////////////////////////////////////////////
/// \file main.cpp
/// \author Anthony Wharton
/// \date 2017/11/13
/// \brief Contains entry point for RaveCave Server
////////////////////////////////////////////////////////////////////////////

#include <cstdio>

#include <SFML/Network.hpp>

#define NET_PORT 1337 // Port that we (the server) should run on

int main(int argc, const char** argv){
	/////////////////////////////////////////////////
	// Setup Networking
	sf::UdpSocket socket;

	if (socket.bind(NET_PORT) != sf::Socket::Done) {
		printf("Could not bind port %d, unable to start server.\n", NET_PORT);
		return 1;
	}
	printf("Server is listening on port %d...", NET_PORT);

	// Recieve some stuff
	char data[100];
	std::size_t received;
	sf::IpAddress sender;
	unsigned short port;
	if (socket.receive(data, 100, received, sender, port) != sf::Socket::Done)
	{
		// error...
	}
	printf("Received %d bytes from $d on port %d.\n", received, sender, port);

	return 0;
}
