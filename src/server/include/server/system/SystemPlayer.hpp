#ifndef SERVER_SYSTEM_PLAYER_HPP
#define SERVER_SYSTEM_PLAYER_HPP

#include <anax/Component.hpp>
#include <anax/System.hpp>

#include <tempo/component/ComponentCombo.hpp>
#include <tempo/component/ComponentPlayerRemote.hpp>
#include <tempo/component/ComponentStageTranslation.hpp>
#include <tempo/time.hpp>

namespace server
{

struct SystemPlayer
    : anax::System<
        anax::Requires<tempo::ComponentCombo, tempo::ComponentStageTranslation, tempo::ComponentPlayerRemote>> {
	tempo::Clock &clock;

	inline SystemPlayer(tempo::Clock &c)
	    : clock(c)
	{
	}

	void update(anax::World &world);
};

}  // namespace server

#endif
