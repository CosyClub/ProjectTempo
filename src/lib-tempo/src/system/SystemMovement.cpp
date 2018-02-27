#include <tempo/system/SystemMovement.hpp>


namespace tempo
{

void SystemMovement::processTranslation()
{
	auto entities = getEntities();

	auto entity = std::begin(entities);
	auto& stage   = entity->getComponent<tempo::ComponentStage>();

	for (auto& entity : entities) {
		if (entity.hasComponent<tempo::ComponentStageTranslation>() &&
				entity.hasComponent<tempo::ComponentStagePosition>()) {

			tempo::ComponentStageTranslation &st = entity.getComponent<tempo::ComponentStageTranslation>();
			if (st.delta.x == 0 && st.delta.y == 0) continue;

			glm::ivec2& position = entity.getComponent<tempo::ComponentStagePosition>().occupied[0];

			if(stage.existstTile(position + st.delta)) {
				position += st.delta;
			}

			st.delta = {0,0};

		}
	}
}

} // namespace tempo
