#ifndef TEMPO_COMPONENT_COMPONENTMODEL_HPP
#define TEMPO_COMPONENT_COMPONENTMODEL_HPP

#include <anax/Component.hpp>

#include <tempo/component/NetworkedComponent.hpp>

#include <tempo/network/base.hpp>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace tempo
{
struct ComponentModel
    : anax::Component
    , NetworkedComponent {
	bool        isMesh;
	std::string path;
	glm::vec3   color;
	glm::ivec2  spriteDim;

ComponentModel(std::string p, glm::vec3 c, bool mesh, glm::ivec2 sdim);

	/////
	// Required for networking
	/////
	ComponentModel(sf::Packet p);
	sf::Packet  dumpComponent();
	ComponentID getId();
};

}  // namespace tempo

#endif
