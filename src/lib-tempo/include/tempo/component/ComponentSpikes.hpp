#ifndef TEMPO_COMPONENT_SPIKES_HPP
#define TEMPO_COMPONENT_SPIKES_HPP

#include <anax/Component.hpp>

#include <glm/vec2.hpp>

#include <tempo/component/NetworkedComponent.hpp>

namespace tempo
{

	struct ComponentSpikes : anax::Component, NetworkedComponent {

		std::vector<glm::ivec2>      spike_positions;
		bool isTriggered = false;


		ComponentSpikes(std::vector<glm::ivec2> positions);

		/////
		// Required for networking
		/////
		ComponentSpikes(sf::Packet p);
		ComponentID getId();
		sf::Packet  dumpComponent();
	};

	//sf::Packet &operator<<(sf::Packet &p, const button &c);
	//sf::Packet &operator>>(sf::Packet &p, button &c);

}  // namespace tempo

#endif
