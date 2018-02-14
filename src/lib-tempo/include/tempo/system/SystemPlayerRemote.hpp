#ifndef TEMPO_SYSTEM_PLAYERREMOTE_HPP
#define TEMPO_SYSTEM_PLAYERREMOTE_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/component/ComponentGridMotion.hpp>
#include <tempo/component/ComponentPlayerRemote.hpp>
#include <tempo/system/SystemID.hpp>
#include <tempo/entity/SystemQID.hpp>
#include <tempo/network/queue.hpp>
#include <tempo/time.hpp>

#include <SDL.h>
#include <iostream>
#include <cstdio>
#include <stdexcept>

#undef main // SDL likes to define main

namespace tempo{

	struct SystemPlayerRemote : anax::System<anax::Requires<ComponentGridMotion, 
	                                                        ComponentPlayerRemote>> 
	{
		tempo::Clock& clock;

		inline SystemPlayerRemote(tempo::Clock& c) : clock(c) {}

		void advanceBeat();
		bool update(int player_id, SystemID system_id);
	};
}

#endif
