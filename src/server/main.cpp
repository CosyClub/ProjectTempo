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
    // Store the current time
    
    // Wait a bit
    
    // Store the current time, and send all the times back

    // Close Socket, Delete Socket Resources + Close thread
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
			*t.join();
		} else {
			// Error, cleanup the client we created
			delete client;
		}
		
	}

	listener.close();
}


int main(int argc, const char** argv) {

    // Start up timeSyncThread
    std::thread timeSyncThread (thread_timeSyncListener); 

    timeSyncThread.join();

	return 0;
}
