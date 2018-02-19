#include <tempo/component/ComponentPlayerLocal.hpp>

namespace tempo
{

ComponentPlayerLocal::ComponentPlayerLocal()
{
	moved_this_beat = false;
}
	
ComponentPlayerLocal::ComponentPlayerLocal(sf::Packet p)
{
	p >> moved_this_beat;
	p >> counter_combo;
	p >> level_combo;
}

sf::Packet ComponentPlayerLocal::dumpComponent()
{
	sf::Packet p;
	p << moved_this_beat;
	p << counter_combo;
	p << level_combo;

	return p;
}

ComponentID ComponentPlayerLocal::getID()
{
	return ComponentID::PLAYER_LOCAL;
}

}
