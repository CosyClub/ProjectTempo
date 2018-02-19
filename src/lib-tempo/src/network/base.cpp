#include <tempo/network/base.hpp>

namespace tempo
{

sf::UdpSocket sock_i;
sf::UdpSocket sock_o;
sf::UdpSocket sock_h;

sf::IpAddress addr_r = "0.0.0.0";
unsigned short port_ci = 0;
unsigned short port_co = 0;  // Should be set within the Client or Server 
unsigned short port_sh = 0;  // at runtime. This way user input/config files
unsigned short port_si = 0;  // can be used to set address/ports dynamically.
unsigned short port_so = 0;  
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
	int id; //Should be a tempo::systemQID but they're the same
	tempo::SystemQID qid;

	//Get ID
	p >> id;

	//convert
	qid = tempo::SystemQID(id);

	//Basic checking
	if (id <= tempo::QID_RESERVED_BOTTOM || id >= tempo::QID_RESERVED_TOP)
	{
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
	uint64_t index = 0;
	uint64_t counter = 0;

	#ifdef ANAX_32_BIT_ENTITY_IDS
	index += uint32_t(id.index);
	counter += uint32_t(id.counter);
	std::cout << "Sending 32 bit ID " << index << ":" << counter << std::endl;
	#else
	index += uint64_t(id.index);
	counter += uint64_t(id.counter);
	std::cout << "Sending 64 bit ID " << sf::Uint64(index) << ":" << sf::Uint64(counter) << std::endl;
	#endif

	packet << sf::Uint64(index);
	packet << sf::Uint64(counter);

	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, anax::Entity::Id& id)
{
	sf::Uint64 index = 0;
	sf::Uint64 counter = 0;
	
	packet >> index;
	packet >> counter;

	#ifdef ANAX_32_BIT_ENTITY_IDS
	std::cout << "Receiving 32 bit ID " << index << ":" << counter << std::endl;
	id.index = uint32_t(index);
	id.counter = uint32_t(counter);
	#else
	std::cout << "Receiving 64 bit ID " << index << ":" << counter << std::endl;
	id.index = uint64_t(index);
	id.counter = uint64_t(counter);
	#endif
	return packet;
}

sf::Packet splitPacket(sf::Packet& packet, uint32_t size)
{
	uint8_t data = 0;
	sf::Packet p;
	for (int I = 0; I < size; I++)
	{
		packet >> data;
		p << data;
	}
	return p;
}

}
