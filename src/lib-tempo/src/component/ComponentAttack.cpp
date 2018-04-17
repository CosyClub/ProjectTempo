#include <tempo/component/ComponentAttack.hpp>

namespace tempo
{
ComponentAttack::ComponentAttack()
{
	damage             = Mask();
	beats_until_attack = -1;
}

void ComponentAttack::updateAttack(Mask &damage, unsigned int beats_until_attack)
{
	this->damage             = damage;
	this->beats_until_attack = beats_until_attack;
}

bool ComponentAttack::isAttacking()
{
	return beats_until_attack != -1;
}

/////
// Required for networking
/////
ComponentAttack::ComponentAttack(sf::Packet p)
{
	p >> damage;
	p >> beats_until_attack;
}

ComponentID ComponentAttack::getId()
{
	return tempo::ComponentID::ATTACK;
}

sf::Packet ComponentAttack::dumpComponent()
{
	sf::Packet p;
	p << damage;
	p << beats_until_attack;
	return p;
}

}  // namespace tempo
