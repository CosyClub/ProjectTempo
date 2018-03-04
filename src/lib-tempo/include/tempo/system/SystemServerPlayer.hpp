#ifndef TEMPO_SYSTEM_SERVER_PLAYER_HPP
#define TEMPO_SYSTEM_SERVER_PLAYER_HPP

#include <anax/Component.hpp>
#include <anax/System.hpp>

#include <tempo/component/ComponentCombo.hpp>
#include <tempo/component/ComponentPlayerRemote.hpp>
#include <tempo/component/ComponentStageTranslation.hpp>
#include <tempo/network/ID.hpp>
#include <tempo/time.hpp>

namespace tempo
{
//
struct SystemServerPlayer
    : anax::System<
        anax::Requires<ComponentCombo, ComponentStageTranslation, ComponentPlayerRemote>> {
	tempo::Clock &clock;

	inline SystemServerPlayer(tempo::Clock &c)
	    : clock(c)
	{
	}

	void update(anax::World &world);
};

}  // namespace tempo

#endif
