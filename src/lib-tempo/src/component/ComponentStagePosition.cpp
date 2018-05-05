#include <tempo/component/ComponentStagePosition.hpp>

#include <glm/vec2.hpp>
#include <iostream>

namespace tempo
{
ComponentStagePosition::ComponentStagePosition(glm::ivec2 bottom_left, glm::ivec2 top_right)
    : occupied(0)
{
	setPosition(bottom_left, top_right);
	isPhased = true;
}

ComponentStagePosition::ComponentStagePosition(glm::ivec2 position, bool phased)
    : occupied(0)
{
	setPosition(position);
	isPhased = phased;
}

ComponentStagePosition::ComponentStagePosition(glm::ivec2 position)
    : occupied(0)
{
	setPosition(position);
	isPhased = true;
}

void ComponentStagePosition::movePosition(glm::ivec2 delta)
{
	for (unsigned int i = 0; i < occupied.size(); i++) {
		occupied[i] = occupied[i] + delta;
	}
}

void ComponentStagePosition::setPosition(glm::ivec2 bottom_left, glm::ivec2 top_right)
{
	occupied.clear();
	assert(bottom_left.x <= top_right.x && bottom_left.y <= top_right.y);

	for (int y = bottom_left.y; y < top_right.y; y++) {
		for (int x = bottom_left.x; x <= top_right.x; x++) {
			occupied.push_back(glm::ivec2(x, y));
		}
	}
}

void ComponentStagePosition::setPosition(glm::ivec2 position)
{
	occupied.clear();
	occupied.push_back(position);
}

std::vector<glm::ivec2> ComponentStagePosition::getOccupied()
{
	return occupied;
}

glm::ivec2 ComponentStagePosition::getOrigin()
{
	return occupied[0];
}

// Required for inital network sync
ComponentStagePosition::ComponentStagePosition(sf::Packet p)
    : occupied(0)
{
	uint32_t size;
	p >> size;

	occupied.clear();
	for (unsigned int i = 0; i < size; i++) {
		uint32_t x, y;
		p >> x >> y;
		occupied.push_back(glm::ivec2(x, y));
	}
}

sf::Packet ComponentStagePosition::dumpComponent()
{
	sf::Packet p;
	uint32_t   size = occupied.size();
	p << size;

	for (unsigned int i = 0; i < size; i++) {
		p << occupied[i].x << occupied[i].y;
	}

	return p;
}

ComponentID ComponentStagePosition::getId()
{
	return ComponentID::STAGE_POSITION;
}

}  // namespace tempo
