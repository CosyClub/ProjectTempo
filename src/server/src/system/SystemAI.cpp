#include <server/system/SystemAI.hpp>

namespace server
{
void SystemAI::update(server::SystemAttack s_attack)
{
	auto entities = getEntities();

	for (auto &entity : entities) {

		if (entity.hasComponent<tempo::ComponentWeapon>())
		{
			auto& weapon = entity.getComponent<tempo::ComponentWeapon>();
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
