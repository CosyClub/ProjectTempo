#ifndef TEMPO_COMPONENT_COMPONENT_AOE_INDICATOR_HPP
#define TEMPO_COMPONENT_COMPONENT_AOE_INDICATOR_HPP

#include <anax/Component.hpp>

#include <tempo/component/NetworkedComponent.hpp>
#include <tempo/network/base.hpp>

#include <SFML/System/Time.hpp>
#include <glm/vec2.hpp>

namespace tempo
{
struct ComponentAOEIndicator
    : anax::Component
    , NetworkedComponent {
	sf::Time                duration;
	std::vector<glm::ivec2> tiles;

	ComponentAOEIndicator();

	ComponentAOEIndicator(sf::Time duration, std::vector<glm::ivec2> tiles);

	/////
	// Required for networking
	/////
	ComponentAOEIndicator(sf::Packet p);
	sf::Packet  dumpComponent();
	ComponentID getId();
};

}  // namespace tempo

#endif
