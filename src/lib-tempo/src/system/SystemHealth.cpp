#include <tempo/system/SystemHealth.hpp>

#include <tempo/component/ComponentStagePosition.hpp>  //Just temporary
#include <tempo/component/ComponentCombo.hpp>
#include <tempo/component/ComponentParty.hpp>

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
			if (entity.hasComponent<ComponentPlayerRemote>() ||
			    entity.hasComponent<ComponentPlayerLocal>())
			{

				int party_number = 0;

				int emptySpace = 40;

				int fheight = 69 + emptySpace;

				if (entity.hasComponent<ComponentParty>()) {
					party_number = entity.getComponent<ComponentParty>().party_number;
				}

				if (entity.hasComponent<ComponentStagePosition>()) {
					entity.getComponent<ComponentStagePosition>().setPosition(
					  glm::ivec2(10 + (party_number* fheight), 0));  // poof
					h.current_health = h.max_health;
				}
			}
			else
			{
				if (entity.hasComponent<ComponentStagePosition>()) {
					entity.getComponent<ComponentStagePosition>().setPosition(
					  glm::ivec2(1000, 1000));  // poof
					h.current_health = h.max_health;
				}
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
			h.HealthUpdate(2);
		}
	}
}

}
