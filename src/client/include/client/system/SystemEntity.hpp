#ifndef CLIENT_SYSTEM_ENTITY_HPP
#define CLIENT_SYSTEM_ENTITY_HPP

#include <anax/World.hpp>

namespace client
{
struct SystemEntity : anax::System<anax::Requires<>> {
	void creationCheck(anax::World &w);
	void deletionCheck(anax::World &w);
};

}  // namespace client

#endif
