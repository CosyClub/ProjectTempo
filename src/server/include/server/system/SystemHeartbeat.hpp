#ifndef SERVER_SYSTEM_HEARTBEAT_HPP
#define SERVER_SYSTEM_HEARTBEAT_HPP

#include <anax/System.hpp>
#include <anax/World.hpp>

namespace server
{

struct SystemHeartbeat : anax::System<anax::Requires<>> {
	void checkForHeatbeats(anax::World &world);
};

} // namespace server

#endif
