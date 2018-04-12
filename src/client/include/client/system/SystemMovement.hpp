#ifndef CLIENT_SYSTEM_MOVEMENT_HPP
#define CLIENT_SYSTEM_MOVEMENT_HPP

#include <anax/System.hpp>
#include <anax/World.hpp>

namespace client
{
struct SystemMovement : anax::System<anax::Requires<>> {
	// Processes the movement intents received from the server
	void processIntents(anax::World &world);

	// Processes the movement corrections (absolute updates) from the server
	void processCorrections(anax::World &world);
};

}  // namespace client

#endif
