#include <client/system/SystemPlayer.hpp>

#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/component/ComponentStageTranslation.hpp>

#include <client/network/client.hpp>
#include <tempo/network/ID.hpp>
#include <tempo/network/queue.hpp>

namespace client
{

using tempo::operator<<;
using tempo::operator>>;

void SystemPlayer::processServerResponses(anax::World &world)
{
	tempo::Queue<sf::Packet> *queue = get_system_queue(tempo::QueueID::PLAYER_UPDATES);

	if (queue->empty())
		return;

	while (!queue->empty()) {
		sf::Packet update = queue->front();
		queue->pop();

		anax::Entity::Id instance_id;
		glm::ivec2 delta(0,0);
		glm::ivec2 facing(0,0);
		update >> instance_id >> facing.x >> facing.y >> delta.x >> delta.y;
		anax::Entity entity = anax::Entity(world, tempo::servertolocal[instance_id]);

		if (entity.hasComponent<tempo::ComponentStageRotation>()) {
			entity.getComponent<tempo::ComponentStageRotation>().facing = facing;
		}
		if (entity.hasComponent<tempo::ComponentStageTranslation>()) {
			entity.getComponent<tempo::ComponentStageTranslation>().delta = delta;
		}
	}
}

}  // namespace client
