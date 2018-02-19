#ifndef TEMPO_SYSTEM_PLAYER_HPP
#define TEMPO_SYSTEM_PLAYER_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/component/ComponentCombo.hpp>
#include <tempo/component/ComponentPlayerRemote.hpp>
#include <tempo/component/ComponentStageTranslation.hpp>
#include <tempo/time.hpp>

namespace tempo
{

struct SystemPlayer : anax::System<anax::Requires<ComponentCombo,
                                                  ComponentStageTranslation, 
                                                  ComponentPlayerRemote>> 
{
	tempo::Clock& clock;

	inline SystemPlayer(tempo::Clock& c) : clock(c) {}

	void update(anax::Entity::Id player_id);
};

} // namespace tempo

#endif
