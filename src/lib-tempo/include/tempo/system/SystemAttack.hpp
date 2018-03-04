#ifndef CLIENT_SYSTEM_ATTACK_HPP
#define CLIENT_SYSTEM_ATTACK_HPP

#include <anax/Component.hpp>
#include <anax/System.hpp>

namespace tempo
{

struct SystemAttack : anax::System<anax::Requires<>> 
{
	// Processes the attack responses from the server
	void processServerResponses();
};

} // namespace tempo

#endif
