#include <tempo/networkBase.hpp>

namespace tempo
{

sf::UdpSocket sock_i;
sf::UdpSocket sock_o;
sf::UdpSocket sock_h;

sf::IpAddress addr_r = "0.0.0.0";
unsigned int port_ci = 0;
unsigned int port_co = 0;  // Should be set within the Client or Server 
unsigned int port_sh = 0;  // at runtime. This way user input/config files
unsigned int port_si = 0;  // can be used to set address/ports dynamically.
unsigned int port_so = 0;  
unsigned int port_st = 0;

bool bindSocket(char socket, unsigned short port)
{
	switch (socket) {
		case 'i': return sock_i.bind(port) == sf::Socket::Done;
		case 'o': return sock_o.bind(port) == sf::Socket::Done;
		case 'h': return sock_h.bind(port) == sf::Socket::Done;
		default:  return false;
	}
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

}
