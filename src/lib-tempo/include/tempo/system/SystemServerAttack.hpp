#ifndef TEMPO_SYSTEM_SYSTEMSERVERATTACK_HPP
#define TEMPO_SYSTEM_SYSTEMSERVERATTACK_HPP

#include <anax/Component.hpp>
#include <anax/System.hpp>

#include <tempo/component/ComponentAOEIndicator.hpp>
#include <tempo/component/ComponentHealth.hpp>
#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/component/ComponentWeapon.hpp>
#include <tempo/mask.hpp>

#include <tempo/network/QueueID.hpp>
#include <tempo/network/server.hpp>

#include <cassert>
#include <glm/glm.hpp>

namespace tempo
{
struct SystemServerAttack : anax::System<anax::Requires<ComponentHealth, ComponentStagePosition>> {
	void Sync(anax::World &w);
};
}

#endif
