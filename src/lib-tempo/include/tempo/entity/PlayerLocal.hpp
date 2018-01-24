////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_ENTITY_PLAYERLOCAL_HPP
#define TEMPO_ENTITY_PLAYERLOCAL_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/entity/LevelManager.hpp>
#include <tempo/time.hpp>

#include <SDL.h>
#undef main // SDL likes to define main

namespace tempo{

	struct ComponentPlayerLocal : anax::Component {
		bool moved_this_beat;

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
