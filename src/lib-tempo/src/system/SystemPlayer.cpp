#include <tempo/system/SystemPlayer.hpp>
#include<iostream>

#define TELEPORT_LEFT 60
#define TELEPORT_RIGHT 70

#define TELEPORT_EXIT glm::ivec2(20,136)

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
		{
			entity.getComponent<ComponentStagePosition>().setPosition(TELEPORT_EXIT);

			// Send update to everyone
			auto &positions = entity.getComponent<tempo::ComponentStagePosition>().occupied;
			sf::Packet p;
			p << entity.getId();
			p << static_cast<uint32_t>(positions.size());
			for (auto &position : positions) {
				p << position.x << position.y;
			}

			// Add facing direction
			if (entity.hasComponent<tempo::ComponentStageRotation>()) {
				p << entity.getComponent<tempo::ComponentStageRotation>().facing.x;
				p << entity.getComponent<tempo::ComponentStageRotation>().facing.y;
			}

			tempo::broadcastMessage(tempo::QueueID::MOVEMENT_UPDATES, p);
		}

	}
}

}
