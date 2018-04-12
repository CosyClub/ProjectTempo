#ifndef TEMPO_SYSTEM_TRIGGER_HPP
#define TEMPO_SYSTEM_TRIGGER_HPP

#include <anax/Component.hpp>
#include <anax/System.hpp>
#include <anax/World.hpp>

#include <glm/vec2.hpp>

#include <tempo/component/ComponentButtonGroup.hpp>
#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentSpikes.hpp>


namespace tempo
{

class SubSystemGetPlayers : public anax::System<anax::Requires<tempo::ComponentStagePosition>>
{
   public:
	std::vector<glm::ivec2> getPlayers();
};

class SubSystemUpdateSpikes : public anax::System<anax::Requires<tempo::ComponentSpikes>>
{
public:
	void updateSpikes(std::vector<glm::ivec2> untriggerPos);
};

class SystemTrigger : public anax::System<anax::Requires<tempo::ComponentButtonGroup>>
{
   public:
	std::vector<glm::ivec2> untriggerPos; // track which spikes need to drop
	std::vector<glm::ivec2> playerPos; // track current player positions
	SystemTrigger(anax::World &world);
	void updateButtons(anax::World &world);

	void resetButtons(int rhythmID); // make the first button triggerable and all others not
	void blockButtons(int rhythmID); // make all buttons untriggerable, until players get off buttons

   private:
	SubSystemGetPlayers subSystemPlayers;
	SubSystemUpdateSpikes  subSystemSpikes;
};
}

#endif
