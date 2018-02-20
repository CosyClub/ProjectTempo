#include <tempo/component/ComponentPlayerLocal.hpp>

namespace tempo
{

ComponentPlayerLocal::ComponentPlayerLocal(sf::Packet p)
{
	return; // Do nothing as this struct is currently empty
}

ComponentID ComponentPlayerLocal::getId()
{
	return ComponentID::PLAYER_LOCAL;
}

sf::Packet ComponentPlayerLocal::dumpComponent()
{
	return sf::Packet();
}

} // namespace tempo
