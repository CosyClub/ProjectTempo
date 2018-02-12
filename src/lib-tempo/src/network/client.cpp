#include <tempo/network/client.hpp>

namespace tempo
{

sf::Time timeSyncClient(tempo::Clock *clock)
{
	// Create a socket to the server and connect to it
	sf::TcpSocket socket;
	sf::Socket::Status status = socket.connect(addr_r, port_st);
	if (status != sf::Socket::Done) {
		std::cout << "Error binding socket" << std::endl;
		return sf::Time::Zero;
	}

	// Initialise time sync protocol variables
	sf::Int64 t0 = clock->get_time().asMicroseconds();
	sf::Int64 t1;
	sf::Int64 t2;
	sf::Int64 t3;

	// Start time sync exchange
	sf::Packet packet;
	packet << t0;
	status = socket.send(packet);
	if (status != sf::Socket::Done) {
		std::cout << "Error sending packet" << std::endl;
		return sf::Time::Zero;
	}

	status = socket.receive(packet);
	if (status != sf::Socket::Done) {
		std::cout << "Error recieving packet" << std::endl;
		return sf::Time::Zero;
	}

	// End time sync exchange, calculate delay in last tranmission from 
	// time sync server.
	t3 = clock->get_time().asMicroseconds();
	packet >> t1 >> t2;
	sf::Int64 delay  = ((t3 - t0) - (t2 - t1)) / 2;

	// Return current time
	return sf::microseconds(t2 + delay);
}

bool sendMessage(tempo::SystemQID id, sf::Packet payload) 
{
	sf::Packet message;

	// Construct message
	message << id;
	message << payload;

	// Send message
	return sock_o.send(message, addr_r, port_si) == sf::Socket::Done;
}


void listenForServerUpdates()
{
	// Bind to port
	if (!bindSocket('i', port_ci)) {
		std::cout << "Could not bind port " << port_ci << ", used to "
		          << "listen for server updates." << std::endl;
		return;
	}

	std::cout << "Server Update Listener Started..." << std::endl;

	sf::IpAddress ip;
	unsigned short port;
	while (true) {
		sf::Packet packet;
		if (sock_i.receive(packet, ip, port) != sf::Socket::Done) {
			std::cout << "Error recieving server update. Ignoring."
			          << std::endl;
			continue;
		}

		// Sort packet into respective system.
		sortPacket(packet);
	}

	return;
}

uint32_t handshakeHello(anax::World& world, 
		        Ogre::SceneManager *scene,
                        tempo::SystemLevelManager system_gm)
{
	// Package up payload
	sf::Packet packet;
	packet << static_cast<uint32_t>(HandshakeID::HELLO);
	packet << sf::IpAddress::getLocalAddress().toInteger();
	packet << port_ci;

	// Send HELLO
	sendMessage(SystemQID::HANDSHAKE, packet);

	// Wait until we recieve HELLO_ROG
	tempo::Queue<sf::Packet> *queue = get_system_queue(SystemQID::HANDSHAKE);
	while (queue->empty()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(40));
	}

	// Take response from queue
	packet = queue->front();
	queue->pop();

	// Extract Data
	uint32_t msg = static_cast<uint32_t>(HandshakeID::DEFAULT);
	uint32_t id = NO_CLIENT_ID;
	uint32_t entityCount = 0;
	packet >> msg;
	if (msg == static_cast<uint32_t>(HandshakeID::HELLO_ROG)) {
		packet >> id;
		packet >> port_si;
		packet >> port_st;
		packet >> entityCount;
		EntityCreationData e;
		for (int i = 0; i < entityCount; i++) {
			packet >> e;
			newEntity(e, world, scene, system_gm);
		}
	} else {
		std::cout << "We didn't get what we expected when connecting "
		          << "to the server." << std::endl;
	}

	return id;
}

bool handshakeRoleReq(uint32_t id, 
                      ClientRole roleID, 
                      ClientRoleData &roleData,
                      anax::World& world,
		      Ogre::SceneManager *scene,
                      tempo::SystemLevelManager system_gm)
{
	// Package up payload
	sf::Packet packet;
	packet << static_cast<uint32_t>(HandshakeID::ROLEREQ);
	packet << id;
	packet << roleID;
	packet << roleData;
	
	// Send ROLEREQ
	sendMessage(SystemQID::HANDSHAKE, packet);
	
	// Receive ROLEREQ_ROG
	sf::IpAddress sender;
	unsigned short port;
	sock_o.receive(packet, sender, port);
	
	// Extract Data
	uint32_t msg = static_cast<uint32_t>(HandshakeID::DEFAULT);
	packet >> msg;
	if (msg == static_cast<uint32_t>(HandshakeID::ROLEREQ_ROG)) {
		//TODO Extract entity/response from ROLEREQ_ROG
		EntityCreationData e;
		packet >> e;
		anax::Entity en = newEntity(e, world, scene, system_gm);
		en.removeComponent<tempo::ComponentPlayerRemote>();
		en.addComponent<tempo::ComponentPlayerLocal>();
	} else {
		std::cout << "The server was rude to us when we requested a "
		          << "role. >:(" << std::endl;
		return false;
	}
	
	return true;
}
	
bool connectToAndSyncWithServer(ClientRole roleID, 
                                ClientRoleData &roleData,
                                anax::World& world,
                                Ogre::SceneManager *scene,
                                tempo::SystemLevelManager system_gm)
{
	// Bind outgoing port if not bound
	if (sock_o.getLocalPort() == 0) {
		if (!bindSocket('o', port_co)) {
			std::cout << "Could not bind socket on port " << port_co
			          << " to connect to and sync with server." 
			          << std::endl;
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

	uint32_t id = handshakeHello(world, scene, system_gm);
	if (id == NO_CLIENT_ID) {
		std::cout << "Couldn't connect to the server." 
		          << std::endl;
		return false;
	}

	return handshakeRoleReq(id, roleID, roleData, world, scene, system_gm);
}

}
