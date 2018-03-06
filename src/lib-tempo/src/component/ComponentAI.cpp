#include <tempo/component/ComponentAI.hpp>

namespace tempo
{
ComponentAI::ComponentAI()
{
	return;  // does nothing
}

/////
// Required for networking
/////
ComponentAI::ComponentAI(sf::Packet p)
{
	return;  // does nothing
}

ComponentID ComponentAI::getId()
{
	return ComponentID::AI;
}

sf::Packet ComponentAI::dumpComponent()
{
	return sf::Packet();  // does nothing
}

}  // namespace tempo
