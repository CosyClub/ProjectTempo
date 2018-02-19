#include <tempo/entity/EntityCreation.hpp>

#include <tempo/component/ComponentTransform.hpp>

namespace tempo
{

anax::Entity addComponent(anax::World& w, sf::Packet p)
{
	anax::Entity::Id id;
	tempo::ComponentID component_id;

	p >> id;
	p >> component_id;

	anax::Entity e = anax::Entity(w, id);
	e.activate();
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
}

} // namespace tempo
