#include <tempo/system/SystemHealth.hpp>

namespace tempo {

	void SystemHealth::CheckHealth() {

		auto entities = getEntities();

		for (auto& entity : entities) {
			auto& h = entity.getComponent<ComponentHealth>();
			int id = entity.getId();

			// If the entity has 0 health then kill entity
			if ((h.current_health <= 0)){
				printf("\nEntity ID: %d has just been \"killed\". \n", id);
				// TODO: Sort something out for here
				// entity.kill();
			}
		}
	}
}
