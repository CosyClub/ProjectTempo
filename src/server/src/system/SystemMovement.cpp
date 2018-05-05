#include <server/system/SystemMovement.hpp>

#include <tempo/component/ComponentStageRotation.hpp>

#include <tempo/network/ID.hpp>
#include <tempo/network/server.hpp>

namespace server
{
using tempo::operator<<;
using tempo::operator>>;

void SystemMovement::receiveTranslations(anax::World &w)
{
	tempo::Queue<sf::Packet> *queue =
	  tempo::get_system_queue(tempo::QueueID::MOVEMENT_INTENT_UPDATES);

	if (queue->empty())
		return;

	while (!queue->empty()) {
		sf::Packet update = queue->front();
		queue->pop();
		sf::Packet update_broadcast;

		anax::Entity::Id instance_id;
		glm::ivec2       delta = glm::ivec2(0,0);
		tempo::Facing    f     = glm::ivec2(0,0);
		bool moved;
		update >> instance_id >> f.x >> f.y >> delta.x >> delta.y >> moved;
		update_broadcast << instance_id << f.x << f.y << delta.x << delta.y << moved;

		try {
			anax::Entity entity = anax::Entity(w, instance_id);
			if (entity.hasComponent<tempo::ComponentStageRotation>() && f != glm::ivec2(0, 0)) {
				entity.getComponent<tempo::ComponentStageRotation>().facing = f;
			}

			if (entity.hasComponent<tempo::ComponentStageTranslation>()) {
				entity.getComponent<tempo::ComponentStageTranslation>().delta = delta;
				entity.getComponent<tempo::ComponentStageTranslation>().moved = moved; 
			}

			// Send update to all clients
			for (auto it = tempo::clients.begin(); it != tempo::clients.end(); ++it) {
				tempo::sendMessage(tempo::QueueID::MOVEMENT_INTENT_UPDATES, update_broadcast,
				                   it->first);
			}
		} catch (std::out_of_range &e) {
			std::cerr << e.what() << std::endl;
		}
	}
}

void SystemMovement::processTranslation()
{
	for (auto &entity : getEntities()) {
		tempo::ComponentStageTranslation &st =
		  entity.getComponent<tempo::ComponentStageTranslation>();

		tempo::ComponentStage &stage = entity.getComponent<tempo::ComponentStage>();

		auto &positions = entity.getComponent<tempo::ComponentStagePosition>().occupied;
		if (entity.hasComponent<tempo::ComponentHealth>())
		{
			if (entity.getComponent<tempo::ComponentHealth>().current_health <= 0)
			{
				for (auto &position : positions) {
					if (!entity.getComponent<tempo::ComponentStagePosition>().isPhased)
					{
						collisionMap[position] = false;
					}
				}
				continue;
			}
		}
		if (!st.moved) continue;

		bool  can_move  = true;

		for (auto &position : positions) {
			can_move &= stage.existstTile(position + st.delta);
			can_move &= stage.getHeight(position + st.delta) < 5;
			if (collisionMap.find(position + st.delta) == collisionMap.end())
				collisionMap[position + st.delta] = false;
			can_move &= !collisionMap[position + st.delta];
		}

		if (can_move) {
			for (auto &position : positions) {
				if (!entity.getComponent<tempo::ComponentStagePosition>().isPhased)
				{
					collisionMap[position] = false;
					collisionMap[position + st.delta] = true;
				}
				position += st.delta;
			}
		}

		st.delta = {0, 0};
		st.moved = false;

		// Send update to everyone
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

}  // namespace server
