#ifndef TEMPO_SYSTEM_PLAYERREMOTESERVER_HPP
#define TEMPO_SYSTEM_PLAYERREMOTESERVER_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/component/ComponentStageTranslation.hpp>
#include <tempo/component/ComponentPlayerRemoteServer.hpp>
#include <tempo/system/SystemID.hpp>
#include <tempo/network/queue.hpp>
#include <tempo/network/QueueID.hpp>
#include <tempo/network/server.hpp>
#include <tempo/time.hpp>

#include <SDL.h>
#include <iostream>
#include <stdexcept>
#include <cstdio>

#undef main // SDL likes to define main

namespace tempo{

	struct SystemPlayerRemoteServer : anax::System<anax::Requires<ComponentStageTranslation, 
	                                                        ComponentPlayerRemoteServer>> 
	{
		tempo::Clock& clock;

		inline SystemPlayerRemoteServer(tempo::Clock& c) : clock(c) {}

		void advanceBeat();
		bool update(SystemID system_id);
	};
}

#endif
