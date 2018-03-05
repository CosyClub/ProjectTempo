#include <server/system/SystemPlayer.hpp>

#include <tempo/component/ComponentStageTranslation.hpp>
#include <tempo/component/ComponentStageRotation.hpp>

#include <tempo/network/base.hpp>
#include <tempo/network/ID.hpp>
#include <tempo/network/queue.hpp>
#include <tempo/network/server.hpp>

#include <anax/Entity.hpp>

namespace server
{

using tempo::operator<<;
using tempo::operator>>;

void SystemPlayer::recieveTranslations(anax::World &w)
{
	tempo::Queue<sf::Packet> *queue = tempo::get_system_queue(tempo::QueueID::PLAYER_UPDATES);

	if (queue->empty())
		return;

	while (!queue->empty()) {
		sf::Packet update = queue->front();
		queue->pop();
		sf::Packet update_broadcast;

		anax::Entity::Id instance_id;
		glm::ivec2       delta = glm::ivec2(0,0);
		tempo::Facing    f     = glm::ivec2(0,0);
		update >> instance_id >> f.x >> f.y >> delta.x >> delta.y;
		update_broadcast << instance_id << f.x << f.y << delta.x << delta.y;

		try {
			anax::Entity entity = anax::Entity(w, instance_id);
			if (entity.hasComponent<tempo::ComponentStageRotation>() && f != glm::ivec2(0,0)) {
				entity.getComponent<tempo::ComponentStageRotation>().facing = f;
			}

			if (entity.hasComponent<tempo::ComponentStagePosition>()) {
				entity.getComponent<tempo::ComponentStageTranslation>().delta = delta;
			}

			// Send update to all clients
			for (auto it = tempo::clients.begin(); it != tempo::clients.end(); ++it) {
				tempo::sendMessage(tempo::QueueID::PLAYER_UPDATES, update_broadcast, it->first);
			}
		} catch (std::out_of_range &e) {
			std::cerr << e.what() << std::endl;
		}
	}
}

}  // namespace server
