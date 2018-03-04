#ifndef SERVER_SYSTEM_SYSTEM_ATTACK_HPP
#define SERVER_SYSTEM_SYSTEM_ATTACK_HPP

#include <anax/System.hpp>

#include <tempo/component/ComponentHealth.hpp>
#include <tempo/component/ComponentStagePosition.hpp>

namespace server
{

struct SystemAttack : anax::System<anax::Requires<tempo::ComponentHealth, tempo::ComponentStagePosition>> {
	void Sync(anax::World &w);
};

} // namespace server

#endif
