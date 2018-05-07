#ifndef TEMPO_SYSTEM_SYSTEMPLAYER_HPP
#define TEMPO_SYSTEM_SYSTEMPLAYER_HPP

#include <anax/System.hpp>

#include <tempo/component/ComponentPlayerRemote.hpp>
#include <tempo/component/ComponentStagePosition.hpp>
#include <glm/vec2.hpp>
#include <glm/glm.hpp>

namespace tempo
{
struct SystemPlayer : anax::System<anax::Requires<ComponentPlayerRemote>> {
	anax::Entity nearest_player(glm::ivec2 pos);
	void TeleportPlayers();

};
}

#endif
