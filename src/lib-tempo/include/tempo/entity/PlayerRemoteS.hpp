///////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_ENTITY_PLAYERREMOTES_HPP
#define TEMPO_ENTITY_PLAYERREMOTES_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/entity/LevelManager.hpp>
#include <tempo/time.hpp>

#include <SDL.h>
#undef main // SDL likes to define main

namespace tempo{

	struct ComponentPlayerRemoteS : anax::Component 
	{
		bool moved_this_beat;

		inline ComponentPlayerRemoteS() : moved_this_beat(false) {}
	};

	struct SystemPlayerRemoteS : anax::System<anax::Requires<ComponentGridMotion, 
	                                                        ComponentPlayerRemoteS>> 
	{
		tempo::Clock& clock;

		inline SystemPlayerRemoteS(tempo::Clock& c) : clock(c) {}

		void advanceBeat();
		bool update();
	};
}

#endif
