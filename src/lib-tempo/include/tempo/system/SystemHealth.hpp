#ifndef TEMPO_SYSTEM_SYSTEMHEALTH_HPP
#define TEMPO_SYSTEM_SYSTEMHEALTH_HPP

#include <anax/System.hpp>

#include <tempo/component/ComponentHealth.hpp>
#include <tempo/network/base.hpp>
#include <tempo/network/ID.hpp>
#include <tempo/network/queue.hpp>

namespace tempo
{
struct SystemHealth : anax::System<anax::Requires<ComponentHealth>> {
	// CheckHealth
	// Check all entities health to see if the entity has died or not
	//
	// Arguments:
	//          none
	// Returns:
	//          void
	void CheckHealth();

	void broadcastHealth();
	void recieveHealth(anax::World &world);
};
}

#endif
