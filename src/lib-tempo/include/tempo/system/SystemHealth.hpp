#ifndef TEMPO_ENTITY_HEALTH_HPP
#define TEMPO_ENTITY_HEALTH_HPP

#include <anax/System.hpp>

#include <tempo\entity\ComponentHealth.hpp>

namespace tempo {

	struct SystemHealth : anax::System<anax::Requires<ComponentHealth>> {

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
