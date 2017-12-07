#include <tempo/config.hpp>
#include <tempo/time.hpp>
#include <tempo/networkServer.hpp>

#include <iostream>

// ithTimeSyncCheck - Checks if the i'th time sync thread has completed
// Arguments:
//         sockets - Vector of time sync sockets (implied synced index)
//         threads - Vector of time sync threads (implied synced index)
//         i       - The i`th element of the given vectors to be checked
// Returns:
//         Binary Value as to whether the i`th time sync thread has completed.
#define ithTimeSyncCheck(sockets, threads, i) \
		((sockets.at(i)->getRemoteAddress() == sf::IpAddress::None) \
			&& (threads.at(i)->joinable()))

namespace tempo
{

// Not in header - for use in a thread by server to deal with a client so the 
//                 main time sync thread can continue listening for more clients
void timeSyncHandler(tempo::Clock *clock, sf::TcpSocket *client) 
{
	std::cout << "Client (" << tcpRemoteToStr(client)
	          << ") time sync started." << std::endl;

	// Store the current time
	sf::Time t1 = clock->get_time();

	// Wait a bit
	std::this_thread::sleep_for(std::chrono::milliseconds(TIMESYNC_DELTA));

	// Store the current time, and send all the times back
	sf::Time t2 = clock->get_time();
	sf::Packet packet;
	packet << t1.asMicroseconds() << t2.asMicroseconds();
	if (client->send(packet) != sf::Socket::Done) {
		std::cout << "Tried to send time sync to client ("
		          << tcpRemoteToStr(client) << "), but failed."
		          << std::endl;
	}

	std::cout << "Client (" << tcpRemoteToStr(client)
	          << ") time sync finished." << std::endl;

	// Close Socket, Delete Socket Resources + Close thread
	client->disconnect();
	return;
}

void timeSyncServer(tempo::Clock *clock) 
{
	sf::TcpListener listener;
	// Listen for a connection
	if (listener.listen(NET_PORT_TS) != sf::Socket::Done) {
		std::cout << "Listener socket could not open on port."
		          << NET_PORT_TS << std::endl;
	}

	std::vector<sf::TcpSocket*> clientSockets;
	std::vector<std::thread*> clientThreads;
	while (true) {
		// Make a new client
		sf::TcpSocket *client = new sf::TcpSocket;

		// Listen for the client, when one connects add to List
		// of clients and deal with time sync protocol
		if (listener.accept(*client) == sf::Socket::Done) {
			clientSockets.push_back(client);
			std::thread *t;
			t = new std::thread(timeSyncHandler, clock, client);
			clientThreads.push_back(t);
		}
		else {
			// Error, cleanup the client we created
			std::cout << "Listener reported something, but "
			          << "no client accepted." << std::endl;
			delete client;
		}

		// Clean up finished threads
		for (int i = 0; i < clientSockets.size(); i++) {
			if (ithTimeSyncCheck(clientSockets, clientThreads, i)) {
				clientSockets.erase(clientSockets.begin() + i);
				clientThreads.erase(clientThreads.begin() + i);
			}
		}
	}

	listener.close();
}

void listenForNewClients(unsigned short port)
{
	return;
}

void listenForClientUpdates(unsigned short port)
{
	return;
}

}
