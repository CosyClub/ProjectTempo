////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////
/// \file PlayerInput.hpp
/// \author Jamie Terry
/// \date 2017/11/15
/// \brief Contains decleration of player input component and system
////////////////////////////////////////////////////////////////////////////

#ifndef TEMPO_ENTITY_PLAYERINPUT_HPP
#define TEMPO_ENTITY_PLAYERINPUT_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/entity/LevelManager.hpp>
#include <tempo/time.hpp>

#include <SDL.h>
#undef main // SDL likes to define main

namespace tempo{

  struct ComponentPlayerInput : anax::Component {
	  bool moved_this_beat;

	  inline ComponentPlayerInput() : moved_this_beat(false) {}
  };

	struct SystemPlayerInput : anax::System<anax::Requires<ComponentGridMotion, ComponentPlayerInput>> {
		tempo::Clock& clock;

		inline SystemPlayerInput(tempo::Clock& c) : clock(c) {}

		void advanceBeat();
		bool handleInput(SDL_Event& e);
	};
}

#endif
