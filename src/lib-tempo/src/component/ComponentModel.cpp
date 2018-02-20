#include <tempo/component/ComponentModel.hpp>

namespace tempo {

ComponentModel::ComponentModel(std::string path)
{
	path = path;
}

/////
// Required for networking
/////
ComponentModel::ComponentModel(sf::Packet p)
{
	p >> path;
}

sf::Packet ComponentModel::dumpComponent()
{
	sf::Packet p;
	
	p << path;

	return p;
}

ComponentID ComponentModel::getId()
{
	return ComponentID::MODEL;
}

} // namespace tempo

