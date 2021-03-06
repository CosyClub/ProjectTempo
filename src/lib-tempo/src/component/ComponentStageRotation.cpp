#include <tempo/component/ComponentStageRotation.hpp>
#include <array>

namespace tempo
{
glm::ivec2 NORTH = glm::ivec2(-1, 0);
glm::ivec2 EAST  = glm::ivec2(0, 1);
glm::ivec2 SOUTH = glm::ivec2(1, 0);
glm::ivec2 WEST  = glm::ivec2(0, -1);
std::array<glm::ivec2, 4> DIRECTIONS{{NORTH, SOUTH, WEST, EAST}};

ComponentStageRotation::ComponentStageRotation(Facing f)
{
	facing = f;
}

ComponentStageRotation::ComponentStageRotation(sf::Packet p)
{
	p >> facing.x;
	p >> facing.y;
}

sf::Packet ComponentStageRotation::dumpComponent()
{
	sf::Packet p;

	p << facing.x;
	p << facing.y;

	return p;
}
ComponentID ComponentStageRotation::getId()
{
	return ComponentID::STAGE_ROTATION;
}

}  // namespace tempo
