#include <client/system/SystemMovement.hpp>

#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/component/ComponentStageTranslation.hpp>

#include <client/network/client.hpp>
#include <tempo/network/queue.hpp>
#include <tempo/network/ID.hpp>

#include <anax/Entity.hpp>

namespace client
{

using tempo::operator<<;
using tempo::operator>>;

void SystemMovement::processServerResponses(anax::World &w) 
{
	tempo::Queue<sf::Packet> *q = tempo::get_system_queue(tempo::QueueID::MOVEMENT_UPDATES);

	while (!q->empty()) {
		sf::Packet p = q->front();
		q->pop();	
		sf::Packet pb(p); // packet for broadcast

		uint32_t code;
		p >> code;

		anax::Entity::Id id;
		p >> id; // ID of the entity this message concerns
		anax::Entity e(w, tempo::servertolocal[id]);

		std::cout << "ID: " << id.index << ", SID: " << tempo::servertolocal[id].index << std::endl; 
		
		// Update Occupied Position
		if (e.hasComponent<tempo::ComponentStagePosition>()) {
			tempo::ComponentStagePosition& s = e.getComponent<tempo::ComponentStagePosition>();
			std::vector<glm::ivec2>& occ = s.occupied;
			glm::ivec2 o;
			occ.clear();
			int occs = 0;
			for (int i = 0; i < occs; i++) {
				p >> o;
				occ.push_back(o);
			}

		}
		
		// Update Rotation Direction
		if (e.hasComponent<tempo::ComponentStageRotation>()) {
			tempo::ComponentStageRotation& r = e.getComponent<tempo::ComponentStageRotation>();
			p >> r.facing.x;
			p >> r.facing.y;
		}

		// Clear Stage Translation
		if (e.hasComponent<tempo::ComponentStageTranslation>()) {
			e.getComponent<tempo::ComponentStageTranslation>().delta = glm::ivec2(0,0);
		}
	}
}

} // namespace client
