#include <tempo/component/ComponentStageTranslation.hpp>

namespace tempo
{
ComponentStageTranslation::ComponentStageTranslation()
{
	delta = glm::ivec2(0, 0);
	moved = false;
}

ComponentStageTranslation::ComponentStageTranslation(sf::Packet p)
{
	p >> delta.x;
	p >> delta.y;
	p >> moved;
}

sf::Packet ComponentStageTranslation::dumpComponent()
{
	sf::Packet p;
	p << delta.x;
	p << delta.y;
	p << moved;

	return p;
}
ComponentID ComponentStageTranslation::getId()
{
	return ComponentID::STAGE_TRANSLATION;
}

}  // namespace tempo
