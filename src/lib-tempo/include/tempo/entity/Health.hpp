#ifndef TEMPO_ENTITY_HEALTH_HPP
#define TEMPO_ENTITY_HEALTH_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

namespace tempo {

	struct ComponentHealth : anax::Component {

		//Starting Health for entity
		int max_health;
		int current_health;

		ComponentHealth(int entity_health);

	};

	struct SystemHealth : anax::System<anax::Requires<ComponentHealth>> {
		
		void HealthUpdate(int delta_health);

	};

}

#endif