#include <tempo/component/ComponentStagePosition.hpp>

#include <glm/vec2.hpp>

namespace tempo
{

ComponentStagePosition::ComponentStagePosition(glm::ivec2 bottom_left, 
                                               glm::ivec2 top_right)
                                             : occupied(0) 
{
	assert(bottom_left.x <= top_right.x && bottom_left.y <= top_right.y);

	for (int y = bottom_left.y; y < top_right.y; y++) {
		for (int x = bottom_left.x; x <= top_right.x; x++) {
			occupied.push_back(glm::ivec2(x, y));
		}
	}
}

ComponentStagePosition::ComponentStagePosition(glm::ivec2 position) 
	: occupied(0) 
{
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
{
	uint32_t size;
	p >> size;

	occupied.clear();
	for (int I = 0; I < size; I++) {
		uint32_t x;
		uint32_t y;

		p >> x;
		p >> y;

		occupied.push_back(glm::ivec2(x, y));
	}
}

sf::Packet ComponentStagePosition::dumpComponent()
{
	sf::Packet p;
	uint32_t size;

	size = occupied.size();
	p << size;

	for (int I = 0; I < size; I++) {
		uint32_t x;
		uint32_t y;

		x = occupied[I].x;
		y = occupied[I].y;
		
		p << x;
		p << y;
	}
	
	return p;
}

ComponentID ComponentStagePosition::getId()
{
	return ComponentID::STAGE_POSITION;
}

} // namespace tempo
