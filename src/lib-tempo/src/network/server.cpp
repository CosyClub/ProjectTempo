#include <tempo/network/server.hpp>

#include <tempo/component/ComponentAOEIndicator.hpp>
#include <tempo/component/ComponentAttack.hpp>
#include <tempo/component/ComponentButtonGroup.hpp>
#include <tempo/component/ComponentCombo.hpp>
#include <tempo/component/ComponentAI.hpp>
#include <tempo/component/ComponentHealth.hpp>
#include <tempo/component/ComponentModel.hpp>
#include <tempo/component/ComponentParty.hpp>
#include <tempo/component/ComponentPlayerLocal.hpp>
#include <tempo/component/ComponentPlayerRemote.hpp>
#include <tempo/component/ComponentRespawn.hpp>
#include <tempo/component/ComponentStage.hpp>
#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/component/ComponentStageTranslation.hpp>
#include <tempo/component/ComponentSpikes.hpp>
#include <tempo/component/ComponentTeam.hpp>
#include <tempo/component/ComponentWeapon.hpp>

namespace tempo
{
// ithTimeSyncCheck - Checks if the i'th time sync thread has completed
// Arguments:
//         sockets - Vector of time sync sockets (implied synced index)
//         threads - Vector of time sync threads (implied synced index)
//         i       - The i`th element of the given vectors to be checked
// Returns:
//         Binary Value as to whether the i`th time sync thread has completed.
#define ithTimeSyncCheck(sockets, threads, i)                                                      \
	((sockets.at(i)->getRemoteAddress() == sf::IpAddress::None) && (threads.at(i)->joinable()))

// Create the global clients map from the header file. Used to store connected
// clients, with their ID pointing to an IP and Port to send updates to.
std::map<uint32_t, tempo::clientConnection> clients;
std::mutex                                  cmtx;

// Refer to documentation in tempo/network/base.hpp
bool allowUnknownIfHandshake = true;

// For use in a separate thread by server to deal with a client so the main time
// sync thread can continue listening for more clients whilst dealing with this.
// This function acts as peer B in our brutalised version of NTP (RFC5905)
// http://www.ietf.org/rfc/rfc5905.txt
void timeSyncHandler(tempo::Clock *clock, sf::TcpSocket *client)
{
	// Initialise time sync protocol variables
	sf::Int64 T1  = 0;  // PACKET: Previous packet time of departure
	sf::Int64 T2  = 0;  // PACKET: Previous packet time of arrival
	sf::Int64 T3  = 0;  // PACKET: Current pakcet time of departure
	sf::Int64 T4  = 0;  // PACKET: Current packet time of arrival
	sf::Int64 org = 0;  // STATE:  Time when message departed from peer
	sf::Int64 rec = 0;  // STATE:  Time when we received from the peer
	sf::Int64 xmt = 0;  // STATE:  Time when we transmitted to the peer

	for (int i = 0; i < TIMESYNC_ITERS; i++) {
		// Time Sync Exchange: t(n+0) -> t(n+1)
		sf::Packet packet;
		client->receive(packet);
		T4 = clock->get_time().asMicroseconds();  // t(n+1)
		packet >> T1 >> T2 >> T3;
		org = T3;  // t(n+0)
		rec = T4;  // t(n+1)

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
		std::this_thread::sleep_for(std::chrono::milliseconds(TIMESYNC_DELTA));

		// Time Sync Exchange: t(n+2) -> t(n+3)
		packet = sf::Packet();
		T1     = org;                                 // t(n+0)
		T2     = rec;                                 // t(n+1)
		T3     = clock->get_time().asMicroseconds();  // t(n+2)
		packet << T1 << T2 << T3;
		if (client->send(packet) != sf::Socket::Done) {
			std::cout << "Tried to send time sync to client (" << tcpRemoteToStr(client)
			          << "), but failed." << std::endl;
		}
		xmt = T3;  // t(n+2)
	}

	std::cout << "Client (" << tcpRemoteToStr(client) << ") completed time sync." << std::endl;

	// Close Socket, Delete Socket Resources + Close thread
	client->disconnect();
	return;
}

void timeSyncServer(tempo::Clock *clock)
{
	sf::TcpListener listener;
	// Listen for a connection
	if (listener.listen(port_st) != sf::Socket::Done) {
		std::cout << "Listener socket could not open on port." << port_st << std::endl;
		return;
	}

	std::cout << "Time Sync Listener Started..." << std::endl;

	std::vector<sf::TcpSocket *> clientSockets;
	std::vector<std::thread *>   clientThreads;
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
		for (unsigned int i = 0; i < clientSockets.size(); i++) {
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

uint32_t addClient(sf::Uint32 ip, unsigned short iport, unsigned short oport, ClientRole role = ClientRole::NO_ROLE)
{
	clientConnection newClient = {ip, iport, oport, role, anax::Entity::Id(), 0};
	cmtx.lock();
	clients.insert(std::make_pair(idCounter, newClient));
	cmtx.unlock();
	return idCounter++;
}

uint32_t findClientID(sf::Uint32 ip, unsigned short port)
{
	sf::Uint32 search = ip;
	if (search == sf::IpAddress("127.0.0.1").toInteger()) {
		search = sf::IpAddress::getLocalAddress().toInteger();
	}

	// Loop through clients
	cmtx.lock();
	for (clientpair element : clients) {
		if (element.second.ip == search && (element.second.iport == port || element.second.oport == port)) {
			cmtx.unlock();
			return element.first;
		}
	}
	cmtx.unlock();
	return NO_CLIENT_ID;
}

void removeClientId(sf::Uint32 ip, unsigned short port)
{
	// Loop through clients
	cmtx.lock();
	for (clientpair element : clients) {
		if (element.second.ip == ip && (element.second.iport == port || element.second.oport == port)) {
			clients.erase(element.first);
			cmtx.unlock();
			return;
		}
	}
	cmtx.unlock();
}

#define ADD_COMPONENT(ENT, CNT, PKT, CMP)                                                          \
	if (ENT.hasComponent<CMP>()) {                                                                 \
		sf::Packet part;                                                                           \
		part << ENT.getComponent<CMP>().getId();                                                   \
		sf::Packet part2 = ENT.getComponent<CMP>().dumpComponent();                                \
		part << part2;                                                                             \
		PKT << sf::Uint32(part.getDataSize());                                                     \
		PKT << part;                                                                               \
		CNT++;                                                                                     \
	}

sf::Packet packageComponents(anax::Entity entity)
{
	sf::Packet packet;
	packet << entity.getId();

	// Temporary measure for some form of bad error checking
	uint32_t c = 0;

	// Put new Components in here
	ADD_COMPONENT(entity, c, packet, ComponentAOEIndicator)
	ADD_COMPONENT(entity, c, packet, ComponentAttack)
	ADD_COMPONENT(entity, c, packet, ComponentButtonGroup)
	ADD_COMPONENT(entity, c, packet, ComponentCombo)
	ADD_COMPONENT(entity, c, packet, ComponentAI)
	ADD_COMPONENT(entity, c, packet, ComponentHealth)
	ADD_COMPONENT(entity, c, packet, ComponentModel)
	ADD_COMPONENT(entity, c, packet, ComponentParty)
	ADD_COMPONENT(entity, c, packet, ComponentPlayerLocal)
	ADD_COMPONENT(entity, c, packet, ComponentPlayerRemote)
	ADD_COMPONENT(entity, c, packet, ComponentRespawn)
	ADD_COMPONENT(entity, c, packet, ComponentStage)
	ADD_COMPONENT(entity, c, packet, ComponentStagePosition)
	ADD_COMPONENT(entity, c, packet, ComponentStageRotation)
	ADD_COMPONENT(entity, c, packet, ComponentStageTranslation)
	ADD_COMPONENT(entity, c, packet, ComponentSpikes)
	ADD_COMPONENT(entity, c, packet, ComponentTeam)
	ADD_COMPONENT(entity, c, packet, ComponentWeapon)

	if (c < entity.getComponentTypeList().count()) {
		std::cout << "WARNING: /Some/ components where not serialised "
		          << "when sending Entity ID: " << entity.getId() << std::endl
		          << "Did you forget add a component to "
		          << "the server without adding NetworkedComponent "
		          << "methods?" << std::endl;
		assert(false);
	}

	return packet;
}

void handshakeHello(sf::Packet &packet, anax::World *world)
{
	// Extract information from packet
	sf::Uint32     ip;
	unsigned short iPort = 0;
	unsigned short oPort = 0;
	packet >> ip;
	packet >> iPort;
	packet >> oPort;
	sf::IpAddress sender(ip);
	std::cout << "New client (" << sender.toString() << ":" << oPort << ") Connecting!"
	          << std::endl;

	// Register Client Internally
	uint32_t id = NO_CLIENT_ID;
	if (findClientID(ip, iPort) == NO_CLIENT_ID) {
		id = addClient(ip, iPort, oPort);
	} else {
		id = findClientID(ip, iPort);
		// TODO: Time out old client and make a new one
		std::cout << "WARNING: Connected client tried to reconnect (" << id << ", " << sender << ":"
		          << oPort << ")" << std::endl;
	}

	// Work out how many components there are in the world
	uint32_t                packetAmount = 0;
	std::vector<sf::Packet> componentPackets;
	for (auto &entity : world->getEntities()) {
		componentPackets.push_back(packageComponents(entity));
		packetAmount++;
	}

	// Construct HELLO_ROG response
	sf::Packet rog;
	rog << static_cast<uint32_t>(HandshakeID::HELLO_ROG);
	rog << id;  // TODO change to temporary token
	rog << port_si;
	rog << port_st;
	rog << packetAmount;

	// Send response back to sender
	sendMessage(QueueID::HANDSHAKE, rog, id);

	// Now send current state back to the original sender
	for (sf::Packet p : componentPackets) {
		sendMessage(QueueID::HANDSHAKE, p, id);
	}
}

void handshakeRoleReq(sf::Packet &packet, anax::World *world)
{
	// Extract data from packet
	uint32_t       id   = NO_CLIENT_ID;
	uint32_t       role = static_cast<uint32_t>(ClientRole::NO_ROLE);
	ClientRoleData roleData;
	packet >> id;  // TODO change to temporary tocken
	packet >> role;
	packet >> roleData;

	// Create Entity for selected role from client
	// Only creating players for now (spectators are not a thing)
	anax::Entity newEntity = newPlayer(*world, roleData.party_number);
	sf::Packet   newPlayer = packageComponents(newEntity);

	// Register Role
	cmtx.lock();
	clients[id].role = static_cast<ClientRole>(role);
	clients[id].id   = newEntity.getId();
	cmtx.unlock();

	// Construct ROLEREQ_ROG response
	sf::Packet rog;
	rog << static_cast<uint32_t>(HandshakeID::ROLEREQ_ROG);
	rog << 1;

	// Send response back to sender
	sendMessage(QueueID::HANDSHAKE, rog, id);
	sendMessage(QueueID::HANDSHAKE, newPlayer, id);

	// Tell everyone that we have a new player
	// TODO Clean this up post merge with new handshake
	for (clientpair client : clients) {
		if (client.first == id)
			continue;
		sendMessage(tempo::QueueID::ENTITY_CREATION, newPlayer, client.first);
	}
}

void checkForClientCreation(anax::World *world)
{
	tempo::Queue<sf::Packet> *queue = get_system_queue(QueueID::HANDSHAKE);
	if (queue->empty())
		return;

	while (!queue->empty()) {
		sf::Packet packet = queue->front();
		queue->pop();

		uint32_t receiveID = static_cast<uint32_t>(HandshakeID::DEFAULT);
		packet >> receiveID;

		switch (static_cast<HandshakeID>(receiveID)) {
		case HandshakeID::HELLO:   handshakeHello(packet, world); break;
		case HandshakeID::ROLEREQ: handshakeRoleReq(packet, world); break;
		default:
			std::cout << "WARNING: an invalid handshake message was "
			          << "received ... ignoring" << std::endl;
			break;
		}
	}
	world->refresh();
}

void checkForClientDeletion(anax::World &world)
{
	tempo::Queue<sf::Packet> *queue = get_system_queue(QueueID::ENTITY_DELETION);
	if (queue->empty())
		return;

	while (!queue->empty()) {
		sf::Packet packet = queue->front();
		queue->pop();

		sf::Packet       broadcast;
		anax::Entity::Id id;
		uint32_t         ip_d;
		unsigned short   port;
		bool             intentional;
		packet >> id >> ip_d >> port >> intentional;
		broadcast << id;
		sf::IpAddress ip(ip_d);

		if (intentional) {
			std::cout << "Client (" << ip.toString() << ":" << port << ") Disconnected." << std::endl;
		} else {
			std::cout << "Client (" << ip.toString() << ":" << port << ") Timed Out." << std::endl;
		}

		if (!id.isNull()) {
			anax::Entity e(world, id);
			auto& comp_party = e.getComponent<tempo::ComponentParty>();
			comp_party.freePartySlot();
			world.killEntity(e);
		}
		removeClientId(ip.toInteger(), port);

		broadcastMessage(tempo::QueueID::ENTITY_DELETION, broadcast);
	}
	world.refresh();
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

	sf::IpAddress  ip;
	unsigned short port;
	while (true) {
		sf::Packet packet;
		if (sock_i.receive(packet, ip, port) != sf::Socket::Done) {
			std::cout << "Error recieving client update. Ignoring." << std::endl;
			continue;
		}

		// Sort packet into respective system.
		bool o = sortPacket(packet, findClientID(ip.toInteger(), port));
		if (!o) std::cout << "Dropped invalid packet/message from unknown address ("
			          << ip.toString() << ":" << port << ")." << std::endl;
	}

	return;
}

////////////////////////////////////////////////////////////////////////////////

bool sendMessage(tempo::QueueID id, sf::Packet p)
{
	tempo::Queue<sf::Packet> *q = get_system_queue(id);
	q->push(p);
	return true;
}

bool broadcastMessage(tempo::QueueID id, sf::Packet p)
{
	bool result = true;
	for (auto client : clients) {
		result &= sendMessage(id, p, client.first);
	}

	return result;
}

bool sendMessage(tempo::QueueID id, sf::Packet payload, uint32_t client_id)
{
	sf::Packet message;

	// Construct message
	message << id;
	message << payload;

	return sock_o.send(message, sf::IpAddress(clients[client_id].ip), clients[client_id].iport)
	       == sf::Socket::Done;
}

}  // end of namespace
