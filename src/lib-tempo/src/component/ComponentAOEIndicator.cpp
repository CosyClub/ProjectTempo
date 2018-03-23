#include <tempo/component/ComponentAOEIndicator.hpp>

namespace tempo
{
ComponentAOEIndicator::ComponentAOEIndicator()
{
	this->duration = sf::Time::Zero;
	this->tiles = std::vector<glm::ivec2>();
}

ComponentAOEIndicator::ComponentAOEIndicator(sf::Time duration, std::vector<glm::ivec2> tiles)
{
	this->duration = duration;
	this->tiles = tiles;
}

/////
// Required for networking
/////
ComponentAOEIndicator::ComponentAOEIndicator(sf::Packet p)
{
	uint32_t items;
	p >> duration;
	p >> items;

	for (int I = 0; I < static_cast<int>(items); I++) {
		glm::ivec2 tmp;
		p >> tmp;
		tiles.push_back(tmp);
	}
}

sf::Packet ComponentAOEIndicator::dumpComponent()
{
	sf::Packet p;
	uint32_t items = tiles.size();

	p << duration;
	p << items;

	for (int I = 0; I < static_cast<int>(items); I++) {
		p << tiles[I];
	}

	return p;
}

ComponentID ComponentAOEIndicator::getId()
{
	return ComponentID::AOEINDICATOR;
}

}  // namespace tempo
