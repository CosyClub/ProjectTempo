#include <tempo/component/ComponentModel.hpp>

namespace tempo
{
ComponentModel::ComponentModel(std::string p, glm::vec3 c, bool mesh, glm::ivec2 sdim)
{
	path   = p;
	color  = c;
	isMesh = mesh;
	spriteDim = sdim;
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
	p >> isMesh;
	p >> spriteDim;
}

sf::Packet ComponentModel::dumpComponent()
{
	sf::Packet p;

	p << path;
	p << color.x;
	p << color.y;
	p << color.z;
	p << isMesh;
	p << spriteDim;

	return p;
}

ComponentID ComponentModel::getId()
{
	return ComponentID::MODEL;
}

}  // namespace tempo
