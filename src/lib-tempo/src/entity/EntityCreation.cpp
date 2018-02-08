#include <tempo/entity/EntityCreation.hpp>

namespace tempo
{

/* EntityCreationData* newEntity(EID type_id, Vec2s pos){ */

/* 	EntityCreationData* entity = new EntityCreationData; */
/* 	entity->type_id = type_id; */
/* 	entity->position = pos; */

/* 	switch (entity->type_id){ */
/* 		case EID_PLAYER: */
/* 			entity->entity_type.player.some_data_for_player = EID_PLAYER; */
/* 			break; */
/* 		case EID_AI: */
/* 			entity->entity_type.ai.some_data_for_ai = EID_AI; */
/* 			break; */
/* 		case EID_DES: */
/* 			entity->entity_type.destroyable.some_data_for_destroyable = EID_DES; */
/* 			break; */
/* 		case EID_NONDES: */
/* 			entity->entity_type.nondestroyable.some_data_for_nondestroyable = EID_NONDES; */
/* 			break; */
/* 		default: printf("Missed all\n"); */
/* 	} */

/* 	return entity; */
/* } */

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

sf::Packet& operator <<(sf::Packet& packet, const Vec2s& vec)
{
	packet << vec.elements[0];
	packet << vec.elements[1];

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

sf::Packet& operator >>(sf::Packet& packet, Vec2s& vec)
{
	packet >> vec.elements[0];
	packet >> vec.elements[1];

	return packet;
}

}
