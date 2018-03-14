#include <client/system/SystemMovement.hpp>

#include <client/component/ComponentRenderSceneNode.hpp>
#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/component/ComponentStageTranslation.hpp>

#include <client/network/client.hpp>
#include <tempo/network/ID.hpp>
#include <tempo/network/queue.hpp>

#include <anax/Entity.hpp>

namespace client
{
using tempo::operator<<;
using tempo::operator>>;

void SystemMovement::processIntents(anax::World &world)
{
	tempo::Queue<sf::Packet> *queue = get_system_queue(tempo::QueueID::MOVEMENT_INTENT_UPDATES);

	if (queue->empty())
		return;

	while (!queue->empty()) {
		sf::Packet update = queue->front();
		queue->pop();

		anax::Entity::Id instance_id;
		glm::ivec2 delta(0,0);
		glm::ivec2 facing(0,0);
		bool moved;
		update >> instance_id >> facing.x >> facing.y >> delta.x >> delta.y >> moved;
		anax::Entity entity = anax::Entity(world, tempo::servertolocal[instance_id]);

		if (entity.hasComponent<tempo::ComponentStageRotation>()) {
			entity.getComponent<tempo::ComponentStageRotation>().facing = facing;
		}
		if (entity.hasComponent<tempo::ComponentStageTranslation>()) {
			entity.getComponent<tempo::ComponentStageTranslation>().delta = delta;
			entity.getComponent<tempo::ComponentStageTranslation>().moved = moved;
		}
	}
}

void SystemMovement::processCorrections(anax::World &world)
{
	tempo::Queue<sf::Packet> *q = tempo::get_system_queue(tempo::QueueID::MOVEMENT_UPDATES);

	while (!q->empty()) {
		sf::Packet p = q->front();
		q->pop();
		sf::Packet pb(p);  // packet for broadcast

		anax::Entity::Id id;
		p >> id;  // ID of the entity this message concerns
		anax::Entity e(world, tempo::servertolocal[id]);

		// Update Occupied Position
		if (e.hasComponent<tempo::ComponentStagePosition>()) {
			tempo::ComponentStagePosition &s   = e.getComponent<tempo::ComponentStagePosition>();
			std::vector<glm::ivec2> &      occ = s.occupied;
			glm::ivec2                     o;
			occ.clear();
			int occs = 0;
			p >> occs;
			for (int i = 0; i < occs; i++) {
				p >> o.x >> o.y;
				occ.push_back(o);
			}
		}

		// Update Rotation Direction
		if (e.hasComponent<tempo::ComponentStageRotation>()) {
			tempo::ComponentStageRotation &r = e.getComponent<tempo::ComponentStageRotation>();
			p >> r.facing.x;
			p >> r.facing.y;
		}

		// Clear Stage Translation
		if (e.hasComponent<tempo::ComponentStageTranslation>()) {
			e.getComponent<tempo::ComponentStageTranslation>().delta = glm::ivec2(0,0);
			e.getComponent<tempo::ComponentStageTranslation>().moved = false;
		}

		if (e.hasComponent<client::ComponentRenderSceneNode>()) {
			e.getComponent<client::ComponentRenderSceneNode>().updateNeeded = true;
		}
	}
}

}  // namespace client
