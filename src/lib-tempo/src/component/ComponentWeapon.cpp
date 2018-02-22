#include <tempo/component/ComponentWeapon.hpp>

namespace tempo
{

ComponentWeapon::ComponentWeapon(Mask& m)
{
	damage = Mask(m);
}

float ComponentWeapon::GetDamage(glm::vec2 offset)
{
	return damage.get_value(offset);
}

ComponentWeapon::ComponentWeapon(sf::Packet p)
{
	p >> damage;
}

sf::Packet ComponentWeapon::dumpComponent()
{
	sf::Packet p;

	p << damage;

	return p;
}
ComponentID ComponentWeapon::getId()
{
	return ComponentID::WEAPON;
}

}
