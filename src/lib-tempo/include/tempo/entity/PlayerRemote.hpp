///////////////////////////////////////////////////////////////////////////
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

	struct ComponentPlayerRemote : anax::Component 
	{
		bool moved_this_beat;

		inline ComponentPlayerRemote() : moved_this_beat(false) {}
	};

	struct SystemPlayerRemote : anax::System<anax::Requires<ComponentGridMotion, 
	                                                        ComponentPlayerRemote>> 
	{
		tempo::Clock& clock;

		inline SystemPlayerRemote(tempo::Clock& c) : clock(c) {}

		void advanceBeat();
		bool update();
	};
}

#endif
