#include <tempo/component/ComponentAttack.hpp>

namespace tempo
{

// Creates a new component combo with no combo
ComponentAttack::ComponentAttack(Mask &damage, unsigned int beats_until_attack) 
{
	this->damage = damage;
	this->beats_until_attack = beats_until_attack;
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

} // namespace tempo
