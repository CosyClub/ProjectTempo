#include <tempo/system/SystemGameInput.hpp>

#include <tempo/network/client.hpp>

namespace tempo
{

void alertServerBrokenCombo(anax::Entity::Id id) 
{
	sf::Packet packet;
	packet << id << static_cast<uint8_t>(MessageCombo::BROKEN_COMBO);
	sendMessage(QueueID::COMBO_UPDATES, packet);
}

bool SystemGameInput::handleInput(SDL_Event& e)
{
	if (e.type != SDL_KEYDOWN) return false;

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

	// HJKL (for nerds)
	case SDLK_h: dx = -1; dy =  0; break;
	case SDLK_l: dx =  1; dy =  0; break;
	case SDLK_k: dx =  0; dy = -1; break;
	case SDLK_j: dx =  0; dy =  1; break;

	default: return false; break;
	}

	auto entities = getEntities();

	// Check if player hit "on" the beat
	if (!clock.within_delta()) {
		// For loop should only run once due to PlayerLocal requirement
		for (auto& entity : entities) {
			auto& id    = entity.getId();
			auto& combo = entity.getComponent<ComponentCombo>();
			combo.breakCombo();
			alertServerBrokenCombo(id);
			
			std::cout << "Missed beat by "
			          << std::min(clock.since_beat().asMilliseconds(),
			                      clock.until_beat().asMilliseconds())
			          << std::endl;
		}
		return true;
	}


	for (auto& entity : entities) {
		auto& id     = entity.getId();
		auto& combo  = entity.getComponent<ComponentCombo>();
		auto& motion = entity.getComponent<ComponentStageTranslation>();

		bool stillInCombo = combo.performAction();
		if (stillInCombo) {
			// Move
			motion.delta.x = dx;
			motion.delta.y = dy;

			// Tell server we are moving
			sf::Packet packet;
			packet << id << dx << dy;
			sendMessage(QueueID::PLAYER_UPDATES, packet);
		} else {
			// Do not move or do anything other than tell the server
			// we have broken our combo	
			alertServerBrokenCombo(id);
		}
	}
	
	return true;
}

} //namespace tempo
