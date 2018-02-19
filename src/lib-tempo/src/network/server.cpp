#include <tempo/network/server.hpp>

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
// This function acts as peer B in our brutalised version of NTP (RFC5905)
// http://www.ietf.org/rfc/rfc5905.txt
void timeSyncHandler(tempo::Clock *clock, sf::TcpSocket *client) 
{
	// Initialise time sync protocol variables
	sf::Int64 T1     = 0; // PACKET: Previous packet time of departure
	sf::Int64 T2     = 0; // PACKET: Previous packet time of arrival
	sf::Int64 T3     = 0; // PACKET: Current pakcet time of departure
	sf::Int64 T4     = 0; // PACKET: Current packet time of arrival
	sf::Int64 org    = 0; // STATE:  Time when message departed from peer
	sf::Int64 rec    = 0; // STATE:  Time when we recieved from the peer
	sf::Int64 xmt    = 0; // STATE:  Time when we transmitted to the peer
	sf::Int64 offset = 0; // Final Result
	
	for (int i = 0; i < TIMESYNC_ITERS; i++) {	
		// Time Sync Exchange: t(n+0) -> t(n+1)
		sf::Packet packet;
		client->receive(packet);
		T4  = clock->get_time().asMicroseconds(); // t(n+1)
		packet >> T1 >> T2 >> T3;
		org = T3; // t(n+0)
		rec = T4; // t(n+1)
	
		// Sanity Checks
		if ((T1 == org) || (T3 == xmt) || (T1 != xmt)) {
			// Something went wrong; duplicate/bogus packet.
			packet = sf::Packet();
			packet << 0 << 0 << 0;
			client->send(packet);
			client->disconnect();
			return;
		}

		// Wait a bit
		std::this_thread::sleep_for(
				std::chrono::milliseconds(TIMESYNC_DELTA));
	
		// Time Sync Exchange: t(n+2) -> t(n+3)
		packet = sf::Packet();
		T1 = org; // t(n+0)
		T2 = rec; // t(n+1)
		T3 = clock->get_time().asMicroseconds(); // t(n+2)
		packet << T1 << T2 << T3;
		if (client->send(packet) != sf::Socket::Done) {
			std::cout << "Tried to send time sync to client ("
			          << tcpRemoteToStr(client) << "), but failed."
			          << std::endl;
		}
		xmt = T3; // t(n+2)
	
	}

	std::cout << "Client (" << tcpRemoteToStr(client)
	          << ") completed time sync." << std::endl;

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
	
	std::cout << "Time Sync Listener Started..." << std::endl;

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

typedef std::vector<std::pair<anax::Entity, anax::Component*>> ec_list;
uint32_t countComponents(anax::Entity entity, ec_list &list)
{
	// TODO Clean this up post merge with new handshake
	uint32_t result = 0;
	for (auto& component: entity.getComponents()) {
		NetworkedComponent *nc;
		nc = dynamic_cast<NetworkedComponent*>(component);
		if (nc != NULL) {
			result++;
			list.push_back(std::make_pair(entity, component));
		}
	}
	return result;

}
	
void sendComponents(uint32_t clientID, ec_list &list)
{
	sf::Packet rog;
	for (auto& pair: list) {
		NetworkedComponent *nc;
		nc = dynamic_cast<NetworkedComponent*>(pair.second);
		rog << pair.first.getId();
		rog << nc->ID;
		rog << nc->dumpComponent();
		sendMessage(QueueID::HANDSHAKE, rog, clientID);
	}
}

void handshakeHello(sf::Packet &packet,
                    anax::World *world)
{
	// Extract information from packet
	sf::Uint32 ip;
	unsigned short updatePort = 0;
	packet >> ip;
	packet >> updatePort;
	sf::IpAddress sender(ip);
	std::cout << "New client (" << sender.toString() << ":" << updatePort 
	          << ") Connecting!" << std::endl;

	// Register Client Internally
	uint32_t id = NO_CLIENT_ID;
	if (findClientID(ip, updatePort) == NO_CLIENT_ID) {
		id = addClient(ip, updatePort);
	} else {
		id = findClientID(ip, updatePort);
		// TODO: Time out old client and make a new one
		std::cout << "WARNING: Connected client tried to reconnect ("
		          << id << ", " << sender << ":" << updatePort << ")" 
		          << std::endl;
	}
	
	//Work out how many components there are in the world
	uint32_t componentAmount = 0;
	ec_list components;
	for (auto& entity: world->getEntities()) 
		componentAmount += countComponents(entity, components);
	
	// Construct HELLO_ROG response
	sf::Packet rog;
	rog << static_cast<uint32_t>(HandshakeID::HELLO_ROG);
	rog << id; // TODO change to temporary token
	rog << port_si;
	rog << port_st;
	rog << componentAmount;

	// Send response back to sender
	sendMessage(QueueID::HANDSHAKE, rog, id);

	// Now send current state back to the original sender
	sendComponents(id, components);
}

void handshakeRoleReq(sf::Packet &packet,
                      anax::World *world,
                      SystemLevelManager system_gm)
{
	// Extract data from packet
	uint32_t id = NO_CLIENT_ID;
	uint32_t role = static_cast<uint32_t>(ClientRole::NO_ROLE); 
	ClientRoleData roleData;
	packet >> id; // TODO change to temporary tocken	
	packet >> role;
	packet >> roleData; 

	// Create Entity for selected role from client
	// Only creating players for now (spectators are not a thing)	
	anax::Entity entity = newPlayer(*world, system_gm);

	// Register Role
	cmtx.lock();
	clients[id].role    = static_cast<ClientRole>(role);
	sf::Uint32 ip       = clients[id].ip;
	unsigned short port = clients[id].port;
	cmtx.unlock();
	
	// Package Requested Entity
	ec_list components;
	uint32_t componentAmount = countComponents(entity, components);

	// Construct ROLEREQ_ROG response
	sf::Packet rog;
	rog << static_cast<uint32_t>(HandshakeID::ROLEREQ_ROG);
	rog << componentAmount;

	// Send response back to sender
	sendMessage(QueueID::HANDSHAKE, rog, id);
	sendComponents(id, components);

	// Tell everyone that we have a new player
	// TODO Clean this up post merge with new handshake
	for (clientpair client : clients) {
		if (client.first == id) continue;
		rog = sf::Packet();
		for (auto& pair : components) {
			NetworkedComponent *nc;
			nc = dynamic_cast<NetworkedComponent*>(pair.second);
			rog << pair.first.getId();
			rog << nc->ID;
			rog << nc->dumpComponent();
			sendMessage(tempo::QueueID::ENTITY_CREATION, 
			            rog,
			            client.first);
		}
	}
}

void checkForNewClients(anax::World *world, SystemLevelManager system_gm)
{
	tempo::Queue<sf::Packet> *queue = get_system_queue(QueueID::HANDSHAKE);	
	if (queue->empty()) return;

	while (!queue->empty()) {
		sf::Packet packet = queue->front();
		queue->pop();

		uint32_t receiveID = static_cast<uint32_t>(HandshakeID::DEFAULT);
		packet >> receiveID;

		switch (static_cast<HandshakeID>(receiveID)) {
		case HandshakeID::HELLO:
			handshakeHello(packet, world);
			break;
		case HandshakeID::ROLEREQ:
			handshakeRoleReq(packet, world, system_gm);
			break;
		default:
			std::cout << "WARNING: an invalid handshake message was "
			          << "recieved ... ignoring" << std::endl;
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

void listenForClientUpdates()
{
	// Bind to port
	if (!bindSocket('i', port_si)) {
		std::cout << "Could not bind port " << port_si << ", used to "
		          << "listen for client updates." << std::endl;
		return;
	}
	
	std::cout << "Client Update Listener Started..." << std::endl;

	sf::IpAddress ip;
	unsigned short port;
	while (true) {
		sf::Packet packet;
		if (sock_i.receive(packet, ip, port) != sf::Socket::Done) {
			std::cout << "Error recieving client update. Ignoring."
			          << std::endl;
			continue;
		}

		// Sort packet into respective system.
		sortPacket(packet);
	}
	
	return;
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


bool sendMessage(tempo::QueueID id, 
                 sf::Packet payload, 
                 uint32_t client_id)
{
	sf::Packet message;

	// Construct message
	message << id;
	message << payload;

	return sock_o.send(message, sf::IpAddress(clients[client_id].ip), clients[client_id].port) == sf::Socket::Done;
}

} // end of namespace
