////////////////////////////////////////////////////////////////////////////
/// \file main.cpp
/// \author Anthony Wharton
/// \date 2017/11/13
/// \brief Contains entry point for RaveCave Server
////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdio>
#include <thread>
#include <vector>

#include <tempo/time.hpp>
#include <SFML/Network.hpp>
#include <SFML/System/Time.hpp>

#define NET_PORT 1337        // Port that we (the server) should run on
#define TIMESYNC_DELTA 500   // Time to wait in time sync protocol (millisecs)
#define BPM 174              // Beats per minutes
#define PLAYER_DELTA 150     // Delta around a beat a player can hit (millisecs)
#define TIME 60000000 / BPM  // Time between beats (microsecs)

// clientToStr - Print Convenience Function
// Takes a pointer to a TCP client socket, and returns a string version of the
// remote address and port in the form ADDRESS:PORT
#define tcpClientToStr(client) (client->getRemoteAddress().toString() + ":" + \
                                std::to_string(client->getRemotePort()))

void thread_timeSyncer(tempo::Clock *clock, sf::TcpSocket *client) {
    std::cout << "Client (" << tcpClientToStr(client) << ") time sync started." 
              << std::endl;
    
    // Store the current time
    sf::Time t1 = clock->get_time();

    // Wait a bit
    std::this_thread::sleep_for (std::chrono::milliseconds(TIMESYNC_DELTA));
    
    // Store the current time, and send all the times back
    sf::Time t2 = clock->get_time();
    sf::Packet packet;
    packet << t1.asMicroseconds() << t2.asMicroseconds();
    if (client->send(packet) != sf::Socket::Done) {
        std::cout << "Tried to send time sync to client (" 
                  << tcpClientToStr(client) << "), but failed." << std::endl;
    }

    std::cout << "Client (" << tcpClientToStr(client) << ") time sync finished."
              << std::endl;

    // Close Socket, Delete Socket Resources + Close thread
    client->disconnect();
    return;
}

void thread_timeSyncListener(tempo::Clock *clock) {
	sf::TcpListener listener;
    	// Listen for a connection
    	if (listener.listen(NET_PORT) != sf::Socket::Done) {
    		std::cout << "Listener socket could not open on port." << NET_PORT
    		          << std::endl;
    	}

	std::vector<sf::TcpSocket*> clientSockets;
	std::vector<std::thread*> clientThreads;
	while (true) { //TODO Maybe update this to have some sort of stop case?
		// Make a new client
		sf::TcpSocket *client = new sf::TcpSocket;

		// Listen for the client, when one connects add to List of
		// clients and deal with time sync protocol
		if (listener.accept(*client) == sf::Socket::Done) {
			std::cout << "Client (" << tcpClientToStr(client) 
			          << ") connected to time-sync, spawning thread.."
			          << std::endl;
			clientSockets.push_back(client);
			std::thread *t = new std::thread(thread_timeSyncer, clock, client);
			clientThreads.push_back(t);
		} else {
			// Error, cleanup the client we created
			std::cout << "Listener reported something, but no client accepted."
			          << std::endl;
			delete client;
		}
	
	    // Clean up finished threads
	    for (int i = 0; i < clientSockets.size(); i++) {
            if (clientSockets.at(i)->getRemoteAddress() 
                == sf::IpAddress::None) {
                if (clientThreads.at(i)->joinable()) {
                    clientSockets.erase(clientSockets.begin()+i);
                    clientThreads.erase(clientThreads.begin()+i);
                }
            }
	    }
	}

	listener.close();
}


int main(int argc, const char** argv) {

    tempo::Clock clock = tempo::Clock(sf::microseconds(TIME),
                                      sf::milliseconds(PLAYER_DELTA));

    // Start up timeSyncThread
    std::thread timeSyncThread (thread_timeSyncListener, &clock); 

    while (true) {
        if (clock.passed_beat()) {
            std::cout << "Server Beat Passed (" << clock.get_time().asSeconds()
                      << ")" << std::endl;
        }
    }

	return 0;
}
