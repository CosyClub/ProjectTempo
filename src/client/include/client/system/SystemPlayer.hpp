#ifndef CLIENT_SYSTEM_PLAYER_HPP
#define CLIENT_SYSTEM_PLAYER_HPP

#include <anax/System.hpp>

namespace client
{

struct SystemPlayer : anax::System<anax::Requires<>> {
	void processServerResponses(anax::World &world);
};

}  // namespace client

#endif
