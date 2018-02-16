#ifndef TEMPO_SYSTEM_SYSTEMATTACK_HPP
#define TEMPO_SYSTEM_SYSTEMATTACK_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/entity/weapon/weapon.hpp>

namespace tempo{

struct SystemAttack : anax::System<anax::Requires<ComponentWeapon>> {

	SystemAttack();
	~SystemAttack();

	void onEntityAdded(anax::Entity& e);
	void onEntityRemoved(anax::Entity& e);
};

}

#endif
