#include <client/system/SystemParseKeyInput.hpp>

#include <client/component/ComponentRenderSceneNode.hpp>

#include <tempo/component/ComponentAttack.hpp>
#include <tempo/component/ComponentCombo.hpp>
#include <tempo/component/ComponentHealth.hpp>
#include <tempo/component/ComponentStageRotation.hpp>
#include <tempo/component/ComponentStageTranslation.hpp>
#include <tempo/component/ComponentWeapon.hpp>

#include <client/network/client.hpp>
#include <tempo/network/ID.hpp>
#include <tempo/network/base.hpp>

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
	auto &sr = entity.getComponent<tempo::ComponentStageRotation>();

	if (entity.hasComponent<tempo::ComponentStageRotation>()) {
		if (facing.x || facing.y) sr.facing = facing;
	}

	p << sr.facing.x << sr.facing.y;

	if (!withinDelta) {
		std::cout << "Actioned outside of delta" << std::endl;
		// Only include delta in update if actioned within the delta
		p << 0 << 0;
		p << false;
		tempo::sendMessage(tempo::QueueID::MOVEMENT_INTENT_UPDATES, p);
		return;
	}

	// Include delta and tell server of intent
	p << delta.x << delta.y;
	p << true;
	tempo::sendMessage(tempo::QueueID::MOVEMENT_INTENT_UPDATES, p);

	if (entity.hasComponent<tempo::ComponentStageTranslation>()) {
		entity.getComponent<tempo::ComponentStageTranslation>().delta = delta;
		entity.getComponent<tempo::ComponentStageTranslation>().moved = true;
	}
}

void addAttack(anax::Entity &entity, bool withinDelta)
{
	if (!withinDelta) {
		std::cout << "Actioned outside of delta" << std::endl;
		return;
	}

	if (entity.hasComponent<tempo::ComponentAttack>()
	    && entity.hasComponent<tempo::ComponentWeapon>()) {
		tempo::ComponentAttack &a = entity.getComponent<tempo::ComponentAttack>();
		tempo::ComponentWeapon &w = entity.getComponent<tempo::ComponentWeapon>();
		a.damage                  = w.damage;
		a.beats_until_attack      = w.beats_until_attack;

		sf::Packet p;
		p << static_cast<uint32_t>(tempo::MessageAttack::UPDATE_INTENT);
		p << tempo::localtoserver[entity.getId()];
		p << a.damage;
		p << a.beats_until_attack;
		tempo::sendMessage(tempo::QueueID::SYSTEM_ATTACK, p);
	}
}

void addHeal(anax::Entity &entity, bool withinDelta)
{

	if (entity.hasComponent<tempo::ComponentHealth>()
	    && entity.hasComponent<tempo::ComponentCombo>()) {

		tempo::ComponentHealth &h = entity.getComponent<tempo::ComponentHealth>();
		tempo::ComponentCombo  &c = entity.getComponent<tempo::ComponentCombo>();

		if(c.comboCounter > 3) { // 3 for testing purpose. It should be 10
			c.comboCounter -= 3;
			h.HealthUpdate(2);

			sf::Packet p;
			p << tempo::localtoserver[entity.getId()];
			p << h.current_health;
			sendMessage(tempo::QueueID::SYSTEM_HEALTH, p);

		}
	}
}

void updateCombo(anax::Entity &entity, bool withinDelta)
{
	if (entity.hasComponent<tempo::ComponentCombo>()) {
		tempo::ComponentCombo &c = entity.getComponent<tempo::ComponentCombo>();
		if (withinDelta) {
			// c.performAction();

			sf::Packet p;
			p << tempo::localtoserver[entity.getId()];
			p << static_cast<uint8_t>(tempo::MessageCombo::INCREMENT_COMBO);
			tempo::sendMessage(tempo::QueueID::COMBO_UPDATES, p);
		} else {
			// c.breakCombo();

			sf::Packet p;
			p << tempo::localtoserver[entity.getId()];
			p << static_cast<uint8_t>(tempo::MessageCombo::BROKEN_COMBO);
			tempo::sendMessage(tempo::QueueID::COMBO_UPDATES, p);
		}
	}
}

void processKeyPressEvent(irr::EKEY_CODE key, anax::Entity &entity, bool withinDelta, irr::IrrlichtDevice* device )
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
		//dance
		addMovement(entity, glm::vec2(0,0), glm::vec2(0,0), withinDelta);
		updateCombo(entity, withinDelta);
		break;
	case irr::KEY_SPACE:
		addAttack(entity, withinDelta);
		updateCombo(entity, withinDelta);
		break;
	case irr::KEY_KEY_1:
		addHeal(entity, withinDelta);
		break;
	case irr::KEY_ESCAPE:
		device->closeDevice();
		break;
	default: break;
	}
}

void SystemParseKeyInput::parseInput(tempo::Clock &clock, irr::IrrlichtDevice* device)
{
	for (auto entity : getEntities()) {
		ComponentKeyInput ke = entity.getComponent<ComponentKeyInput>();

		bool withinDelta = false;
		if (clock.within_delta())
			withinDelta = true;

		for (unsigned int i = 0; i < ke.keysPressed.size(); i++) {
			if (ke.keysPressed[i].press) {
				processKeyPressEvent(ke.keysPressed[i].key, entity, withinDelta, device);
			}
		}
	}
}

}  // namespace client
