#ifndef TEMPO_SYSTEM_SYSTEMATTACK_HPP
#define TEMPO_SYSTEM_SYSTEMATTACK_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/component/ComponentHealth.hpp>

namespace tempo{

struct SystemAttack : anax::System<anax::Requires<ComponentHealth>> {

	// Attack
	// If attack is made, all attackable entities (have ComonentHealth)
	// are checked to see if they are inside the attack range
	//
	// Arguments:
	//          anax::Entity attacker - The entity that has chosen to attack
	// Returns:
	//          void
	void Attack(anax::Entity attacker);
};

}

#endif
