#ifndef TEMPO_COMPONENT_COMPONENT_ATTACK_HPP
#define TEMPO_COMPONENT_COMPONENT_ATTACK_HPP

#include <anax/Component.hpp>

#include <tempo/mask.hpp>

#include <tempo/component/NetworkedComponent.hpp>

namespace tempo
{

// Component captures a combo mechanic for the entity with this component.
struct ComponentAttack : anax::Component, NetworkedComponent 
{
	// Damage mask for the attack
	Mask damage;
	// Time until the attack (-1
	int beats_until_attack;

	// Creates a new component attack
	ComponentAttack();

	// Adds and attack to the ComponentAttack
	void updateAttack(Mask &damage, unsigned int beats_until_attack);

	// Returns whether or not there is an intent to attack
	bool isAttacking();

	/////
	// Required for networking
	/////
	ComponentAttack(sf::Packet p);
	ComponentID getId();
	sf::Packet  dumpComponent();
};

}  // namespace tempo

#endif
