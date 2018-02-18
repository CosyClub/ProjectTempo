#include <tempo/entity/EntityCreation.hpp>

#include <tempo/component/ComponentTransform.hpp>

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
	switch (component_id)
	{
		case Component_ID::CID_TRANSFORM :
			if (! e.hasComponent<ComponentTransform>())
			{
				e.addComponent<ComponentTransform>();
			}
			ComponentTransform c = e.getComponent<ComponentTransform>();
			NetworkedComponent *nc = static_cast<NetworkedComponent*>(&c);		
			nc->restoreComponent(p);
			break;
	}
}


}
