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

std::vector<glm::ivec2> ComponentAttack::getAbsolutePositions(glm::ivec2 pos, Facing rot)
{	
	std::vector<glm::ivec2> ps;
	
	glm::ivec2 forward = rot;
	glm::ivec2 left = glm::ivec2(-rot.y, -rot.x);  // Hacky cross product

	for (glm::ivec2 p : damage.positions) {
		p -= damage.ctr;
		ps.push_back(pos + ((p.x * left) + (p.y * forward)));
	}

	return ps;
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
