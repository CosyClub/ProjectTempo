#include <client/system/SystemParseKeyInput.hpp>

#include <tempo/component/ComponentStageTranslation.hpp>
#include <tempo/component/ComponentStageRotation.hpp>

#include <Keycodes.h>

#include <glm/vec2.hpp>

#include <iostream>

namespace client
{

void addMovement(anax::Entity &entity, glm::ivec2 delta, tempo::Facing facing) {
	if (entity.hasComponent<tempo::ComponentStageTranslation>()) {
		entity.getComponent<tempo::ComponentStageTranslation>().delta = delta;
	}
	if (entity.hasComponent<tempo::ComponentStageRotation>()) {
		entity.getComponent<tempo::ComponentStageRotation>().facing = facing;
	}
}

void processKeyPressEvent(irr::EKEY_CODE key, anax::Entity &entity) {
	switch (key) {
	case irr::KEY_KEY_W:
	case irr::KEY_UP:
		addMovement(entity, tempo::NORTH, tempo::NORTH);
		break;
	case irr::KEY_KEY_A:
	case irr::KEY_LEFT:
		addMovement(entity, tempo::WEST, tempo::WEST);
		break;
	case irr::KEY_KEY_S:
	case irr::KEY_DOWN:
		addMovement(entity, tempo::SOUTH, tempo::SOUTH);
		break;
	case irr::KEY_KEY_D:
	case irr::KEY_RIGHT:
		addMovement(entity, tempo::EAST, tempo::EAST);
		break;
	case irr::KEY_KEY_E:
		// system_attack.Attack(entity_player);
		break;
	default: break;
	}
}

void SystemParseKeyInput::parseInput(tempo::Clock &clock)
{
	for (auto entity : getEntities()) {
		ComponentKeyInput ke = entity.getComponent<ComponentKeyInput>();

		for (unsigned int i = 0; i < ke.keysPressed.size(); i++) {
			if (ke.keysPressed[i].press) {
				processKeyPressEvent(ke.keysPressed[i].key,
				                     entity);
			}
		}
	}
}
// bool SystemGameInput::handleInput(SDL_Event& e)
// {
	// if (e.type != SDL_KEYDOWN) return false;

	// int dx;
	// int dy;

	// switch (e.key.keysym.sym) {
	// // Arrows
	// case SDLK_LEFT:  dx = -1; dy =  0; break;
	// case SDLK_RIGHT: dx =  1; dy =  0; break;
	// case SDLK_UP:    dx =  0; dy = -1; break;
	// case SDLK_DOWN:  dx =  0; dy =  1; break;

	// // WASD
	// case SDLK_a: dx = -1; dy =  0; break;
	// case SDLK_d: dx =  1; dy =  0; break;
	// case SDLK_w: dx =  0; dy = -1; break;
	// case SDLK_s: dx =  0; dy =  1; break;

	// // HJKL (for nerds)
	// case SDLK_h: dx = -1; dy =  0; break;
	// case SDLK_l: dx =  1; dy =  0; break;
	// case SDLK_k: dx =  0; dy = -1; break;
	// case SDLK_j: dx =  0; dy =  1; break;

	// default: return false; break;
	// }

	// auto entities = getEntities();

	// // Check if player hit "on" the beat
	// if (!clock.within_delta()) {
	// 	// For loop should only run once due to PlayerLocal requirement
	// 	for (auto& entity : entities) {
	// 		auto& id    = entity.getId();
	// 		auto& combo = entity.getComponent<ComponentCombo>();
	// 		combo.breakCombo();
	// 		alertServerBrokenCombo(id);

	// 		std::cout << "Missed beat by "
	// 		          << std::min(clock.since_beat().asMilliseconds(),
	// 		                      clock.until_beat().asMilliseconds())
	// 		          << std::endl;
	// 	}
	// 	return true;
	// }


	// for (auto& entity : entities) {
	// 	auto& id     = entity.getId();
	// 	auto& combo  = entity.getComponent<ComponentCombo>();
	// 	auto& motion = entity.getComponent<ComponentStageTranslation>();

	// 	bool stillInCombo = combo.performAction();
	// 	if (stillInCombo) {
	// 		// Move
	// 		motion.delta.x = dx;
	// 		motion.delta.y = dy;

	// 		// Tell server we are moving
	// 		sf::Packet packet;
	// 		packet << localtoserver[id] << dx << dy;
	// 		sendMessage(QueueID::PLAYER_UPDATES, packet);
	// 	} else {
	// 		// Do not move or do anything other than tell the server
	// 		// we have broken our combo
	// 		alertServerBrokenCombo(id);
	// 	}
	// }

	// return true;
// }

} //namespace tempo
