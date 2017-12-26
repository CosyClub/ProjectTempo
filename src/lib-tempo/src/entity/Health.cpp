#include <tempo/entity/Health.hpp>

namespace tempo {

	ComponentHealth::ComponentHealth(int entity_health) {
		this->max_health = entity_health;
		this->current_health = entity_health;
	}

	void ComponentHealth::HealthUpdate(int delta_health){

		if ((this->current_health) + delta_health > (this->max_health)){
			this->current_health = this->max_health;
		}

		else{
			this->current_health += delta_health;
		}

	}

	void SystemHealth::HealthUpdate(int delta_health) {

		auto entities = getEntities();

		for (auto& entity : entities) {
			auto& h = entity.getComponent<ComponentHealth>();
			int id = entity.getId();

			if ((h.current_health) + delta_health > (h.max_health)) {
				h.current_health = h.max_health;
			}

			else {
				h.current_health += delta_health;
			}

			printf("Entity ID: %d has %d Health Points.\n", id, h.current_health);

			if ((h.current_health <= 0)) {
				printf("\nEntity ID: %d has just been killed.\n", id);
				entity.kill();
			}
		}

	}

}
