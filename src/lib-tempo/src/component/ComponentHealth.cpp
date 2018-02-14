#include <tempo/component/ComponentHealth.hpp>

namespace tempo {

	ComponentHealth::ComponentHealth(int entity_health) {

		// Assign Health to Entity
		this->max_health = entity_health;
		this->current_health = entity_health;
	}

	void ComponentHealth::HealthUpdate(int delta_health) {

		// The entity's current health should not exceed the maximum health of the entity
		if ((this->current_health) + delta_health > (this->max_health)) {
			this->current_health = this->max_health;
		}

		// Apply change to current health
		else {
			this->current_health += delta_health;
		}

		// Stop health from going less than 0
		if ((this->current_health < 0)) {
			this->current_health = 0;
		}

	}
}