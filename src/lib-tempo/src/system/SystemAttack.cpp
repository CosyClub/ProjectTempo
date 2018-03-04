#include <tempo/system/SystemAttack.hpp>

#include <tempo/component/ComponentAOEIndicator.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/component/ComponentWeapon.hpp>

#include <tempo/mask.hpp>
#include <tempo/network/ID.hpp>
#include <tempo/network/base.hpp>

#include <glm/glm.hpp>

namespace tempo
{

SystemAttack::SystemAttack(anax::World &world)
{
	subSystem = SubSystemAttack();
	world.addSystem(subSystem);
	world.refresh();
}
	
void SystemAttack::processAttacks()
{
	for (auto &entity : getEntities()) {
		if (entity.getComponent<ComponentAttack>().isAttacking()) {
			subSystem.Attack(entity);
		}
	}
}

void SubSystemAttack::Attack(anax::Entity attacker)
{
	// Attacker
	glm::ivec2 attackerpos = attacker.getComponent<tempo::ComponentStagePosition>().getOrigin();
	glm::ivec2 rot         = attacker.getComponent<tempo::ComponentStageRotation>().facing;
	auto &     weapon      = attacker.getComponent<tempo::ComponentWeapon>();
	Mask       m           = weapon.damage;

	if (weapon.isDelayed) {
	}

	for (auto &entity : getEntities()) {
		// TODO some team system

		glm::ivec2 pos    = entity.getComponent<tempo::ComponentStagePosition>().getOrigin();
		auto &     health = entity.getComponent<tempo::ComponentHealth>();

		glm::vec2 forward = rot;
		glm::vec2 left = glm::ivec2(-rot.y, -rot.x);  // Hacky cross product

		glm::vec2  diff          = pos - attackerpos;
		glm::ivec2 relative_diff = glm::ivec2(glm::dot(diff, left), glm::dot(diff, forward));

		float damage = weapon.GetDamage(relative_diff);
		if (damage != 0) {
			std::cout << "hit entity " << entity.getId().index << " for " << damage << std::endl;
		}
		health.HealthUpdate(-1 * damage);
	}
}

void SubSystemAttack::Broadcast(anax::World &w)
{
	tempo::Queue<sf::Packet> *q = get_system_queue(QueueID::SYSTEM_ATTACK);

	while (!q->empty()) {
		sf::Packet p = q->front();
		q->pop();

		broadcastMessage(QueueID::SYSTEM_ATTACK, p);
	}
}

void SubSystemAttack::Recieve(anax::World &w)
{
	tempo::Queue<sf::Packet> *q = get_system_queue(QueueID::SYSTEM_ATTACK);

	while (!q->empty()) {
		sf::Packet p = q->front();
		q->pop();

		int code;
		p >> code;

		anax::Entity::Id id;
		p >> id;
		id = servertolocal[id];
		anax::Entity e(w, id);

		switch (code) {
		case Messages::ATTACK: {
			if (!e.hasComponent<ComponentAOEIndicator>()) {
				std::cout << "Entity does not contain ComponentAOEIndicator" << std::endl;
				assert(false);
			}
			ComponentAOEIndicator aoe    = e.getComponent<ComponentAOEIndicator>();
			auto &                weapon = e.getComponent<tempo::ComponentWeapon>();
			aoe.duration                 = sf::seconds(0.1);
			aoe.tiles                    = weapon.damage.positions;
			// Do some animating?

		} break;
		case Messages::DELAYED_ATTACK:
			// TODO
			break;
		default: std::cout << "ATTACK: Unhandled message" << std::endl;
		}
	}
}
}
