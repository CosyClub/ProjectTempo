#ifndef TEMPO_COMPONENT_HEALTH_HPP
#define TEMPO_COMPONENT_HEALTH_HPP

#include <anax/Component.hpp>

#include <tempo/component/NetworkedComponent.hpp>

namespace tempo
{
struct ComponentHealth
    : anax::Component
    , NetworkedComponent {
	// Starting Health for entity
	int32_t max_health;
	int32_t current_health;

	// ComponentHealth
	// Initialisation of an entity's Health Component
	//
	// Arguments:
	//          entity_health - The amount of health to give to the entity
	ComponentHealth(int32_t entity_health);

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

	// HealthUpdate
	// Update the health of an individual entity by a specific amount
	//
	// Arguments:
	//          delta_health - The amount the entity's health will change
	//                         (can be �)
	void HealthUpdate(int32_t delta_health);

	/////
	// Required for networking
	/////
	ComponentHealth(sf::Packet p);
	sf::Packet  dumpComponent();
	ComponentID getId();
};

}  // namespace tempo

#endif
