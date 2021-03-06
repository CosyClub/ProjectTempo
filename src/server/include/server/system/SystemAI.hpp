#ifndef SERVER_SYSTEM_AI_HPP
#define SERVER_SYSTEM_AI_HPP
#include <tempo/component/ComponentAI.hpp>
#include <tempo/component/ComponentPlayerRemote.hpp>
#include <tempo/component/ComponentStage.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/component/ComponentStageTranslation.hpp>
#include <tempo/component/ComponentWeapon.hpp>
#include <server/system/SystemAttack.hpp>
#include <tempo/system/SystemPlayer.hpp>

#include <anax/System.hpp>
#include <anax/World.hpp>

namespace server
{
struct SystemAI : anax::System<anax::Requires<tempo::ComponentStageTranslation,
                                              tempo::ComponentAI>> {
	void update(anax::World& world, tempo::SystemPlayer system_player, server::SystemAttack s_attack);
};
}

#endif
