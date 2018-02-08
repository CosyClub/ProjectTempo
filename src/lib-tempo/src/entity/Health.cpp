#include <tempo/entity/Health.hpp>

namespace tempo {

	ComponentHealth::ComponentHealth(int entity_health) {

		// Assign Health to Entity
		this->max_health = entity_health;
		this->current_health = entity_health;
	}

	ComponentHealth::ComponentHealth(int current_health, int max_health) {

		//Assign Health to Entity
		this->max_health = max_health;
		this->current_health = current_health;

	}

	int ComponentHealth::getCurrentHealth(){
		return (this->current_health);
	}

	int ComponentHealth::getMaxHealth(){
		return this->max_health;
	}

	void ComponentHealth::setHealth(int current_health){

					if(current_health <= (this->current_health)){
						this->current_health = current_health;
					}

					else{
						this->current_health = this->max_health;
					}

	}

	void ComponentHealth::HealthUpdate(int delta_health){

		// The entity's current health should not exceed the maximum health of the entity
		if ((this->current_health) + delta_health > (this->max_health)){
			this->current_health = this->max_health;
		}

		// Apply change to current health
		else{
			this->current_health += delta_health;
		}

		// Stop health from going less than 0
		if((this->current_health < 0)){
			this->current_health = 0;
		}

	}

	void SystemHealth::CheckHealth() {

		auto entities = getEntities();

		for (auto& entity : entities) {
			auto& h = entity.getComponent<ComponentHealth>();
			int id = entity.getId();

			// If the entity has 0 health then kill entity
			if ((h.getCurrentHealth() <= 0)){
				printf("\nEntity ID: %d has just been \"killed\". \n", id);
				// TODO: Sort something out for here
				// entity.kill();
			}
		}
	}
}
