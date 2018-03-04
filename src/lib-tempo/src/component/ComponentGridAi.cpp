#include <tempo/component/ComponentGridAi.hpp>

namespace tempo
{
ComponentGridAi::ComponentGridAi()
{
	return;  // does nothing
}

/////
// Required for networking
/////
ComponentGridAi::ComponentGridAi(sf::Packet p)
{
	return;  // does nothing
}

ComponentID ComponentGridAi::getId()
{
	return ComponentID::GRID_AI;
}

sf::Packet ComponentGridAi::dumpComponent()
{
	return sf::Packet();  // does nothing
}

}  // namespace tempo
