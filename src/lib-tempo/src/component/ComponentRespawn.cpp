#include <tempo/component/ComponentRespawn.hpp>

namespace tempo
{

ComponentRespawn::ComponentRespawn(glm::ivec2 spawn_location)
{
	this->spawn_location = spawn_location;
}
	
ComponentRespawn::ComponentRespawn(sf::Packet p)
{
	int x, y;
	p >> x >> y;
	spawn_location = glm::ivec2(x,y);
}

ComponentID ComponentRespawn::getId()
{
	return tempo::ComponentID::RESPAWN;
}

sf::Packet ComponentRespawn::dumpComponent()
{
	sf::Packet p;
	p << spawn_location.x << spawn_location.y;
	return p;
}

}
