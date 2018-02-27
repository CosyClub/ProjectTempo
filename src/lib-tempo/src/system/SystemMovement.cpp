#include <tempo/system/SystemMovement.hpp>


namespace tempo
{

void SystemMovement::processTranslation()
{
	auto entities = getEntities();

	for (auto& entity : entities) {
		if (entity.hasComponent<tempo::ComponentStageTranslation>() &&
				entity.hasComponent<tempo::ComponentStagePosition>()) {

			tempo::ComponentStageTranslation &st = entity.getComponent<tempo::ComponentStageTranslation>();
			if (st.delta.x == 0 && st.delta.y == 0) continue;
			entity.getComponent<tempo::ComponentStagePosition>().occupied[0] += st.delta;
			st.delta = {0,0};
			
		}
	}
}

} // namespace tempo
