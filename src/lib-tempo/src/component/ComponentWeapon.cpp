#include <tempo/component/ComponentWeapon.hpp>

namespace tempo
{

ComponentWeapon::ComponentWeapon(Mask& m)
{
	damage = Mask(m);
	this->isDelayed = 0;
	this->delay = sf::Time::Zero;
}
 
ComponentWeapon::ComponentWeapon(Mask& m, sf::Time delay)
{
	this->damage = Mask(m);
	this->isDelayed = true;
	this->delay = delay;
}

float ComponentWeapon::GetDamage(glm::vec2 offset)
{
	return damage.get_value(offset);
}

ComponentWeapon::ComponentWeapon(sf::Packet p)
{
	p >> damage;
	p >> isDelayed;
	p >> delay;
}

sf::Packet ComponentWeapon::dumpComponent()
{
	sf::Packet p;

	p << damage;
	p << isDelayed;
	p << delay;

	return p;
}
ComponentID ComponentWeapon::getId()
{
	return ComponentID::WEAPON;
}

}
