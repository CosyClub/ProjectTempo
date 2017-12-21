#include <tempo/networkBase.hpp>

namespace tempo
{

sf::UdpSocket sock_i;
sf::UdpSocket sock_o;
sf::UdpSocket sock_h;

sf::IpAddress addr_r = DEFAULT_ADDR;
unsigned int port_ci = DEFAULT_PORT_IN;
unsigned int port_co = DEFAULT_PORT_OUT;
unsigned int port_si = DEFAULT_PORT_IN;
unsigned int port_so = DEFAULT_PORT_OUT;
unsigned int port_sh = DEFAULT_PORT_HS;
unsigned int port_st = DEFAULT_PORT_TS;

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
