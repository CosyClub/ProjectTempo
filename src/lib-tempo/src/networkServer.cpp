#include <tempo/config.hpp>
#include <tempo/time.hpp>
#include <tempo/networkServer.hpp>

#include <iostream>


namespace tempo
{

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

// Create the global clients map from the header file. Used to store connected
// clients, with their ID pointing to an IP and Port to send updates to.
std::map<uint32_t, tempo::clientConnection> clients;
std::mutex cmtx;

// For use in a separate thread by server to deal with a client so the main time 
// sync thread can continue listening for more clients whilst dealing with this.
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
		return;
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
		} else {
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

////////////////////////////////////////////////////////////////////////////////

uint32_t findClientID(sf::Uint32 ip, unsigned short port)
{
	// Loop through clients
	cmtx.lock();
	for (clientpair element : clients) {
		if (element.second.ip == ip && element.second.port == port) {
			cmtx.unlock();
			return element.first;
		}
	}
	cmtx.unlock();
	return NO_CLIENT_ID;
}

static uint32_t idCounter = NO_CLIENT_ID + 1; 
uint32_t addClient(sf::Uint32 ip, unsigned short port)
{
	clientConnection newClient = {ip, port};
	cmtx.lock();
	clients.insert(std::make_pair(idCounter, newClient));
	cmtx.unlock();
	return idCounter++;
}

void processNewClientPacket(sf::Packet &packet, 
                            sf::IpAddress &sender,
                            unsigned short port)
{
	int receiveID = static_cast<int>(HandshakeID::DEFAULT);
	packet >> receiveID;

	switch (static_cast<HandshakeID>(receiveID)) {
	case HandshakeID::HELLO: {
		// Register Client Internally
		sf::Uint32 clientIP = sender.toInteger();
		unsigned short clientPort = 0;
		packet >> clientPort;

		if (findClientID(clientIP, clientPort) == NO_CLIENT_ID) {
			addClient(clientIP, clientPort);
		}

		// HELLO_ROG
		// Send the entire level
		break; }
	case HandshakeID::ROLEREQ: {
		// Create Entity for selected role from client
		
		// Register Role
		
		// Send back Entity to the client
		
		break; }
	default:
		// error, this is not a message we expect
		break;
	}
}

void listenForNewClients(unsigned short port)
{
	// Bind to port
	sf::UdpSocket socket;
	if (socket.bind(port) != sf::Socket::Done) {
		std::cout << "Could not bind port %d, used or listening for new"
		          << " clients." << std::endl;
		return;
	}

	// Loop listening for new clients
	while (true) {
		sf::Packet packet;
		sf::IpAddress ip;
		unsigned short port;

		if (socket.receive(packet, ip, port) != sf::Socket::Done) {
			std::cout << "Error recieving something from new "
			          << "(connecting) client." << std::endl;
		}
	
		processNewClientPacket(packet, ip, port);
	}

	// TODO Probably should close the socket but it's a protected function
	// so I can't.
}

void listenForClientUpdates(unsigned short port)
{
	return;
}

} // end of namespace
