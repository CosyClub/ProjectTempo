#include <tempo/system/SystemHealth.hpp>

#include <tempo/component/ComponentStagePosition.hpp>  //Just temporary

namespace tempo
{
void SystemHealth::CheckHealth()
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
}
