#ifndef CLIENT_SYSTEM_MOVEMENT_HPP
#define CLIENT_SYSTEM_MOVEMENT_HPP

#include <anax/System.hpp>
#include <anax/World.hpp>

namespace client
{

struct SystemMovement : anax::System<anax::Requires<>> 
{
	// Processes the attack responses from the server
	void processServerResponses(anax::World &w);
};

} // namespace client

#endif
