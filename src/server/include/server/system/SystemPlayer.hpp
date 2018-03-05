#ifndef SERVER_SYSTEM_PLAYER_HPP
#define SERVER_SYSTEM_PLAYER_HPP

#include <anax/System.hpp>

namespace server
{

struct SystemPlayer : anax::System<anax::Requires<>> 
{
	// Recieves all the movement intents and bounces them to all the clients.
	// To be run all the time.
	void recieveTranslations(anax::World &w);
};

}  // namespace server

#endif
