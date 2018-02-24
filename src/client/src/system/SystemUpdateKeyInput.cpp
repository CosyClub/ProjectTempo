#include <client/system/SystemUpdateKeyInput.hpp>
#include <client/component/ComponentKeyInput.hpp>

#include <iostream>

namespace client {
	void SystemUpdateKeyInput::setup(irr::IrrlichtDevice* device) {
		device->setEventReceiver(&receiver);
		receiver.init();
	}

	void SystemUpdateKeyInput::addKeys() {
		auto entities = getEntities();

		for (auto entity : entities)
		{
			client::ComponentKeyInput& ki = entity.getComponent<client::ComponentKeyInput>();

			// add all keys that were queued up
			std::vector<client::KeyEvent> keys = receiver.getChars();
			ki.keysPressed.insert(ki.keysPressed.end(),
			                      keys.begin(), keys.end());
		}

		// clear the keys in the reciever
		receiver.clearChars();
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
