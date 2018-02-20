#include <tempo/component/ComponentModel.hpp>

namespace tempo {

ComponentModel::ComponentModel(std::string p, glm::vec3 c)
{
	path = p;
	color = c;
}

/////
// Required for networking
/////
ComponentModel::ComponentModel(sf::Packet p)
{
	p >> path;
	p >> color.x;
	p >> color.y;
	p >> color.z;
}

sf::Packet ComponentModel::dumpComponent()
{
	sf::Packet p;
	
	p << path;
	p << color.x;
	p << color.y;
	p << color.z;

	return p;
}

ComponentID ComponentModel::getId()
{
	return ComponentID::MODEL;
}

} // namespace tempo

