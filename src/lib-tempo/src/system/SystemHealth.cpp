#include <tempo/system/SystemHealth.hpp>

#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/component/ComponentCombo.hpp>
#include <tempo/component/ComponentParty.hpp>
#include <tempo/component/ComponentRespawn.hpp>

#include <tempo/constants.hpp>

namespace tempo
{

void SystemHealth::check_health()
{
	auto& entities = getEntities();

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
				glm::ivec2 spawn_loc = entity.getComponent<ComponentRespawn>().spawn_location;

				if (entity.hasComponent<ComponentStagePosition>()) {
					entity.getComponent<ComponentStagePosition>().setPosition(spawn_loc);
					h.current_health = h.max_health;
		
					// Tell everyone they have moved to a respawn position
					auto &positions = entity.getComponent<tempo::ComponentStagePosition>().occupied;
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

					// Tell everyone they have broken combo
					entity.getComponent<tempo::ComponentCombo>().zeroCombo();
					p = sf::Packet();
					p << entity.getId() << tempo::MessageCombo::ZERO_COMBO;
					tempo::broadcastMessage(tempo::QueueID::COMBO_UPDATES, p);
				}
			}
			else
			{
				if (entity.hasComponent<ComponentStagePosition>()) {
					entity.getComponent<ComponentStagePosition>().setPosition(
					  glm::ivec2(1000, 1000));  // poof
				}
			}
			// entity.deactivate();
		}
	}
}

void SystemHealth::broadcastHealth()
{
	auto& entities = getEntities();

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
	auto& entities = getEntities();

	for (auto &entity : entities)
	{
		if (!entity.hasComponent<tempo::ComponentHealth>() ||
	      !entity.hasComponent<tempo::ComponentCombo>()) {

			continue;
		}

		auto &h = entity.getComponent<tempo::ComponentHealth>();
		auto &c = entity.getComponent<tempo::ComponentCombo>();

		// more healing for everyone!
		// h.HealthUpdate(PLAYER_MAX_HEALTH / 10) = n beats to heal
		if (c.comboCounter >= 200) {
			h.HealthUpdate(PLAYER_MAX_HEALTH / 25);
		}
		else if (c.comboCounter >= 120) {
			h.HealthUpdate(PLAYER_MAX_HEALTH / 30);
		}
		else if (c.comboCounter >= 60) {
			h.HealthUpdate(PLAYER_MAX_HEALTH / 40);
		}
		else if (c.comboCounter >= 40) {
			h.HealthUpdate(PLAYER_MAX_HEALTH / 55);
		}
		else if (c.comboCounter >= 20) {
			h.HealthUpdate(PLAYER_MAX_HEALTH / 70);
		}
		else if (c.comboCounter >= 15) {
			h.HealthUpdate(PLAYER_MAX_HEALTH / 80);
		}
		else if (c.comboCounter >= 10) {
			h.HealthUpdate(PLAYER_MAX_HEALTH / 95);
		} 
	}
}

}
