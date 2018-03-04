#ifndef TEMPO_COMPONENT_COMPONENT_ATTACK_HPP
#define TEMPO_COMPONENT_COMPONENT_ATTACK_HPP

#include <anax/Component.hpp>

#include <tempo/mask.hpp>

#include <tempo/component/NetworkedComponent.hpp>

namespace tempo
{

// Component captures a combo mechanic for the entity with this component.
struct ComponentAttack
    : anax::Component
    , NetworkedComponent {
	Mask damage;                      // Damage mask for the attack
	unsigned int beats_until_attack;  // Time until the attack

	// Creates a new component combo with no combo
	ComponentAttack(Mask &damage, unsigned int beats_until_attack);

	/////
	// Required for networking
	/////
	ComponentAttack(sf::Packet p);
	ComponentID getId();
	sf::Packet  dumpComponent();
};

}  // namespace tempo

#endif
