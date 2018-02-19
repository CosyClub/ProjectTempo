#include <tempo/network/base.hpp>

namespace tempo
{

sf::UdpSocket sock_i;
sf::UdpSocket sock_o;
sf::UdpSocket sock_h;

sf::IpAddress addr_r = "0.0.0.0";
unsigned short port_ci = 0;
unsigned short port_co = 0;  // Should be set within the Client or Server
unsigned short port_si = 0;  // at runtime. This way user input/config files
unsigned short port_so = 0;  // can be used to set address/ports dynamically.
unsigned short port_st = 0;

bool bindSocket(char socket, unsigned short port)
{
	switch (socket) {
		case 'i': 
			if (sock_i.getLocalPort() == 0) {
				return sock_i.bind(port) == sf::Socket::Done;
			} else {
				return sock_i.getLocalPort() == port;
			}
		
		case 'o': 
			if (sock_o.getLocalPort() == 0) {
				return sock_o.bind(port) == sf::Socket::Done;
			} else {
				return sock_o.getLocalPort() == port;
			}
		
		case 'h': 
			if (sock_h.getLocalPort() == 0) {
				return sock_h.bind(port) == sf::Socket::Done;
			} else {
				return sock_h.getLocalPort() == port;
			}
		
		default:  return false;
	}
}

bool sortPacket(sf::Packet p)
{
	int id; // Should be a tempo::systemQID but they're the same
	tempo::SystemQID qid;

	// Get ID
	p >> id;

	// Convert
	qid = tempo::SystemQID(id);

	// Basic checking
	if (id <= tempo::QID_RESERVED_BOTTOM || id >= tempo::QID_RESERVED_TOP) {
		return false;
	}

	//Sort into queue
	tempo::Queue<sf::Packet>* q = tempo::get_system_queue(qid);
	q->push(p);

	return true;
}
	
sf::Packet& operator <<(sf::Packet& p1, sf::Packet& p2)
{
	uint8_t temp;
	while (p2 >> temp) {
		p1 << temp;
	}
	return p1;
}
	
sf::Packet& operator <<(sf::Packet& p, const ClientRoleData& c)
{
	return p << c.name;
}

sf::Packet& operator >>(sf::Packet& p, ClientRoleData& c)
{
	return p >> c.name;	
}

sf::Packet& operator <<(sf::Packet& packet, const anax::Entity::Id id)
{
	uint64_t data = 0;
	uint8_t* raw_data = (uint8_t*)&data;

	#ifdef ANAX_32_BIT_ENTITY_IDS
	data += uint32_t(id);
	#else
	data += uint64_t(id);
	#endif

	for (int I = 0; I < sizeof(uint64_t); I++)
	{
		packet << raw_data[I];
	}
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, anax::Entity::Id& id)
{
	uint64_t data = 0;
	uint8_t* raw_data = (uint8_t*)&data;
	for (int I = 0; I < sizeof(uint64_t); I++)
	{
		packet >> raw_data[I];
	}
	#ifdef ANAX_32_BIT_ENTITY_IDS
	data += uint32_t(id);
	#else
	data += uint64_t(id);
	#endif
	return packet;
}

}
