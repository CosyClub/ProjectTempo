#ifndef TEMPO_ENTITY_PLAYERREMOTESERVER_HPP
#define TEMPO_ENTITY_PLAYERREMOTESERVER_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/component/ComponentGridMotion.hpp>
#include <tempo/system/SystemID.hpp>
#include <tempo/entity/SystemQID.hpp>
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

	struct SystemPlayerRemoteServer : anax::System<anax::Requires<ComponentGridMotion, 
	                                                        ComponentPlayerRemoteServer>> 
	{
		tempo::Clock& clock;

		inline SystemPlayerRemoteServer(tempo::Clock& c) : clock(c) {}

		void advanceBeat();
		bool update(SystemID system_id);
	};
}

#endif
