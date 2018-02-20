#include <tempo/component/ComponentPlayerRemote.hpp>

namespace tempo
{

ComponentPlayerRemote::ComponentPlayerRemote(sf::Packet p)
{
	return; // Do nothing as this struct is currently empty
}

ComponentID ComponentPlayerRemote::getId()
{
	return ComponentID::PLAYER_REMOTE;
}

sf::Packet ComponentPlayerRemote::dumpComponent()
{
	return sf::Packet();
}

} // namespace tempo
