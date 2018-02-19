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

bool sendMessage(tempo::SystemQID id, 
                 sf::Packet payload, 
                 bool isHandshake = false) 
{
	sf::Packet message;

	// Construct message
	message << id;
	message << payload;

	

	// Send message
	if (isHandshake) {
		return sock_o.send(message, addr_r, port_sh) == sf::Socket::Done;
	} else {
		return sock_o.send(message, addr_r, port_si) == sf::Socket::Done;
	}
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
	packet << port_ci;

	// Send HELLO
	sock_o.send(packet, addr_r, port_sh);

	// Recieve HELLO_ROG
	sf::IpAddress sender;
	unsigned short port;
	sock_o.receive(packet, sender, port);

	// Extract Data
	uint32_t msg = static_cast<uint32_t>(HandshakeID::DEFAULT);
	uint32_t id = NO_CLIENT_ID;
	uint32_t componentCount = 0;
	packet >> msg;
	if (msg == static_cast<uint32_t>(HandshakeID::HELLO_ROG)) {
		packet >> id;
		packet >> port_si;
		packet >> port_st;
		packet >> componentCount;

		sf::Packet p2;
		for (int i = 0; i < componentCount; i++) {
			sock_o.receive(p2, sender, port);
			std::cout << "Recieved " << i << "/" << componentCount << std::endl;
			addComponent(world, p2);
		}
	} else {
		std::cout << "The server was rude to us when we said hello. >:("
		          << std::endl;
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
	sock_o.send(packet, addr_r, port_sh);
	
	// Receive ROLEREQ_ROG
	sf::IpAddress sender;
	unsigned short port;
	sock_o.receive(packet, sender, port);
	
	// Extract Data
	uint32_t msg = static_cast<uint32_t>(HandshakeID::DEFAULT);
	int componentCount;
	packet >> msg;
	packet >> componentCount;
	if (msg == static_cast<uint32_t>(HandshakeID::ROLEREQ_ROG)) {
		//TODO Extract entity/response from ROLEREQ_ROG
		sf::Packet p2;
		anax::Entity en;
		for (int i = 0; i < componentCount; i++) {
			sock_o.receive(p2, sender, port);
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
		          << "connect to server without this!" << std::endl;
		return false;
	}

	std::cout << "HELLO STARTING" << std::endl;
	uint32_t id = handshakeHello(world, scene, system_gm);
	std::cout << "HELLO COMPLETE" << std::endl;
	if (id == NO_CLIENT_ID) {
		std::cout << "The server didn't like us saying HELLO!" 
		          << std::endl;
		return false;
	}

	std::cout << "ROLEREQ STARTING" << std::endl;
	bool ret = handshakeRoleReq(id, roleID, roleData, world, scene, system_gm);
	std::cout << "ROLEREQ COMPLETE" << std::endl;
	return ret;
}


}
