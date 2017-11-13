////////////////////////////////////////////////////////////////////////////
/// \file main.cpp
/// \author Anthony Wharton
/// \date 2017/11/13
/// \brief Contains entry point for RaveCave Server
////////////////////////////////////////////////////////////////////////////

#include <cstdio>
#include <thread>
#include <vector>

#include <SFML/Network.hpp>
#include <SFML/System/Time.hpp>

#define NET_PORT 1337 // Port that we (the server) should run on

void thread_timeSyncer(sf::TcpSocket *client) {

}

void thread_timeSyncListener() {
	sf::TcpListener listener;

	if (listener.listen(NET_PORT) != sf::Socket::Done) {
		// error ..
	}

	std::vector<sf::TcpSocket*> clientSockets;
	std::vector<std::thread*> clientThreads;
	while (true) { //TODO Maybe update this to have some sort of stop case?
		// Make a new client
		sf::TcpSocket *client = new sf::TcpSocket;

		// Listen for the client, when one connects add to List of
		// clients and deal with time sync protocol
		if (listener.accept(*client) == sf::Socket::Done) {
			clientSockets.push_back(client);
			std::thread *t = new std::thread(thread_timeSyncer, client);
		} else {
			// Error, cleanup the client we created
			delete client;
		}
		
	}

	listener.close();
}


int main(int argc, const char** argv) {
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
