#ifndef TEMPO_COMPONENT_COMPONENTMODEL_HPP
#define TEMPO_COMPONENT_COMPONENTMODEL_HPP

#include <anax/Component.hpp>

#include <tempo/component/NetworkedComponent.hpp>

#include <glm/vec3.hpp>

namespace tempo
{
struct ComponentModel
    : anax::Component
    , NetworkedComponent {
	bool        isMesh;
	std::string path;
	glm::vec3   color;

	ComponentModel(std::string p, glm::vec3 c, bool mesh);

	/////
	// Required for networking
	/////
	ComponentModel(sf::Packet p);
	sf::Packet  dumpComponent();
	ComponentID getId();
};

}  // namespace tempo

#endif
