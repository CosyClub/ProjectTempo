#ifndef TEMPO_SYSTEM_SYSTEMATTACK_HPP
#define TEMPO_SYSTEM_SYSTEMATTACK_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/component/ComponentHealth.hpp>

namespace tempo{

struct SystemAttack : anax::System<anax::Requires<ComponentHealth>> {

	SystemAttack();
	~SystemAttack();
	void Attack(anax::Entity attacker);
};

}

#endif
