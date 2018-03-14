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

void SystemEntity::creationCheck(anax::World &w)
{
	tempo::Queue<sf::Packet> *q = get_system_queue(tempo::QueueID::ENTITY_CREATION);
	while (!q->empty()) {
		sf::Packet p = q->front();
		q->pop();
		tempo::addComponent(w, p);
	}
	w.refresh();
}

void SystemEntity::deletionCheck(anax::World &w)
{
	tempo::Queue<sf::Packet> *q = get_system_queue(tempo::QueueID::ENTITY_DELETION);
	while (!q->empty()) {
		std::cout << "DELETED AN ENTITY\n";
		sf::Packet p = q->front();
		q->pop();
		anax::Entity::Id id;
		p >> id;
		anax::Entity e(w, tempo::servertolocal[id]);
		w.killEntity(e);
	}
	w.refresh();
}

}  // namespace client
