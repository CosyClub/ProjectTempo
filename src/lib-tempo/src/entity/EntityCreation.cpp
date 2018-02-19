#include <tempo/entity/EntityCreation.hpp>

#define CASE(NAME, CID) case ComponentID::CID : 					\
				if (!e.hasComponent<NAME>()) { 			\
					e.addComponent<NAME>(p);			\
				}							\
				else							\
				{							\
					std::cout << "Warning: Reinstanciation of "	\
						  << "" #NAME 				\
						  << std::endl;				\
				}							\
				break;
namespace tempo
{

anax::Entity addComponent(anax::World& w, sf::Packet p)
{
	anax::Entity::Id id;
	anax::Entity::Id localid;
	anax::Entity e;
	tempo::ComponentID component_id;

	p >> id;
	p >> component_id;

	auto a = servertolocal.find(id);
	if (a == servertolocal.end())
	{
		//Looks like it's a new one
		std::cout << "Recieved New Entity with server ID " << id.index
		          << std::endl;
		e = w.createEntity();
		localid = e.getId();
		servertolocal.emplace(id, localid);
		localtoserver.emplace(localid, id);
		e.activate();
	}
	else
	{
		std::cout << "Using current Entity with server ID " << id.index 
		          << std::endl;
		localid = a->second;
		e = anax::Entity(w, localid);
		e.activate();
	}
	
	std::cout << "Adding Component with ID " << component_id << std::endl;
	switch (component_id) {
	CASE(ComponentHealth, HEALTH)
	CASE(ComponentStageTranslation, STAGE_TRANSLATION)
	CASE(ComponentPlayerLocal, PLAYER_LOCAL)
	default :
		std::cout << "WARNING: Unimplemented deserialisation of"
			     " recieved component occured, ignoring." 
		          << std::endl;
	}

	return e;
}

} // namespace tempo
