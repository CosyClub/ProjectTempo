#ifndef TEMPO_ENTITY_PLAYERLOCAL_HPP
#define TEMPO_ENTITY_PLAYERLOCAL_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/entity/ComponentGridMotion.hpp>
#include <tempo/entity/ID.hpp>
#include <tempo/entity/SystemQID.hpp>
#include <tempo/network/client.hpp>
#include <tempo/time.hpp>

#include <iostream>
#include <cstdio>
#include <SDL.h>

#undef main // SDL likes to define main

namespace tempo{

	struct ComponentPlayerLocal : anax::Component {
		bool moved_this_beat;
		uint counter_combo;
		uint level_combo;

		inline ComponentPlayerLocal() : moved_this_beat(false) {}
	};

	struct SystemPlayerLocal : anax::System<anax::Requires<ComponentGridMotion, ComponentPlayerLocal>> {
		tempo::Clock& clock;

		inline SystemPlayerLocal(tempo::Clock& c) : clock(c) {}

		void advanceBeat();
		bool handleInput(SDL_Event& e);
	};
}

#endif
