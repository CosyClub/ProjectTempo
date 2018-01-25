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
/* 			printf("\n\n\n\n%d\n\n\n\n\n", entity->entity_type.player.some_data_for_player); */
/* 			break; */
/* 		case EID_AI: */
/* 			entity->entity_type.ai.some_data_for_ai = EID_AI; */
/* 			printf("\n\n\n\n%d\n\n\n\n\n", entity->entity_type.ai.some_data_for_ai); */
/* 			break; */
/* 		case EID_DES: */
/* 			entity->entity_type.destroyable.some_data_for_destroyable = EID_DES; */
/* 			printf("\n\n\n\n%d\n\n\n\n\n", entity->entity_type.destroyable.some_data_for_destroyable); */
/* 			break; */
/* 		case EID_NONDES: */
/* 			entity->entity_type.nondestroyable.some_data_for_nondestroyable = EID_NONDES; */
/* 			printf("\n\n\n\n%d\n\n\n\n\n", entity->entity_type.nondestroyable.some_data_for_nondestroyable); */
/* 			break; */
/* 		default: printf("Missed all\n"); */
/* 	} */

/* 	return entity; */
/* } */

sf::Packet& operator <<(sf::Packet& packet, const EntityCreationData& data)
{
	packet << data.type_id;
	packet << data.position;
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
	packet >> data.entity_type;
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, Entity_Type& type)
{
	uint8_t *data = (uint8_t*)(&type);

	for (int I = 0; I < sizeof(Entity_Type); I++)
	{
		packet >> data[I];
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