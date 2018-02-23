#ifndef CLIENT_SYSTEM_SYSTEMATTACK_HPP
#define CLIENT_SYSTEM_SYSTEMATTACK_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/component/ComponentHealth.hpp>
#include <tempo/component/ComponentAOEIndicator.hpp>
#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/component/ComponentWeapon.hpp>

#include <tempo/network/QueueID.hpp>
#include <tempo/network/base.hpp>
#include <tempo/mask.hpp>

#include <glm/glm.hpp>
#include <cassert>

namespace tempo{

struct SystemAttack : anax::System<anax::Requires<ComponentHealth,
                                                  ComponentStagePosition>> {

	// Attack
	// If attack is made, all attackable entities (have ComonentHealth)
	// are checked to see if they are inside the attack range
	//
	// Arguments:
	//          anax::Entity attacker - The entity that has chosen to attack
	// Returns:
	//          void
	void Attack(anax::Entity attacker);

	void Broadcast(anax::World& w);
	void Recieve(anax::World& w);

	enum Messages {
		ATTACK,
		DELAYED_ATTACK,
	};
};

}

#endif
