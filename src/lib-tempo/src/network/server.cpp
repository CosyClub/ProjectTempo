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

typedef std::pair<anax::Entity, std::vector<anax::Component*>> c_list;
typedef std::vector<c_list> ec_list;
uint32_t countComponents(anax::Entity entity, ec_list &e_list)
{
	// TODO Clean this up post merge with new handshake
	c_list list;
	uint32_t result = 0;
	list.first = entity;
	for (auto& component: entity.getComponents()) {
		NetworkedComponent *nc;
		nc = dynamic_cast<NetworkedComponent*>(component);
		if (nc != NULL) {
			result++;
			list.second.push_back(component);
		}
	}
	e_list.push_back(list);
	return result;

}

sf::Packet makeBigPacket(c_list list)
{
	anax::Entity e = list.first;
	sf::Packet p = sf::Packet();
	p << e.getId();
	std::cout << "Sending components for entity " << e.getId().index << std::endl;
	for (anax::Component* c : list.second)
	{
		sf::Packet part = sf::Packet();
		NetworkedComponent *nc;
		nc = dynamic_cast<NetworkedComponent*>(c);
		std::cout << "Adding component with ID " << nc->getID() << std::endl;
		part << nc->getID();
		part << nc->dumpComponent();
		p << sf::Uint64(part.getDataSize());
		p << part;
	}

	return p;
}
	
void sendComponents(sf::IpAddress ip, unsigned short port, ec_list &e_list)
{
	sf::Packet rog;
	for (c_list list: e_list) {
		rog = makeBigPacket(list);
		sock_h.send(rog, ip, port);
	}
}

void handshakeHello(sf::Packet &packet,
                    sf::IpAddress &sender,
                    unsigned short port,
                    anax::World *world)
{
	// Extract information from packet
	unsigned short updatePort = 0;
	packet >> updatePort;
	sf::Uint32 ip = sender.toInteger();

	// Register Client Internally
	uint32_t id = NO_CLIENT_ID;
	if (findClientID(ip, updatePort) == NO_CLIENT_ID) {
		id = addClient(ip, updatePort);
	} else {
		id = findClientID(ip, updatePort);
		// TODO: Time out old client and make a new one
		std::cout << "WARNING: Connected client tried to reconnect ("
		          << id << ", " << ip << ":" << updatePort << ")" 
		          << std::endl;
	}
	
	//Work out how many components there are in the world
	uint32_t componentAmount = 0;
	ec_list components;
	for (auto& entity: world->getEntities()) 
		componentAmount += countComponents(entity, components);

	std::cout << "Found " << componentAmount << " components from "
	          << world->getEntities().size() << " entities" << std::endl;
	
	// Construct HELLO_ROG response
	sf::Packet rog;
	rog << static_cast<uint32_t>(HandshakeID::HELLO_ROG);
	rog << id; // TODO change to temporary token
	rog << port_si;
	rog << port_st;
	rog << componentAmount;

	// Send response back to sender
	sock_h.send(rog, sender, port);

	// Now send current state back to the original sender
	sendComponents(sender, port, components);
	
}

void handshakeRoleReq(sf::Packet &packet,
                      sf::IpAddress &sender, 
                      unsigned short port,
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
	clients[id].role = static_cast<ClientRole>(role);
	cmtx.unlock();
	
	// Package Requested Entity
	ec_list components;
	uint32_t componentAmount = countComponents(entity, components);

	// Construct ROLEREQ_ROG response
	sf::Packet rog;
	rog << static_cast<uint32_t>(HandshakeID::ROLEREQ_ROG);
	rog << componentAmount;

	// Send response back to sender
	sock_h.send(rog, sender, port);
	sendComponents(sender, port, components);

	// Tell everyone that we have a new player
	// TODO Clean this up post merge with new handshake
	for (clientpair client : clients) {
		if (client.first == id) continue;
		rog = sf::Packet();
		for (c_list list : components) {
			// NetworkedComponent *nc;
			// nc = dynamic_cast<NetworkedComponent*>(pair.second);
			// rog << pair.first.getId();
			// rog << nc->getID();
			// rog << nc->dumpComponent();
			rog = makeBigPacket(list);
			sendMessage(tempo::SystemQID::ENTITY_CREATION, 
			            rog,
			            client.first);
		}
	}
}

void processNewClientPacket(sf::Packet &packet, 
                            sf::IpAddress &sender,
                            unsigned short port,
                            anax::World *world,
                            SystemLevelManager system_gm)
{
	uint32_t receiveID = static_cast<uint32_t>(HandshakeID::DEFAULT);
	packet >> receiveID;

	switch (static_cast<HandshakeID>(receiveID)) {
	case HandshakeID::HELLO:
		std::cout << "New client (" << sender.toString() << ":" << port
		          << ") Connecting!" << std::endl; 
		handshakeHello(packet, sender, port, world);
		break;
	case HandshakeID::ROLEREQ:
		handshakeRoleReq(packet, sender, port, world, system_gm);
		break;
	default:
		std::cout << "WARNING: an invalid handshake message was "
		          << "recieved from " << sender.toString() << ":" 
	                  << port << " ... ignoring" << std::endl;
		break;
	}
}

void listenForNewClients(anax::World *world, SystemLevelManager system_gm)
{
	// Bind to port
	if (!bindSocket('h', port_sh)) {
		std::cout << "Could not bind port " << port_sh << ", used to "
		          << "listen for new clients." << std::endl;
		return;
	}

	std::cout << "New Client Listener Started..." << std::endl;

	// Loop listening for new clients
	while (true) {
		sf::Packet packet;
		sf::IpAddress ip;
		unsigned short port;

		if (sock_h.receive(packet, ip, port) != sf::Socket::Done) {
			std::cout << "Error recieving something from new "
			          << "(connecting) client. Ignoring." 
			          << std::endl;
			continue;
		}
	
		processNewClientPacket(packet, ip, port, world, system_gm);
	}

	// TODO Probably should close the socket but it's a protected function
	// so I can't.
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


bool sendMessage(tempo::SystemQID id, 
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
