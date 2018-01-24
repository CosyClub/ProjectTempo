////////////////////////////////////////////////////////////////////////////
///                      Part of Project Tempo                           ///
////////////////////////////////////////////////////////////////////////////

#include <tempo/entity/PlayerLocal.hpp>
#include <iostream>
#include <cstdio>

namespace tempo{
	void SystemPlayerLocal::advanceBeat(){
		auto entities = getEntities();

		for(auto& entity : entities){
			auto& input = entity.getComponent<tempo::ComponentPlayerLocal>();
			input.moved_this_beat = false;
		}
	}

	bool SystemPlayerLocal::handleInput(SDL_Event& e){
		if(e.type != SDL_KEYDOWN){
			return false;
		}

		int dx;
		int dy;

		switch (e.key.keysym.sym) {

			// Arrows
		case SDLK_LEFT:  dx = -1; dy =  0; break;
		case SDLK_RIGHT: dx =  1; dy =  0; break;
		case SDLK_UP:    dx =  0; dy = -1; break;
		case SDLK_DOWN:  dx =  0; dy =  1; break;

			// WASD
		case SDLK_a: dx = -1; dy =  0; break;
		case SDLK_d: dx =  1; dy =  0; break;
		case SDLK_w: dx =  0; dy = -1; break;
		case SDLK_s: dx =  0; dy =  1; break;

			// WASD
		case SDLK_h: dx = -1; dy =  0; break;
		case SDLK_l: dx =  1; dy =  0; break;
		case SDLK_k: dx =  0; dy = -1; break;
		case SDLK_j: dx =  0; dy =  1; break;

		default: return false; break;
		}

		if(!clock.within_delta()){
			std::cout << "Missed beat by " 
			          << std::min(clock.since_beat().asMilliseconds(), 
			                      clock.until_beat().asMilliseconds()) 
			          << std::endl;
			return true;
		}

		auto entities = getEntities();

		for(auto& entity : entities){
			auto& motion = entity.getComponent<tempo::ComponentGridMotion>();
			auto& input  = entity.getComponent<tempo::ComponentPlayerLocal>();

			if(!input.moved_this_beat){
				input.moved_this_beat = true;
				motion.beginMovement(dx, dy);
			}
		}
	}
}