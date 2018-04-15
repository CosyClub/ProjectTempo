#include <tempo/system/SystemHealth.hpp>

#include <tempo/component/ComponentStagePosition.hpp>  //Just temporary
#include <tempo/component/ComponentCombo.hpp>

namespace tempo
{

void SystemHealth::check_health()
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

void SystemHealth::regenerate()
{
	auto entities = getEntities();

	for (auto &entity : entities)
	{
		if (!entity.hasComponent<tempo::ComponentHealth>() ||
		    !entity.hasComponent<tempo::ComponentCombo>()) {

			continue;
		}

		auto &h = entity.getComponent<tempo::ComponentHealth>();
		auto &c = entity.getComponent<tempo::ComponentCombo>();

		if( c.comboCounter > 20) {
			h.HealthUpdate(1);
		}
	}
}

void SystemHealth::broadcastHealth()
{
	auto entities = getEntities();

	for (auto &entity : entities)
	{
		auto &h = entity.getComponent<ComponentHealth>();

		// Lets try not to send health all the time please
		if (h.current_health != h.__prev_health) {
			h.__prev_health = h.current_health;
		} else {
			continue;
		}

		anax::Entity::Id id = entity.getId();

		sf::Packet p;
		p << id;
		p << h.current_health;
		broadcastMessage(tempo::QueueID::SYSTEM_HEALTH, p);
	}
}

void SystemHealth::client_receiveHealth(anax::World &world)
{
	tempo::Queue<sf::Packet> *q = get_system_queue(QueueID::SYSTEM_HEALTH);
	while (!q->empty())
	{
		sf::Packet p = q->front();
		q->pop();

		anax::Entity::Id id;
		p >> id;
		SERVERTOLOCAL(id)
		if (id.isNull()) continue;
		anax::Entity e = anax::Entity(world, id);
		ComponentHealth &h = e.getComponent<ComponentHealth>();

		p >> h.current_health;
	}
}

/////////////////////////////////
//// DEPRECATED CODE GRAVEYARD
//// There's a chance someone may need something from here so I haven't deleted
//// it entirely, just removed it from sight and mind.

// void SystemHealth::client_sendHealth(anax::Entity entity)
// {
// 	auto &h = entity.getComponent<ComponentHealth>();

// 	sf::Packet p;
// 	p << tempo::localtoserver[entity.getId()];
// 	p << h.current_health;
// 	sendMessage(tempo::QueueID::SYSTEM_HEALTH, p);
// }

// void SystemHealth::server_sendHealth(anax::Entity entity)
// {
// 	auto &h = entity.getComponent<ComponentHealth>();
// 	anax::Entity::Id id = entity.getId();

// 	sf::Packet p;
// 	p << id;
// 	p << h.current_health;
// 	sendMessage(tempo::QueueID::SYSTEM_HEALTH, p);
// }

//void SystemHealth::server_receiveHealth(anax::World &world)
//{
//	tempo::Queue<sf::Packet> *q = get_system_queue(QueueID::SYSTEM_HEALTH);
//	while (!q->empty())
//	{
//		sf::Packet p = q->front();
//		q->pop();

//		anax::Entity::Id id;
//		p >> id;

//		int health;
//		p >> health;

//		ERROR: THIS SHOULD BE CHANGED TO THE NEW LOCALTOSERVER MACRO IF
//		EVER UNCOMMENTED AND USED AGAIN:
//		id = localtoserver[id];
//		anax::Entity e(world, id);
//		if (!e.hasComponent<tempo::ComponentHealth>()) {
//			std::cout << "Received Health from entity without ComponentHealth"
//								<< std::endl;
//		} else {
//		ComponentHealth &h = e.getComponent<ComponentHealth>();
//		h.current_health = health;
//		}
//	}
//}

}
