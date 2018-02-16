#ifndef TEMPO_COMPONENT_COMPONENTHEALTH_HPP 
#define TEMPO_COMPONENT_COMPONENTHEALTH_HPP 

#include <anax/Component.hpp>

namespace tempo {

struct ComponentHealth : anax::Component {

	// Starting Health for entity
	int max_health;
	int current_health;

	// ComponentHealth
	// Initialisation of an entity's Health Component
	//
	// Arguments:
	//          entity_health - The amount of health to give to the entity
	ComponentHealth(int entity_health);

	// HealthUpdate
	// Update the health of an individual entity by a specific amount
	//
	// Arguments:
	//          delta_health - The amount the entity's health will change 
	//                         (can be ±)
	void HealthUpdate(int delta_health);

};

} // namespace tempo

#endif
