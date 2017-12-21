#include <tempo/networkBase.hpp>

namespace tempo
{

sf::UdpSocket socket;
	
bool sendMessage(tempo::SystemQID id, sf::Packet payload) {
	sf::Packet message;
	uint8_t temp;

	// Construct message
	message << id;
	while (payload >> temp)
	{
		message << temp;
	}

	// Send message
	return socket.send(message, NET_ADDR, NET_PORT_DT) == sf::Socket::Done;
}

}
