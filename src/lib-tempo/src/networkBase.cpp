#include <tempo/networkBase.hpp>

namespace tempo
{

sf::UdpSocket socket;
	
bool sendMessage(tempo::SystemQID id, sf::Packet payload) {
	sf::Packet message;
	uint8_t temp;

	// Construct message
	message << id;
	message << payload;

	// Send message
	return socket.send(message, NET_ADDR, NET_PORT_DT) == sf::Socket::Done;
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
