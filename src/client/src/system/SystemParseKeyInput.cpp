#include <client/system/SystemParseKeyInput.hpp>

#include <tempo/component/ComponentAttack.hpp>
#include <tempo/component/ComponentCombo.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/component/ComponentStageTranslation.hpp>
#include <tempo/component/ComponentWeapon.hpp>

#include <tempo/network/base.hpp>
#include <tempo/network/ID.hpp>
#include <client/network/client.hpp>

#include <Keycodes.h>

#include <glm/vec2.hpp>

#include <iostream>

namespace client
{

using tempo::operator<<;
using tempo::operator>>;

void addMovement(anax::Entity &entity, glm::ivec2 delta, tempo::Facing facing, bool withinDelta)
{
	// Send Movement Intent to Networking
	sf::Packet p;
	p << tempo::localtoserver[entity.getId()];
	// Always include change of facing direction
	p << facing.x << facing.y;

	if (entity.hasComponent<tempo::ComponentStageRotation>()) {
		entity.getComponent<tempo::ComponentStageRotation>().facing = facing;
	}

	if (!withinDelta) {
		std::cout << "Actioned outside of delta" << std::endl;
		// Only include delta in update if actioned within the delta
		p << 0 << 0;
		tempo::sendMessage(tempo::QueueID::PLAYER_UPDATES, p);
		return;
	}

	// Include delta and tell server of intent
	p << delta.x << delta.y;
	tempo::sendMessage(tempo::QueueID::PLAYER_UPDATES, p);

	if (entity.hasComponent<tempo::ComponentStageTranslation>()) {
		entity.getComponent<tempo::ComponentStageTranslation>().delta = delta;
	}
}

void addAttack(anax::Entity &entity, tempo::Facing facing, bool withinDelta)
{
	if (!withinDelta) {
		std::cout << "Actioned outside of delta" << std::endl;
		return;
	}

	if (entity.hasComponent<tempo::ComponentAttack>() && entity.hasComponent<tempo::ComponentWeapon>()) {
		tempo::ComponentAttack &a = entity.getComponent<tempo::ComponentAttack>();
		tempo::ComponentWeapon &w = entity.getComponent<tempo::ComponentWeapon>();
		a.damage = w.damage;
		a.beats_until_attack = w.beats_until_attack;

		sf::Packet p;
		p << static_cast<uint32_t>(tempo::MessageAttack::UPDATE_INTENT);
		p << a.damage;
		p << a.beats_until_attack;
		tempo::sendMessage(tempo::QueueID::SYSTEM_ATTACK, p);
	}
}

void updateCombo(anax::Entity &entity, bool withinDelta)
{
	if (entity.hasComponent<tempo::ComponentCombo>()) {
		tempo::ComponentCombo &c = entity.getComponent<tempo::ComponentCombo>();
		if (withinDelta) {
			c.performAction();
		} else {
			c.breakCombo();
		}
		std::cout << "Combo: " << c.comboCounter << std::endl;
	}
}

void processKeyPressEvent(irr::EKEY_CODE key, anax::Entity &entity, bool withinDelta)
{
	switch (key) {
	case irr::KEY_KEY_W:
	case irr::KEY_UP:
		addMovement(entity, tempo::NORTH, tempo::NORTH, withinDelta);
		updateCombo(entity, withinDelta);
		break;
	case irr::KEY_KEY_A:
	case irr::KEY_LEFT:
		addMovement(entity, tempo::WEST, tempo::WEST, withinDelta);
		updateCombo(entity, withinDelta);
		break;
	case irr::KEY_KEY_S:
	case irr::KEY_DOWN:
		addMovement(entity, tempo::SOUTH, tempo::SOUTH, withinDelta);
		updateCombo(entity, withinDelta);
		break;
	case irr::KEY_KEY_D:
	case irr::KEY_RIGHT:
		addMovement(entity, tempo::EAST, tempo::EAST, withinDelta);
		updateCombo(entity, withinDelta);
		break;
	case irr::KEY_KEY_E:
		// system_attack.Attack(entity_player);
		updateCombo(entity, withinDelta);
		break;
	default: break;
	}
}

void SystemParseKeyInput::parseInput(tempo::Clock &clock)
{
	for (auto entity : getEntities()) {
		ComponentKeyInput ke = entity.getComponent<ComponentKeyInput>();

		bool withinDelta = false;
		if (clock.within_delta())
			withinDelta = true;

		for (unsigned int i = 0; i < ke.keysPressed.size(); i++) {
			if (ke.keysPressed[i].press) {
				processKeyPressEvent(ke.keysPressed[i].key, entity, withinDelta);
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

}  // namespace tempo
