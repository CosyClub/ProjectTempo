#ifndef TEMPO_COMPONENT_COMPONENTMODEL_HPP 
#define TEMPO_COMPONENT_COMPONENTMODEL_HPP 

#include <anax/Component.hpp>

#include <tempo/component/NetworkedComponent.hpp>

namespace tempo {

struct ComponentModel : anax::Component, NetworkedComponent
{
	std::string path;

	ComponentModel(std::string path);

	/////
	// Required for networking
	/////
	ComponentModel(sf::Packet p);
	sf::Packet dumpComponent();
	ComponentID getId();
};

} // namespace tempo

#endif
