#ifndef TEMPO_COMPONENT_STAGE_TRANSLATION_HPP
#define TEMPO_COMPONENT_STAGE_TRANSLATION_HPP

#include <tempo/component/NetworkedComponent.hpp>

#include <anax/Component.hpp>

#include <glm/fwd.hpp>
#include <glm/vec2.hpp>

namespace tempo
{
// Represents a translation on the stage
struct ComponentStageTranslation
    : anax::Component
    , NetworkedComponent {
	// The position translation
	glm::ivec2 delta;
	bool moved;
	// Creates the stage translation with no delta
	ComponentStageTranslation();

	/////
	// Required for inital network sync
	/////
	ComponentStageTranslation(sf::Packet p);
	sf::Packet  dumpComponent();
	ComponentID getId();
};

}  // namespace tempo

#endif
