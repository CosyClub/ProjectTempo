#include <server/system/SystemAttack.hpp>

#include <tempo/component/NetworkedComponent.hpp>
#include <tempo/component/ComponentAOEIndicator.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/component/ComponentWeapon.hpp>

#include <tempo/network/ID.hpp>
#include <tempo/network/server.hpp>

#include <tempo/mask.hpp>

namespace server
{

using tempo::operator<<;
using tempo::operator>>;

SystemAttack::SystemAttack(anax::World &world)
{
	subSystem = SubSystemAttack();
	world.addSystem(subSystem);
	world.refresh();
}

void SystemAttack::recieveAttacks(anax::World &w)
{
	tempo::Queue<sf::Packet> *q = tempo::get_system_queue(tempo::QueueID::SYSTEM_ATTACK);

	while (!q->empty()) {
		sf::Packet p = q->front();
		q->pop();	
		sf::Packet pb; // packet for broadcast

		uint32_t code;
		p >> code;
		pb << code;

		anax::Entity::Id id;
		p >> id;  // ID of the entity this message concerns
		pb << id;
		anax::Entity e(w, id);

		switch (static_cast<tempo::MessageAttack>(code)) {
		case tempo::MessageAttack::UPDATE_INTENT: {
			if (!e.hasComponent<tempo::ComponentAttack>()) {
				std::cout << "Recieved Attack Intent Update from entity without ComponentAttack" << std::endl;
				continue;
			}
			tempo::ComponentAttack &c = e.getComponent<tempo::ComponentAttack>();
			p >> c.damage;
			p >> c.beats_until_attack;
			pb << c.damage;
			pb << c.beats_until_attack;
			tempo::broadcastMessage(tempo::QueueID::SYSTEM_ATTACK, pb);
			break;
		}
		case tempo::MessageAttack::ATTACK_CORRECTION:
			// do nothing, server should not recieve corrections
			break;
		default: 
			std::cout << "ATTACK: Unhandled/erroneous message" << std::endl;
		}

	}
}

void SystemAttack::processAttacks()
{
	for (auto &entity : getEntities()) {
		if (entity.getComponent<tempo::ComponentAttack>().isAttacking()) {
			subSystem.Attack(entity);
		}
	}
}

bool SystemAttack::bestAttack(anax::Entity attacker, glm::ivec2 &direction)
{
	return subSystem.bestAttack(attacker, direction);
}

bool SubSystemAttack::bestAttack(anax::Entity attacker, glm::ivec2 &direction)
{
	glm::ivec2  attackerpos = attacker.getComponent<tempo::ComponentStagePosition>().getOrigin();
	// glm::ivec2  rot         = attacker.getComponent<tempo::ComponentStageRotation>().facing;
	auto &      weapon      = attacker.getComponent<tempo::ComponentWeapon>();
	
	glm::ivec2 bestDirection;
	float bestDamage = 0;

	for (glm::ivec2 rot : tempo::DIRECTIONS)
	{
		float currentDamage = 0;
		for (auto &entity : getEntities()) {
			// Get health and positions occupired by other entity
			std::vector<glm::ivec2> ps = entity.getComponent<tempo::ComponentStagePosition>().getOccupied();

			// Add positions after stage translation (if any) to ps vector
			// TODO: This doesn't take into account if the movement system
			//       doesn't accept a stage translation. Work out if 
			//       that matters?
			if (entity.hasComponent<tempo::ComponentStageTranslation>()) {
				glm::ivec2 d = entity.getComponent<tempo::ComponentStageTranslation>().delta;
				if (d != glm::ivec2(0,0)) {
					for (glm::ivec2 p : entity.getComponent<tempo::ComponentStagePosition>().getOccupied()) {
						if (std::find(ps.begin(), ps.end(), p+d) != ps.end()) {
							ps.push_back(p+d);
						}

					}
				}
			}

			// Calculate relative directions from the attacker
			glm::vec2 forward = rot;
			glm::vec2 left = glm::ivec2(-rot.y, -rot.x);  // Hacky cross product

			// Deal damage to the other entity
			for (glm::ivec2 p : ps) {
				glm::vec2  diff          = p - attackerpos;
				glm::ivec2 relative_diff = glm::ivec2(glm::dot(diff, left), glm::dot(diff, forward));

				float damage = weapon.GetDamage(relative_diff);
				currentDamage += damage;
			}
		}

		if ( currentDamage > bestDamage ) bestDirection = rot;
	}

	direction = bestDirection;	
	return bestDamage;
}

void SubSystemAttack::Attack(anax::Entity attacker)
{
	// Get Attacker Components
	glm::ivec2  attackerpos = attacker.getComponent<tempo::ComponentStagePosition>().getOrigin();
	glm::ivec2  rot         = attacker.getComponent<tempo::ComponentStageRotation>().facing;
	auto &      attack      = attacker.getComponent<tempo::ComponentAttack>();
	auto &      weapon      = attacker.getComponent<tempo::ComponentWeapon>();

	// If a delayed attack, process and update clients
	if (attack.beats_until_attack > 0) {
		attack.beats_until_attack--;
		sf::Packet p;
		p << static_cast<uint32_t>(tempo::MessageAttack::UPDATE_INTENT);
		p << attacker.getId();
		p << attack.damage;
		p << attack.beats_until_attack;
		tempo::broadcastMessage(tempo::QueueID::SYSTEM_ATTACK, p);
		return;
	}

	for (auto &entity : getEntities()) {
		// Get health and positions occupired by other entity
		std::vector<glm::ivec2> ps = entity.getComponent<tempo::ComponentStagePosition>().getOccupied();
		auto &health = entity.getComponent<tempo::ComponentHealth>();

		// Add positions after stage translation (if any) to ps vector
		// TODO: This doesn't take into account if the movement system
		//       doesn't accept a stage translation. Work out if 
		//       that matters?
		if (entity.hasComponent<tempo::ComponentStageTranslation>()) {
			glm::ivec2 d = entity.getComponent<tempo::ComponentStageTranslation>().delta;
			if (d != glm::ivec2(0,0)) {
				for (glm::ivec2 p : entity.getComponent<tempo::ComponentStagePosition>().getOccupied()) {
					if (std::find(ps.begin(), ps.end(), p+d) != ps.end()) {
						ps.push_back(p+d);
					}

				}
			}
		}

		// Calculate relative directions from the attacker
		glm::vec2 forward = rot;
		glm::vec2 left = glm::ivec2(-rot.y, -rot.x);  // Hacky cross product

		// Deal damage to the other entity
		for (glm::ivec2 p : ps) {
			glm::vec2  diff          = p - attackerpos;
			glm::ivec2 relative_diff = glm::ivec2(glm::dot(diff, left), glm::dot(diff, forward));

			float damage = weapon.GetDamage(relative_diff);
			if (damage != 0) {
				std::cout << "hit entity " << entity.getId().index << " for " << damage << std::endl;
				health.HealthUpdate(-1 * damage);

				sf::Packet p;
				p << entity.getId();
				p << static_cast<uint32_t>(tempo::MessageAttack::ATTACK_CORRECTION);
				p << health.current_health;
				tempo::broadcastMessage(tempo::QueueID::SYSTEM_ATTACK, p);

				break;
			}
		}
	}

	// Clear attack intent and broadcast to clients
	// TODO:
	attack.damage = tempo::Mask(glm::ivec2(0,0), NULL, glm::ivec2(0,0));
	attack.beats_until_attack = -1;
	sf::Packet p;
	p << static_cast<uint32_t>(tempo::MessageAttack::UPDATE_INTENT);
	p << attacker.getId();
	p << attack.damage;
	p << attack.beats_until_attack;
	tempo::broadcastMessage(tempo::QueueID::SYSTEM_ATTACK, p);
}

} // namespace server
