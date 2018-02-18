#include <tempo/network/QueueID.hpp>

namespace tempo
{

sf::Packet& operator <<(sf::Packet& packet, const ComponentID id)
{
	return packet << uint32_t(id);
}
sf::Packet& operator >>(sf::Packet& packet, ComponentID& id)
{
	uint32_t data;
	packet >> data;
	id = (ComponentID)data;
	return packet << (id);
}

} // namespace tempo
