#include <tempo/component/ComponentWeapon.hpp>

namespace tempo
{
ComponentWeapon::ComponentWeapon(Mask &m)
{
	damage                   = Mask(m);
	this->isDelayed          = 0;
	this->beats_until_attack = 0;
}

ComponentWeapon::ComponentWeapon(Mask &m, unsigned int beats_until_attack)
{
	this->damage             = Mask(m);
	this->isDelayed          = true;
	this->beats_until_attack = beats_until_attack;
}

float ComponentWeapon::GetDamage(glm::vec2 offset)
{
	return damage.get_value(offset);
}

ComponentWeapon::ComponentWeapon(sf::Packet p)
{
	p >> damage;
	p >> isDelayed;
	p >> beats_until_attack;
}

sf::Packet ComponentWeapon::dumpComponent()
{
	sf::Packet p;

	p << damage;
	p << isDelayed;
	p << beats_until_attack;

	return p;
}
ComponentID ComponentWeapon::getId()
{
	return ComponentID::WEAPON;
}
}
