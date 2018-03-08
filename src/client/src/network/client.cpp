#include <client/network/client.hpp>

#include <tempo/component/ComponentPlayerLocal.hpp>
#include <tempo/component/ComponentPlayerRemote.hpp>

#include <tempo/entity/EntityCreation.hpp>

namespace tempo
{
// Brutalised version of the NTP Time Sync Protocol (RFC5905)
// http://www.ietf.org/rfc/rfc5905.txt (Page 27)
sf::Int64 timeSyncClient(tempo::Clock *clock)
{
	// Create a socket to the server and connect to it
	sf::TcpSocket socket;
	if (socket.connect(addr_r, port_st) != sf::Socket::Done) {
		std::cout << "Error binding socket" << std::endl;
		return 0;
	}

	// Initialise time sync protocol variables
	sf::Int64 T1  = 0;  // PACKET: Previous packet time of departure
	sf::Int64 T2  = 0;  // PACKET: Previous packet time of arrival
	sf::Int64 T3  = 0;  // PACKET: Current pakcet time of departure
	sf::Int64 T4  = 0;  // PACKET: Current packet time of arrival
	sf::Int64 org = 0;  // STATE:  Time when message departed from peer
	// sf::Int64 rec    = 0; // STATE:  Time when we recieved from the peer
	sf::Int64 xmt    = 0;  // STATE:  Time when we transmitted to the peer
	sf::Int64 offset = 0;  // Final Result

	for (int i = 0; i < TIMESYNC_ITERS; i++) {
		// Time Sync Exchange: t(n+0) -> t(n+1)
		T1 = T3;
		T2 = T4;
		T3 = clock->get_time().asMicroseconds();  // t(n+0)
		sf::Packet packet;
		packet << T1 << T2 << T3;
		if (socket.send(packet) != sf::Socket::Done) {
			std::cout << "Error sending T/S packet" << std::endl;
			return 0;
		}
		xmt = T3;  // t(n+0)

		// Time Sync Exchange: t(n+2) -> t(n+3)
		if (socket.receive(packet) != sf::Socket::Done) {
			std::cout << "Error recieving T/S packet" << std::endl;
			return 0;
		}
		T4 = clock->get_time().asMicroseconds();  // t(n+3)
		packet >> T1 >> T2 >> T3;

		// Sanity Checks
		if ((T1 == org) || (T3 == xmt) || (T1 != xmt)) {
			// Something went wrong; duplicate/bogus packet.
			return 0;
		}
		org = T3;
		// rec = T4;

		// Calculate offset
		offset += ((T2 - T1) + (T3 - T4)) / 2;
	}

	// Return Time Sync Offset
	return offset / TIMESYNC_ITERS;
}

bool sendMessage(tempo::QueueID id, sf::Packet payload)
{
	sf::Packet message;

	// Construct message
	message << id;
	message << payload;

	// Send message
	return sock_o.send(message, addr_r, port_si) == sf::Socket::Done;
}

sf::Packet receiveMessage(QueueID qid)
{
	tempo::Queue<sf::Packet> *queue = get_system_queue(qid);
	while (queue->empty())
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	sf::Packet packet = queue->front();
	queue->pop();
	return packet;
}

void listenForServerUpdates(std::atomic<bool> &running)
{
	// Bind to port
	if (!bindSocket('i', port_ci)) {
		std::cout << "Could not bind port " << port_ci << ", used to "
		          << "listen for server updates." << std::endl;
		return;
	}

	std::cout << "Server Update Listener Started..." << std::endl;

	sock_i.setBlocking(false);
	sf::IpAddress  ip;
	unsigned short port;
	sf::Packet     packet;

	while (running.load()) {
		sf::Socket::Status status = sock_i.receive(packet, ip, port);
		if (status == sf::Socket::Error || status == sf::Socket::Disconnected) {
			std::cout << "Error recieving server update. Ignoring." << std::endl;
			continue;
		}

		// Sort packet into respective system.
		if (status == sf::Socket::Done) {
			sortPacket(packet);
			packet = sf::Packet();
		}
	}

	std::cout << "Server Update Listener Closed." << std::endl;

	return;
}

uint32_t handshakeHello(anax::World &world)
{
	// Package up payload
	sf::Packet packet;
	packet << static_cast<uint32_t>(HandshakeID::HELLO);
	packet << sf::IpAddress::getLocalAddress().toInteger();
	packet << port_ci;

	// Send HELLO
	sendMessage(QueueID::HANDSHAKE, packet);

	// Receive HELLO_ROG
	packet = receiveMessage(QueueID::HANDSHAKE);

	// Extract Data
	uint32_t msg            = static_cast<uint32_t>(HandshakeID::DEFAULT);
	uint32_t id             = NO_CLIENT_ID;
	uint32_t componentCount = 0;
	packet >> msg;
	if (msg == static_cast<uint32_t>(HandshakeID::HELLO_ROG)) {
		packet >> id;
		packet >> port_si;
		packet >> port_st;
		packet >> componentCount;

		sf::Packet p2;
		for (unsigned int i = 0; i < componentCount; i++) {
			p2 = receiveMessage(QueueID::HANDSHAKE);
			std::cout << "Recieved " << i << "/" << componentCount << std::endl;
			addComponent(world, p2);
		}
	} else {
		std::cout << "We didn't get what we expected when connecting "
		          << "to the server." << std::endl;
	}

	return id;
}

bool handshakeRoleReq(uint32_t id, ClientRole roleID, ClientRoleData &roleData, anax::World &world)
{
	// Package up payload
	sf::Packet packet;
	packet << static_cast<uint32_t>(HandshakeID::ROLEREQ);
	packet << id;
	packet << roleID;
	packet << roleData;

	// Send ROLEREQ
	sendMessage(QueueID::HANDSHAKE, packet);

	// Wait until we recieve ROLEREQ_ROG
	packet = receiveMessage(QueueID::HANDSHAKE);

	// Extract Data
	uint32_t msg = static_cast<uint32_t>(HandshakeID::DEFAULT);
	int      componentCount;
	packet >> msg;
	packet >> componentCount;
	if (msg == static_cast<uint32_t>(HandshakeID::ROLEREQ_ROG)) {
		// TODO Extract entity/response from ROLEREQ_ROG
		sf::Packet   p2;
		anax::Entity en;
		for (int i = 0; i < componentCount; i++) {
			p2 = receiveMessage(QueueID::HANDSHAKE);
			en = addComponent(world, p2);
		}
		en.removeComponent<tempo::ComponentPlayerRemote>();
		en.addComponent<tempo::ComponentPlayerLocal>();
	} else {
		std::cout << "The server was rude to us when we requested a "
		          << "role. >:(" << std::endl;
		return false;
	}

	return true;
}

bool connectToAndSyncWithServer(ClientRole roleID, ClientRoleData &roleData, anax::World &world)
{
	// Bind outgoing port if not bound
	if (sock_o.getLocalPort() == 0) {
		if (!bindSocket('o', port_co)) {
			std::cout << "Could not bind socket on port " << port_co
			          << " to connect to and sync with server." << std::endl;
			return false;
		}
	}

	// Ensure server update lisener has started - i.e. it's socket is open
	if (sock_i.getLocalPort() == 0) {
		std::cout << "Looks like the listener thread hasn't started, "
		          << "or didn't bind it's socket correctly. Will not "
		          << "connect to server without this or we won't hear "
		          << "anything from the server!" << std::endl;
		return false;
	}

	std::cout << "HELLO STARTING" << std::endl;
	uint32_t id = handshakeHello(world);
	std::cout << "HELLO COMPLETE" << std::endl;
	if (id == NO_CLIENT_ID) {
		std::cout << "Couldn't connect to the server." << std::endl;
		return false;
	}

	std::cout << "ROLEREQ STARTING" << std::endl;
	bool ret = handshakeRoleReq(id, roleID, roleData, world);
	std::cout << "ROLEREQ COMPLETE" << std::endl;
	world.refresh();
	return ret;
}

void disconnectFromServer(anax::Entity::Id id)
{
	sf::Packet p;
}


bool broadcastMessage(QueueID id, sf::Packet p)
{
	assert(false);  // I think this should nevevr happen
	return false;
}

}  // namespace tempo
