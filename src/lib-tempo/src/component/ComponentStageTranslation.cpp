#include <tempo/component/ComponentStageTranslation.hpp>

namespace tempo
{
ComponentStageTranslation::ComponentStageTranslation()
{
	delta = glm::ivec2(0, 0);
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
ComponentID ComponentStageTranslation::getId()
{
	return ComponentID::STAGE_TRANSLATION;
}

}  // namespace tempo
