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
	e.activate();
	anax::detail::TypeId type_id;
	if (! e.getComponentTypeList()[type_id])
	{
		e.addComponent<>
	}
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


}
