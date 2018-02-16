#ifndef TEMPO_SYSTEM_PLAYERLOCAL_HPP
#define TEMPO_SYSTEM_PLAYERLOCAL_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/component/ComponentStageTranslation.hpp>
#include <tempo/component/ComponentPlayerLocal.hpp>
#include <tempo/component/ComponentID.hpp>
#include <tempo/network/client.hpp>
#include <tempo/network/QueueID.hpp>
#include <tempo/time.hpp>

#include <iostream>
#include <cstdio>
#include <SDL.h>

#undef main // SDL likes to define main

namespace tempo{

	struct SystemPlayerLocal : anax::System<anax::Requires<ComponentStageTranslation, ComponentPlayerLocal>> {
		tempo::Clock& clock;

		inline SystemPlayerLocal(tempo::Clock& c) : clock(c) {};

		void advanceBeat();
		bool handleInput(SDL_Event& e);
	};
}

#endif
