#include <tempo/component/ComponentTeam.hpp>

namespace tempo
{

ComponentTeam::ComponentTeam(Team t)
{
	team = t;
}

ComponentTeam::ComponentTeam(sf::Packet p)
{
	uint32_t tmp;
	p >> tmp;
	team = (Team) tmp;
}

sf::Packet ComponentTeam::dumpComponent()
{
	sf::Packet p;

	p << (uint32_t) team;

	return p;
}
ComponentID ComponentTeam::getId()
{
	return ComponentID::TEAM;
}

}  // namespace tempo
