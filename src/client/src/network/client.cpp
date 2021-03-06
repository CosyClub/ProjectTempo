#include <client/network/client.hpp>

#include <tempo/component/ComponentPlayerLocal.hpp>
#include <tempo/component/ComponentPlayerRemote.hpp>

#include <tempo/entity/EntityCreation.hpp>

namespace tempo
{

// Refer to documentation in tempo/network/base.hpp
bool allowUnknownIfHandshake = false;

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
	sf::Int64 T3  = 0;  // PACKET: Current packet time of departure
	sf::Int64 T4  = 0;  // PACKET: Current packet time of arrival
	sf::Int64 org = 0;  // STATE:  Time when message departed from peer
	// sf::Int64 rec    = 0; // STATE:  Time when we received from the peer
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

std::pair<bool, sf::Packet> receiveMessage(QueueID qid)
{
	tempo::Queue<sf::Packet> *queue = get_system_queue(qid);
	uint32_t count = 0;

	// 250 * 20ms = 5 seconds
	while (queue->empty() && count++ < 250) {
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	if (!queue->empty()) {
		sf::Packet packet = queue->front();
		queue->pop();
		return std::pair<bool, sf::Packet>(true, packet);
	} else {
		return std::pair<bool, sf::Packet>(false, sf::Packet());
	}
}

bool check_sockets()
{
	// Bind outgoing port if not bound
	if (!bindSocket('o', port_co)) {
		std::cout << "Could not bind socket on port " << port_co
		          << " to connect to and sync with server." << std::endl;
		return false;
	}

	// Ensure server update lisener has started - i.e. it's socket is open
	if (sock_i.getLocalPort() == 0) {
		std::cout << "Looks like the listener thread hasn't started, "
		          << "or didn't bind it's socket correctly. Will not "
		          << "connect to server without this or we won't hear "
		          << "anything from the server!" << std::endl;
		return false;
	}

	return true;
}

uint32_t handshakeHello(anax::World &world)
{
	// Package up payload
	sf::Packet send, receive;
	send << static_cast<uint32_t>(HandshakeID::HELLO);
	send << sf::IpAddress::getLocalAddress().toInteger();
	send << port_ci;
	send << port_co;

	std::cout << "Your address is: "
	          << sf::IpAddress::getLocalAddress().toString() << ":"
	          << tempo::port_co << std::endl;

	bool     b     = false;
	uint32_t count = 0;
	while (!b && count++ < 12) {
		// Send HELLO
		std::cout << "Attempting to connect to the server..." << std::endl;
		sendMessage(QueueID::HANDSHAKE, send);

		// Receive HELLO_ROG
		std::pair<bool, sf::Packet> o = receiveMessage(QueueID::HANDSHAKE);
		b       = o.first;
		receive = o.second;
	}
	if (!b) {
		std::cout << "Failed to connect to server after 1 minute." << std::endl;
		return NO_CLIENT_ID;
	}

	// Extract Data
	uint32_t msg            = static_cast<uint32_t>(HandshakeID::DEFAULT);
	uint32_t id             = NO_CLIENT_ID;
	uint32_t componentCount = 0;
	receive >> msg;
	if (msg == static_cast<uint32_t>(HandshakeID::HELLO_ROG)) {
		receive >> id;
		receive >> port_si;
		receive >> port_st;
		receive >> componentCount;

		for (unsigned int i = 0; i < componentCount; i++) {
			bool b = false;
			while (!b) {
				std::pair<bool, sf::Packet> o = receiveMessage(QueueID::HANDSHAKE);
				b       = o.first;
				receive = o.second;
			}
			std::cout << "Recieved " << i << "/" << componentCount << std::endl;
			addComponent(world, receive);
		}
		std::cout << "Synced with server!" << std::endl;
	} else {
		std::cout << "Failed to sync with server." << std::endl;
	}

	return id;
}

bool handshakeRoleReq(uint32_t id, ClientRole roleID, ClientRoleData &roleData, anax::World &world)
{
	// Package up payload
	sf::Packet send, receive;
	send << static_cast<uint32_t>(HandshakeID::ROLEREQ);
	send << id;
	send << roleID;
	send << roleData;

	bool b = false;
	uint32_t count = 0;
	while (!b && count++ < 12) {
		// Send ROLEREQ
		std::cout << "Joining game..." << std::endl;
		sendMessage(QueueID::HANDSHAKE, send);

		// Recieve ROLEREQ_ROG
		std::pair<bool, sf::Packet> o = receiveMessage(QueueID::HANDSHAKE);
		b       = o.first;
		receive = o.second;
	}
	if (!b) {
		std::cout << "Failed to join game after 1 minute." << std::endl;
		return false;
	}

	// Extract Data
	uint32_t msg = static_cast<uint32_t>(HandshakeID::DEFAULT);
	int      componentCount;
	receive >> msg;
	receive >> componentCount;
	if (msg == static_cast<uint32_t>(HandshakeID::ROLEREQ_ROG)) {
		// TODO Extract entity/response from ROLEREQ_ROG
		anax::Entity en;
		for (int i = 0; i < componentCount; i++) {
			bool b = false;
			while (!b) {
				std::pair<bool, sf::Packet> o = receiveMessage(QueueID::HANDSHAKE);
				b       = o.first;
				receive = o.second;
			}
			en = addComponent(world, receive);
		}
		en.removeComponent<tempo::ComponentPlayerRemote>();
		en.addComponent<tempo::ComponentPlayerLocal>();
	} else {
		std::cout << "Failed to join game." << std::endl;
		return false;
	}

	world.refresh();
	std::cout << "Joined game!" << std::endl;
	return true;
}

////////////////////////////////////////////////////////////////////////////////
// Server Listener (To be run on a separate thread)
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
	sf::IpAddress  local = sf::IpAddress::getLocalAddress();
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
			bool o = sortPacket(packet, (ip == tempo::addr_r) || (ip == local));
			if (!o) std::cout << "Dropped invalid packet/message from unknown address ("

				          << ip.toString() << ":" << port << ")." << std::endl;
			packet = sf::Packet();
		}
	}

	std::cout << "Server Update Listener Closed." << std::endl;

	return;
}


