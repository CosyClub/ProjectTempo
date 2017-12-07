#include <tempo/networkBase.hpp>

sf::UdpSocket socket = null;
	
bool sendMessage(tempo::SystemMsgID id, sf::Packet payload) {
	// Check if socket is open.
	if (socket == null) {
		return false; 
	} else {
		// TODO Construct message	

		// Send message
		socket.send()
	}
}
