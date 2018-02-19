#ifndef TEMPO_COMPONENT_STAGE_TRANSLATION_HPP
#define TEMPO_COMPONENT_STAGE_TRANSLATION_HPP

#include <tempo/network/QueueID.hpp>
#include <tempo/component/NetworkedComponent.hpp>

#include <anax/Component.hpp>

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

namespace tempo
{
	
// Represents a translation on the stage
struct ComponentStageTranslation : anax::Component, NetworkedComponent
{
	// The position translation
	ComponentID id;
	glm::ivec2 delta;
	// Creates the stage translation with no delta
	ComponentStageTranslation();
	
	sf::Packet dumpComponent();
	void restoreComponent(sf::Packet p);

};


void restoreComponentStageTranslation(anax::Entity& e, sf::Packet p);

} // namespace tempo

#endif
