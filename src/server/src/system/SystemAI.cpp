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
				tempo::ComponentAttack &a = entity.getComponent<tempo::ComponentAttack>();
				tempo::ComponentWeapon &w = entity.getComponent<tempo::ComponentWeapon>();
				tempo::ComponentStageRotation &r = entity.getComponent<tempo::ComponentStageRotation>();
				a.damage = w.damage;
				a.beats_until_attack = w.beats_until_attack;

				w.beats_until_attack = 0;
				r.facing = direction;
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
