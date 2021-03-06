#include <client/system/SystemAttack.hpp>

#include <tempo/component/ComponentAttack.hpp>
#include <tempo/component/ComponentHealth.hpp>
#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStageTranslation.hpp>

#include <client/network/client.hpp>
#include <tempo/network/ID.hpp>
#include <tempo/network/queue.hpp>

#include <anax/Entity.hpp>

namespace client
{
using tempo::operator<<;
using tempo::operator>>;

void SystemAttack::processServerResponses(anax::World &w)
{
	tempo::Queue<sf::Packet> *q = tempo::get_system_queue(tempo::QueueID::SYSTEM_ATTACK);

	while (!q->empty()) {
		sf::Packet p = q->front();
		q->pop();
		sf::Packet pb(p);  // packet for broadcast

		uint32_t code;
		p >> code;

		anax::Entity::Id id;
		p >> id;  // ID of the entity this message concerns
		SERVERTOLOCAL(id);
		if (id.isNull()) continue;
		anax::Entity e(w, id);

		switch (static_cast<tempo::MessageAttack>(code)) {
		case tempo::MessageAttack::UPDATE_INTENT: {
			if (!e.hasComponent<tempo::ComponentAttack>()) {
				std::cout << "Received Attack Intent Update for entity without ComponentAttack"
				          << std::endl;
				continue;
			}
			tempo::ComponentAttack &c = e.getComponent<tempo::ComponentAttack>();
			p >> c.damage;
			p >> c.beats_until_attack;
			break;
		}
		default: std::cout << "ATTACK: Unhandled/erroneous message" << std::endl;
		}
	}
}

}  // namespace client
