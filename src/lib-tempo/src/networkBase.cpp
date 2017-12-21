#include <tempo/networkBase.hpp>

namespace tempo
{

sf::UdpSocket sock_i;
sf::UdpSocket sock_o;
sf::UdpSocket sock_h;

bool bindSocket(char socket, unsigned short port)
{
	switch (socket) {
		case 'i': return sock_i.bind(port) == sf::Socket::Done;
		case 'o': return sock_o.bind(port) == sf::Socket::Done;
		case 'h': return sock_h.bind(port) == sf::Socket::Done;
		default:  return false;
	}
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
	// TODO if (isHandshake) {
	return sock_o.send(message, NET_ADDR, NET_PORT_DT) == sf::Socket::Done;
}

sf::Packet& operator <<(sf::Packet& p1, sf::Packet& p2)
{
	uint8_t temp;

	while (p2 >> temp)
	{
		p1 << temp;
	}

	return p1;
}

}
