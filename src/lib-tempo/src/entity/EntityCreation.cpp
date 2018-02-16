#include <tempo/entity/EntityCreation.hpp>

namespace tempo
{

anax::Entity addComponent(anax::World& w, sf::Packet p)
{
	anax::Entity::Id id;
	tempo::Component_ID component_id;

	p >> id;
	p >> component_id;

	anax::Entity e = anax::Entity(w, id);
	auto f = restore_map.find(component_id);
	if (f == restore_map.end())
	{
		//FUCK
		std::cout << "Failed to find component with ID " << component_id
		          << std::endl;	
		return e;
	}
	(*f->second)(e, p);
}

sf::Packet& operator <<(sf::Packet& packet, const anax::Entity::Id id)
{
	uint64_t data = 0;
	uint8_t* raw_data = (uint8_t*)&data;

	#ifdef ANAX_32_BIT_ENTITY_IDS
	data += uint32_t(id);
	#else
	data += uint64_t(id);
	#endif

	for (int I = 0; I < sizeof(uint64_t); I++)
	{
		packet << raw_data[I];
	}
	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, anax::Entity::Id& id)
{
	uint64_t data = 0;
	uint8_t* raw_data = (uint8_t*)&data;
	for (int I = 0; I < sizeof(uint64_t); I++)
	{
		packet >> raw_data[I];
	}
	#ifdef ANAX_32_BIT_ENTITY_IDS
	data += uint32_t(id);
	#else
	data += uint64_t(id);
	#endif
	return packet;
}

}
