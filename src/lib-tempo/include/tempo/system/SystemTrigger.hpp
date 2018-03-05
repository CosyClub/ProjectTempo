#ifndef TEMPO_SYSTEM_TRIGGER_HPP
#define TEMPO_SYSTEM_TRIGGER_HPP

#include <anax/Component.hpp>
#include <anax/System.hpp>
#include <anax/World.hpp>

#include <glm/vec2.hpp>

#include <tempo/component/ComponentButtonGroup.hpp>
#include <tempo/component/ComponentStagePosition.hpp>

namespace tempo
{
class SubSystemGetPlayers : public anax::System<anax::Requires<tempo::ComponentStagePosition>>
{
   public:
	std::vector<glm::ivec2> getPlayers();
};

class SystemTrigger : public anax::System<anax::Requires<tempo::ComponentButtonGroup>>
{
   public:
	std::vector<glm::ivec2> playerPos;
	SystemTrigger(anax::World &world);
	void updateButtons();

   private:
	SubSystemGetPlayers subSystem;
};
}

#endif
