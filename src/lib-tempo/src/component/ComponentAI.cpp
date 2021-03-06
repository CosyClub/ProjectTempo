#include <tempo/component/ComponentAI.hpp>

int ai_index_top = 0;

namespace tempo
{

ComponentAI::ComponentAI(MoveType m, bool teleport, bool slide)
{
	type = m;
	hitTeleport = teleport;
	hitSlide = slide;
	index =  ai_index_top++;
}
ComponentAI::ComponentAI(MoveType m, bool teleport, bool slide, std::deque<glm::ivec2> p)
{
	type = m;
	hitTeleport = teleport;
	hitSlide = slide;
	path = p;
	index =  ai_index_top++;
}

/////
// Required for networking
/////
ComponentAI::ComponentAI(sf::Packet p)
{
	uint32_t tmp;
	uint32_t path_size;
	p >> tmp;
	type = (MoveType)tmp;
	p >> hitTeleport;
	p >> hitSlide;
	p >> path_size;
	for (uint32_t I = 0; I < path_size; I++)
	{
		glm::ivec2 tmp;
		p >> tmp;
		path.push_back(tmp);
	}
}

ComponentID ComponentAI::getId()
{
	return ComponentID::AI;
}

sf::Packet ComponentAI::dumpComponent()
{
	sf::Packet p;

	p << (uint32_t)type;
	p << hitTeleport;
	p << hitSlide;
	p << (uint32_t) path.size();
	for (uint32_t I = 0; I < path.size(); I++)
	{
		glm::ivec2 vec = path.front();
		path.pop_front();
		p << vec;
		path.push_back(vec);
	}
	return p;  // does nothing
}

}  // namespace tempo
