#include <tempo/system/SystemHealth.hpp>

#include <tempo/component/ComponentStagePosition.hpp>  //Just temporary

namespace tempo
{
void SystemHealth::CheckHealth()
{
	auto entities = getEntities();

	for (auto &entity : entities) {
		auto &h = entity.getComponent<ComponentHealth>();
		// anax::Entity::Id id = entity.getId();

		// If the entity has 0 health then kill entity
		if ((h.current_health <= 0)) {
			// printf("\nEntity ID: %ld has just been \"killed\". \n", id.index);
			// This got too annoying
			if (entity.hasComponent<ComponentStagePosition>()) {
				entity.getComponent<ComponentStagePosition>().movePosition(
				  glm::ivec2(1000, 1000));  // poof
			}
			// entity.deactivate();
		}
	}
}

void SystemHealth::broadcastHealth()
{
	auto entities = getEntities();

	for (auto &entity : entities)
	{
		auto &h = entity.getComponent<ComponentHealth>();
		anax::Entity::Id id = entity.getId();

		sf::Packet p;
		p << id;
		p << h.current_health;
		broadcastMessage(tempo::QueueID::SYSTEM_HEALTH, p);
	}
}

void SystemHealth::sendHealth(anax::Entity entity)
{
	auto &h = entity.getComponent<ComponentHealth>();
	anax::Entity::Id id = entity.getId();

	sf::Packet p;
	p << id;
	p << h.current_health;
	sendMessage(tempo::QueueID::SYSTEM_HEALTH, p);
}

void SystemHealth::receiveHealth(anax::World &world)
{
	tempo::Queue<sf::Packet> *q = get_system_queue(QueueID::SYSTEM_HEALTH);
	while (!q->empty())
	{
		sf::Packet p = q->front();
		q->pop();

		anax::Entity::Id id;
		p >> id;

		int health;
		p >> health;

		id = servertolocal[id];
		anax::Entity e = anax::Entity(world, id);
		ComponentHealth &h = e.getComponent<ComponentHealth>();
		h.current_health = health;
	}
}

void SystemHealth::receiveHealths(anax::World &world)
{
	tempo::Queue<sf::Packet> *q = get_system_queue(QueueID::SYSTEM_HEALTH);
	while (!q->empty())
	{
		sf::Packet p = q->front();
		q->pop();

		anax::Entity::Id id;
		p >> id;

		int health;
		p >> health;

		id = localtoserver[id];
		anax::Entity e = anax::Entity(world, id);
		ComponentHealth &h = e.getComponent<ComponentHealth>();
		h.current_health = health;
	}
}

}
