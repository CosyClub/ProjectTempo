#include <tempo/component/ComponentAI.hpp>

namespace tempo
{
ComponentAI::ComponentAI(MoveType m, bool teleport, bool slide)
{
	type = m;
	hitTeleport = teleport;
	hitSlide = slide;
}

/////
// Required for networking
/////
ComponentAI::ComponentAI(sf::Packet p)
{
	uint32_t tmp;
	int path_size;
	p >> tmp;
	type = (MoveType) tmp;
	p >> hitTeleport;
	p >> hitSlide;
	p >> path_size;
	for (int I = 0; I < path_size; I++)
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

	p << (uint32_t) type;
	p << hitTeleport;
	p << hitSlide;
	p << (uint32_t) path.size();
	for (glm::ivec2 vec : path)
	{
		p << vec;
	}
	return sf::Packet();  // does nothing
}

}  // namespace tempo
