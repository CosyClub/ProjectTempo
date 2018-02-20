#include <client/system/SystemUpdateKeyInput.hpp>
#include <client/component/ComponentKeyInput.hpp>

#include <iostream>

namespace client {
	void SystemUpdateKeyInput::setup(irr::IrrlichtDevice* device) {
		device->setEventReceiver(&receiver);
	}

	void SystemUpdateKeyInput::addkey() {
		auto entities = getEntities();

		for (auto entity : entities)
		{
			client::ComponentKeyInput& ki = entity.getComponent<client::ComponentKeyInput>();

			if(receiver.IsKeyDown(irr::KEY_KEY_W)) {
			    ki.keysPressed.push_back('w');
			}
			else if(receiver.IsKeyDown(irr::KEY_KEY_S))
			    ki.keysPressed.push_back('s');

			if(receiver.IsKeyDown(irr::KEY_KEY_A)) {
			    ki.keysPressed.push_back('a');
			}
			else if(receiver.IsKeyDown(irr::KEY_KEY_D)) {
			    ki.keysPressed.push_back('d');
			}
		}
	}
	void SystemUpdateKeyInput::clear() {
		auto entities = getEntities();

		for (auto entity : entities)
		{
			client::ComponentKeyInput& ki = entity.getComponent<client::ComponentKeyInput>();

			ki.keysPressed.clear();
		}
	}
} // namespace client
