////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// Health.hpp
///
/// Header definitions for Health Component & Systems.
////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_ENTITY_HEALTH_HPP
#define TEMPO_ENTITY_HEALTH_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>
#include <tempo/entity/Render.hpp>
#include <Ogre.h>

namespace tempo {

	struct ComponentHealth : anax::Component {

		//Starting Health for entity
		int max_health;
		int current_health;

		// ComponentHealth
		// Initialisation of an entity's Health Component
		//
		// Arguments:
		//          entity_health - The amount of health to give to the entity
		// Returns:
		//          void
		ComponentHealth(int entity_health);

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


	struct RenderHealth : anax::System<anax::Requires<ComponentHealth, ComponentRender>> {

		// HealthBarUpdate
		// Update all the health bars of entities based on their current health value
		//
		// Arguments:
		//          none
		// Returns:
		//          void
		void HealthBarUpdate();
	};

}

#endif
