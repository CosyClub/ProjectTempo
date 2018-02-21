#ifndef TEMPO_SYSTEM_SYSTEMATTACK_HPP
#define TEMPO_SYSTEM_SYSTEMATTACK_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/component/ComponentWeapon.hpp>

namespace tempo{

struct SystemAttack : anax::System<anax::Requires<ComponentWeapon>> {

	SystemAttack();
	~SystemAttack();
};

}

#endif
