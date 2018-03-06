#include <server/system/SystemAI.hpp>

#include <iostream>

namespace server
{
	
using tempo::operator<<;

void SystemAI::update(server::SystemAttack s_attack)
{
	auto entities = getEntities();

	for (auto &entity : entities) {

		if (entity.hasComponent<tempo::ComponentAttack>() && entity.hasComponent<tempo::ComponentWeapon>())
		{
			glm::ivec2 direction;
			if (s_attack.bestAttack(entity, direction))
			{
				std::cout << "Attacking" << std::endl;
				tempo::ComponentAttack &a = entity.getComponent<tempo::ComponentAttack>();
				tempo::ComponentWeapon &w = entity.getComponent<tempo::ComponentWeapon>();
				tempo::ComponentStageRotation &r = entity.getComponent<tempo::ComponentStageRotation>();

				r.facing = direction; //enemies don't believe in turning, they just stab

				a.damage = w.damage;
				a.beats_until_attack = w.beats_until_attack;

				sf::Packet p;
				p << static_cast<uint32_t>(tempo::MessageAttack::UPDATE_INTENT);
				p << entity.getId();
				p << a.damage;
				p << a.beats_until_attack;
				
				tempo::Queue<sf::Packet> *q = get_system_queue(tempo::QueueID::SYSTEM_ATTACK);
				q->push(p);
				return;
			}
		}

		auto &grid_motion = entity.getComponent<tempo::ComponentStageTranslation>();

		int dir    = rand() % 2;            // between 0 and 1
		int amount = (rand() % 2) * 2 - 1;  //-1 or 1

		if (dir) {
			;
		} else {
			grid_motion.delta = glm::ivec2(0, amount);
		}
	}
}
}
