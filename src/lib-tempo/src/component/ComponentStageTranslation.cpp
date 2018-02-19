#include <tempo/component/ComponentStageTranslation.hpp>

namespace tempo
{

ComponentStageTranslation::ComponentStageTranslation() 
: delta(0, 0) 
{
	id = ComponentID::TRANSFORM;
}

ComponentStageTranslation::ComponentStageTranslation(sf::Packet p)
{
	p >> delta.x;
	p >> delta.y;
}

sf::Packet ComponentStageTranslation::dumpComponent()
{
	sf::Packet p;
	p << delta.x;
	p << delta.y;

	return p;
}

} // namespace tempo
