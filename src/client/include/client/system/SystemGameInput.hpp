#ifndef TEMPO_SYSTEM_GAME_INPUT_HPP
#define TEMPO_SYSTEM_GAME_INPUT_HPP

#include <anax/System.hpp>
#include <anax/Component.hpp>

#include <tempo/component/ComponentCombo.hpp>
#include <tempo/component/ComponentPlayerLocal.hpp>
#include <tempo/component/ComponentStageTranslation.hpp>
#include <tempo/time.hpp>

#include <SDL.h>

#undef main // SDL likes to define main

namespace tempo
{

	struct SystemGameInput : anax::System<
	                         anax::Requires<ComponentCombo,
	                                        ComponentStageTranslation, 
	                                        ComponentPlayerLocal>> 
	{
		tempo::Clock& clock;

		inline SystemGameInput(tempo::Clock& c) : clock(c) {};

		bool handleInput(SDL_Event& e);
	};

} // namespace tempo

#endif