////////////////////////////////////////////////////////////////////////////////
// Functions for connnection, syncing and disconnection

static bool     _synced      = false;
static uint32_t _handshakeid = NO_CLIENT_ID;

bool connectToAndSyncWithServer(anax::World &world)
{
	if (check_sockets()) {
		_handshakeid = handshakeHello(world);
		_synced      = _handshakeid != NO_CLIENT_ID;
	}
	return _synced;
}

bool joinGame(ClientRole roleID, ClientRoleData &roleData, anax::World &world)
{
	if (check_sockets() && _synced) {
		return handshakeRoleReq(_handshakeid, roleID, roleData, world);
	}
	return false;
}

void disconnectFromServer(anax::Entity &entity_player)
{
	sf::Packet p;
	anax::Entity::Id id = entity_player.getId();
	LOCALTOSERVER(id);
	if (id.isNull()) {
		printf("Wat!?!? When trying to disconnect, we couldn't find the ID of ourself!\n");
	} else {
		tempo::operator<<(p, id);
		p << sf::IpAddress::getLocalAddress().toInteger();
		p << tempo::port_ci;
		p << true;
		tempo::sendMessage(tempo::QueueID::ENTITY_DELETION, p);
	}
}

////////////////////////////////////////////////////////////////////////////////
// Functions for message sending

void sendHeatbeat() {
	sf::Packet p;
	p << sf::IpAddress::getLocalAddress().toInteger();
	p << tempo::port_ci;
	tempo::sendMessage(tempo::QueueID::HEARTBEAT, p);

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

bool broadcastMessage(QueueID id, sf::Packet p)
{
	return false;
}

}  // namespace tempo
