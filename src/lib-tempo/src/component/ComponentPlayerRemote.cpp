#include <tempo/component/ComponentPlayerRemote.hpp>

namespace tempo
{
ComponentPlayerRemote::ComponentPlayerRemote()
{
	id = ComponentID::PLAYER_REMOTE;
	moved_this_beat = false;
}

ComponentPlayerRemote::ComponentPlayerRemote(sf::Packet p)
{
	id = ComponentID::PLAYER_REMOTE;
	p >> moved_this_beat;
}

sf::Packet ComponentPlayerRemote::dumpComponent()
{
	sf::Packet p;

	p << moved_this_beat;

	return p;
}

}
