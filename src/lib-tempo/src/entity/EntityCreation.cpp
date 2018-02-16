#include <tempo/entity/EntityCreation.hpp>

namespace tempo
{

sf::Packet& operator <<(sf::Packet& packet, const EntityCreationData& data)
{
	packet << data.type_id;
	packet << data.position;
	packet << data.instance_id;
	packet << data.entity_type;
	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const Entity_Type& type)
{
	uint8_t *data = (uint8_t*)(&type);

	for (int I = 0; I < sizeof(Entity_Type); I++)
	{
		packet << data[I];
	}

	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const glm::vec2& vec)
{
	packet << vec.x;
	packet << vec.y;

	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, EntityCreationData& data)
{
	int tmp;

	packet >> tmp;
	data.type_id = (EID)tmp;
	packet >> data.position;
	packet >> data.instance_id;
	packet >> data.entity_type;
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, Entity_Type& type)
{
	uint8_t *data = (uint8_t*)(&type);

	uint8_t c;
	for (int I = 0; I < sizeof(Entity_Type); I++)
	{
		packet >> c;
		data[I] = c;
	}

	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, glm::vec2& vec)
{
	packet >> vec.x;
	packet >> vec.y;

	return packet;
}

}
