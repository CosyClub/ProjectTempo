#ifndef TEMPO_ENTITY_HEALTH_HPP
#define TEMPO_ENTITY_HEALTH_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

namespace tempo {

	struct ComponentHealth : anax::Component {

	private:
		//Starting Health for entity
		int max_health;
		int current_health;

	public:
		// ComponentHealth
		// Initialisation of an entity's Health Component
		//
		// Arguments:
		//          entity_health - The amount of health to give to the entity
		// Returns:
		//          void
		ComponentHealth(int entity_health);

		// ComponentHealth
		// Initialisation of an entity's Health Component where the starting health is not
		// the same as the max health
		//
		// Arguments:
		//          current_health - The amount of health to give to the entity
		//          max_health - the max_health to give to the entity
		// Returns:
		//          void
		ComponentHealth(int current_health, int max_health);

		int getCurrentHealth();
		int getMaxHealth();

		void setHealth(int current_health);

		// HealthUpdate
		// Update the health of an individual entity by a specific amount
		//
		// Arguments:
		//          delta_health - The amount the entity's health will change (can be ±)
		// Returns:
		//          void
		void HealthUpdate(int delta_health);

	};

	struct SystemHealth : anax::System<anax::Requires<ComponentHealth>> {

		// HealthUpdate
		// Update all entities health by a specific amount
		//
		// Arguments:
		//          delta_health - The amount each entities health will change (can be ±)
		// Returns:
		//          void
		void HealthUpdate(int delta_health);

		// CheckHealth
		// Check all entities health to see if the entity has died or not
		//
		// Arguments:
		//          none
		// Returns:
		//          void
		void CheckHealth();

	};
}

#endif
