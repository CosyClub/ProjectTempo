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
	std::vector<glm::ivec2> untriggerPos;
	std::vector<glm::ivec2> playerPos;
	SystemTrigger(anax::World &world);
	void updateButtons(anax::World &world);
	void resetButtons(int rhythmID);
	void blockButtons(int rhythmID);

   private:
	SubSystemGetPlayers subSystemPlayers;
	SubSystemUpdateSpikes  subSystemSpikes;
};
}

#endif
