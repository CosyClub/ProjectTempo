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
	if (listener.listen(port_st) != sf::Socket::Done) {
		std::cout << "Listener socket could not open on port."
		          << port_st << std::endl;
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
				delete clientSockets.at(i);
				clientSockets.erase(clientSockets.begin() + i);
				clientThreads.erase(clientThreads.begin() + i);
			}
		}
	}

	listener.close();
}

////////////////////////////////////////////////////////////////////////////////

// Internal, should only be used when registering new clients.
// Will not check if client already exists, just add the information as a new
// client, so it is recommended to use `if (!findClientID(ip, port)) first!
static uint32_t idCounter = NO_CLIENT_ID + 1; 
uint32_t addClient(sf::Uint32 ip, 
                   unsigned short port,
                   ClientRole role = ClientRole::NO_ROLE)
{
	clientConnection newClient = {ip, port, role};
	cmtx.lock();
	clients.insert(std::make_pair(idCounter, newClient));
	cmtx.unlock();
	return idCounter++;
}

void handshakeHello(sf::Packet &packet, sf::IpAddress &sender)
{
	// Extract information from packet
	unsigned short port = 0;
	packet >> port;
	sf::Uint32 ip = sender.toInteger();

	// Register Client Internally
	uint32_t id = NO_CLIENT_ID;
	if (findClientID(ip, port) == NO_CLIENT_ID) {
		id = addClient(ip, port);
	} else {
		// TODO: Time out old client and make a new one
		std::cout << "WARNING: Connected client tried to reconnect ("
		          << id << ", " << ip << ":" << port << ")\r\n";
	}
	
	// Construct HELLO_ROG response
	sf::Packet rog;
	rog << static_cast<uint32_t>(HandshakeID::HELLO_ROG);
	rog << id; // TODO change to temporary token
	rog << port_si;
	rog << port_st;
	// TODO Package entire level, eg:
	// rog << packageLevel()

	// Send response back to sender
	sock_h.send(rog, sender, port);
}

void handshakeRoleReq(sf::Packet &packet, sf::IpAddress &sender)
{
	// Extract data from packet
	uint32_t id = NO_CLIENT_ID;
	uint32_t role = static_cast<uint32_t>(ClientRole::NO_ROLE); 
	ClientRoleData roleData;
	packet >> id; // TODO change to temporary tocken	
	packet >> role;
	packet >> roleData; 

	// Create Entity for selected role from client
	// TODO ^The above

	// Register Role
	cmtx.lock();
	clients[id].role = static_cast<ClientRole>(role);
	unsigned short port = clients[id].port;
	cmtx.unlock();
	
	// Construct ROLEREQ_ROG response
	sf::Packet rog;
	rog << static_cast<uint32_t>(HandshakeID::ROLEREQ_ROG);
	// TODO Package Requested Entity, eg:
	// rog << packagePlayer(id);
	
	//Send response back to sender
	sock_h.send(rog, sender, port);
}

void processNewClientPacket(sf::Packet &packet, 
                            sf::IpAddress &sender,
                            unsigned short port)
{
	uint32_t receiveID = static_cast<uint32_t>(HandshakeID::DEFAULT);
	packet >> receiveID;

	switch (static_cast<HandshakeID>(receiveID)) {
	case HandshakeID::HELLO:
		handshakeHello(packet, sender);
		break;
	case HandshakeID::ROLEREQ:
		handshakeRoleReq(packet, sender);
		break;
	default:
		std::cout << "WARNING: an invalid handshake message was "
		          << "recieved from " << sender.toString() << ":" 
	                  << port << " ... ignoring" << std::endl;
		break;
	}
}

void listenForNewClients()
{
	// Bind to port
	if (!bindSocket('h', port_sh)) {
		std::cout << "Could not bind port %d, used to listen for "
		          << "new clients." << std::endl;
		return;
	}

	// Loop listening for new clients
	while (true) {
		sf::Packet packet;
		sf::IpAddress ip;
		unsigned short port;

		if (sock_h.receive(packet, ip, port) != sf::Socket::Done) {
			std::cout << "Error recieving something from new "
			          << "(connecting) client." << std::endl;
		}
	
		processNewClientPacket(packet, ip, port);
	}

	// TODO Probably should close the socket but it's a protected function
	// so I can't.
}

void listenForClientUpdates()
{
	// Bind to port
	if (!bindSocket('i', port_si)) {
		std::cout << "Could not bind port %d, used to listen for "
		          << "client updates." << std::endl;
		return;
	}
	
	// TODO Implement me!
	
	return;
}

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



} // end of namespace
