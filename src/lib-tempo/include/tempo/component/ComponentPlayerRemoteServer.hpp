#ifndef TEMPO_COMPONENT_PLAYERREMOTESERVER_HPP
#define TEMPO_COMPONENT_PLAYERREMOTESERVER_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/component/ComponentStagePosition.hpp>
#include <tempo/system/SystemID.hpp>
#include <tempo/network/QueueID.hpp>
#include <tempo/network/queue.hpp>
#include <tempo/network/server.hpp>
#include <tempo/time.hpp>

#include <SDL.h>
#include <iostream>
#include <stdexcept>
#include <cstdio>

#undef main // SDL likes to define main

namespace tempo{

	struct ComponentPlayerRemoteServer : anax::Component 
	{
		bool moved_this_beat;

		inline ComponentPlayerRemoteServer() : moved_this_beat(false) {}
	};
}

#endif
