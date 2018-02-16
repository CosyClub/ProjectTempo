#include <tempo/system/SystemPlayerLocal.hpp>

namespace tempo{
	void SystemPlayerLocal::advanceBeat(){
		auto entities = getEntities();

		for(auto& entity : entities){
			auto& input = entity.getComponent<tempo::ComponentPlayerLocal>();
			if(!input.moved_this_beat) {
				input.counter_combo = 0;
				input.level_combo = 0;
			}
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

		auto entities = getEntities();
		
		if(!clock.within_delta()){
			for (auto& entity : entities) {
				if (entity.hasComponent<tempo::ComponentPlayerLocal>()) {
					auto& input = entity.getComponent<tempo::ComponentPlayerLocal>();
					input.counter_combo = 0;
					break;
				}
			}
			std::cout << "Local player missed beat by "
			          << std::min(clock.since_beat().asMilliseconds(),
			                      clock.until_beat().asMilliseconds())
			          << std::endl;
			return true;
		}


		for(auto& entity : entities){
			auto& id     = entity.getComponent<tempo::ComponentID>();
			auto& motion = entity.getComponent<tempo::ComponentStageTranslation>();
			auto& input  = entity.getComponent<tempo::ComponentPlayerLocal>();

			if(!input.moved_this_beat){
				input.moved_this_beat = true;
				input.counter_combo++;
				assert(false);
				//motion.beginMovement(dx, dy);

				sf::Packet packet;
				packet << id.instance_id << dx << dy;
				sendMessage(SystemQID::PLAYER_UPDATES, packet, false);
			}
		}
	}
}
