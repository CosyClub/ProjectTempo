#include <tempo/component/ComponentPickup.hpp>

//TODO Is this a placeholder for the future or should it be removed?

namespace tempo
{
	
/////
// Required for networking
/////
ComponentPickup::ComponentPickup(sf::Packet p)
{
	return; // does nothing
}

ComponentID ComponentPickup::getId()
{
	return ComponentID::PICKUP;
}

sf::Packet ComponentPickup::dumpComponent()
{
	return sf::Packet(); // does nothing
}

}
