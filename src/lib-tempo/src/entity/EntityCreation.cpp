#include <tempo/entity/EntityCreation.hpp>


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
		std::cout << "Recieved New Entity" << std::endl;
		e = w.createEntity();
		localid = e.getId();
		servertolocal.emplace(id, localid);
		localtoserver.emplace(localid, id);
		e.activate();
	}
	else
	{
		std::cout << "Adding component to current entity" << std::endl;
		localid = a->second;
		e = anax::Entity(w, localid);
		e.activate();
	}

	switch (component_id) {
	case ComponentID::STAGE_TRANSLATION :
		if (!e.hasComponent<ComponentStageTranslation>()) {
			e.addComponent<ComponentStageTranslation>();
		}
		e.getComponent<ComponentStageTranslation>().restoreComponent(p);
		break;
	default :
		std::cout << "WARNING: Unimplemented deserialisation of"
			     " recieved component occured, ignoring." 
		          << std::endl;
	}

	return e;
}

} // namespace tempo
