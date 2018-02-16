#include <tempo/network/QueueID.hpp>

namespace tempo
{

restore_pointer_map restore_map;
dump_pointer_map dump_map;

sf::Packet& operator <<(sf::Packet& packet, const Component_ID id)
{
	return packet << uint32_t(id);
}
sf::Packet& operator >>(sf::Packet& packet, Component_ID& id)
{
	uint32_t data;
	packet >> data;
	id = (Component_ID)data;
	return packet << (id);
}

}
