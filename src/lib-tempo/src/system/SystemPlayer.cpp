#include <tempo/system/SystemPlayer.hpp>
#include<iostream>

#define TELEPORT_LEFT 30
#define TELEPORT_RIGHT 40

#define TELEPORT_EXIT glm::ivec2(6,137)

namespace tempo
{

anax::Entity SystemPlayer::nearest_player(glm::ivec2 pos)
{
	float min = 9999999;
	anax::Entity out;
	for (auto entity : getEntities())
	{
		glm::vec2 diff = pos - entity.getComponent<ComponentStagePosition>().getOrigin();
		if (length(diff) < min)
		{
			min = length(diff);
			out = entity;
		}

	}

	return out;
}

void SystemPlayer::TeleportPlayers()
{
	for (auto entity : getEntities())
	{
		glm::vec2 pos = entity.getComponent<ComponentStagePosition>().getOrigin();
		if (pos.y > TELEPORT_LEFT && pos.y < TELEPORT_RIGHT)
		entity.getComponent<ComponentStagePosition>().setPosition(TELEPORT_EXIT);

	}
}

}
