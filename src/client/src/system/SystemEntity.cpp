#include <client/system/SystemEntity.hpp>

#include <tempo/component/ComponentAttack.hpp>
#include <tempo/component/ComponentHealth.hpp>

#include <tempo/entity/EntityCreation.hpp>

#include <client/network/client.hpp>
#include <tempo/network/ID.hpp>
#include <tempo/network/queue.hpp>

#include <anax/Entity.hpp>

namespace client
{
using tempo::operator<<;
using tempo::operator>>;

bool SystemEntity::creationCheck(anax::World &w)
{
	tempo::Queue<sf::Packet> *q = get_system_queue(tempo::QueueID::ENTITY_CREATION);
	bool flag = !q->empty();
	while (!q->empty()) {
		sf::Packet p = q->front();
		q->pop();
		tempo::addComponent(w, p);
	}
	w.refresh();
	return flag;
}

void SystemEntity::deletionCheck(anax::World &w)
{
	tempo::Queue<sf::Packet> *q = get_system_queue(tempo::QueueID::ENTITY_DELETION);
	while (!q->empty()) {
		sf::Packet p = q->front();
		q->pop();

		anax::Entity::Id id, ids;
		p >> id;

		// Not using macro as this requires special case stuff
		std::map<anax::Entity::Id, anax::Entity::Id>::iterator it = tempo::servertolocal.find(id);
		if (it != tempo::servertolocal.end()) {
			ids = tempo::servertolocal[id];
		} else {
			ids = anax::Entity::Id();
		}
		if (ids.isNull()) continue;

		tempo::servertolocal.erase(id);
		tempo::localtoserver.erase(ids);
		anax::Entity e(w, ids);
		w.killEntity(e);
	}
	w.refresh();
}

}  // namespace client
