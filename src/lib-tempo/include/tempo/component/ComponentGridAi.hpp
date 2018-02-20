#ifndef TEMPO_COMPONENT_GRID_AI_HPP
#define TEMPO_COMPONENT_GRID_AI_HPP

#include <anax/Component.hpp>

#include <tempo/component/NetworkedComponent.hpp>

namespace tempo
{

struct ComponentGridAi : anax::Component, NetworkedComponent
{
	ComponentGridAi();

	/////
	// Required for networking
	/////
	ComponentGridAi(sf::Packet p);
	ComponentID getId();
	sf::Packet dumpComponent();
};

} // namespace tempo

#endif
