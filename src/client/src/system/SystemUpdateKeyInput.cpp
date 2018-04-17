#include <client/component/ComponentKeyInput.hpp>
#include <client/system/SystemUpdateKeyInput.hpp>

#include <iostream>


bool KeyInput::OnEvent(const irr::SEvent &event)
{
	if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
		// if key is Pressed Down
		if (event.KeyInput.PressedDown == true) {
			// If key was not down before
			if (keyState[event.KeyInput.Key] != DOWN && keyState[event.KeyInput.Key] != PRESSED) {
				keyState[event.KeyInput.Key] = PRESSED;  // Set to Pressed
				std::lock_guard<std::mutex> lock(chars_mutex);
				chars.push_back(client::KeyEvent(event.KeyInput.Key, true));
			} else {
				// if key was down before
				keyState[event.KeyInput.Key] = DOWN;  // Set to Down
			}
		} else {
			// if the key is down
			if (keyState[event.KeyInput.Key] != UP) {
				keyState[event.KeyInput.Key] = RELEASED;  // Set to Released
				std::lock_guard<std::mutex> lock(chars_mutex);
				chars.push_back(client::KeyEvent(event.KeyInput.Key, false));
			}
		}
	}
	return false;
}

void KeyInput::init()
{
	for (int i = 0; i <= irr::KEY_KEY_CODES_COUNT; i++) {
		keyState[i] = UP;
	}
}


std::vector<client::KeyEvent> KeyInput::getChars()
{
	std::lock_guard<std::mutex> lock(chars_mutex);
	return chars;
}

void KeyInput::clearChars()
{
	std::lock_guard<std::mutex> lock(chars_mutex);
	chars.clear();
}


namespace client
{
void SystemUpdateKeyInput::setup(irr::IrrlichtDevice *device)
{
	device->setEventReceiver(&receiver);
	// receiver.init();
}

void SystemUpdateKeyInput::addKeys()
{
	auto entities = getEntities();

	for (auto entity : entities) {
		client::ComponentKeyInput &ki = entity.getComponent<client::ComponentKeyInput>();

		// add all keys that were queued up
		std::vector<client::KeyEvent> keys = receiver.getChars();
		ki.keysPressed.insert(ki.keysPressed.end(), keys.begin(), keys.end());
	}

	// clear the keys in the receiver
	receiver.clearChars();
}

std::vector<client::KeyEvent> SystemUpdateKeyInput::getKeys()
{
	std::vector<client::KeyEvent> keys_temp = receiver.getChars();
	// clear the keys in the receiver
	receiver.clearChars();

	// std::cout << "reciever.size(): " << keys_temp.size() << std::endl;

	return keys_temp;
}

void SystemUpdateKeyInput::clear()
{
	auto entities = getEntities();

	for (auto entity : entities) {
		client::ComponentKeyInput &ki = entity.getComponent<client::ComponentKeyInput>();

		ki.keysPressed.clear();
	}
}
}  // namespace client
