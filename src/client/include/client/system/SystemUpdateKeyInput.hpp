#ifndef CLIENT_SYSTEM_UPDATE_KEY_INPUT_HPP
#define CLIENT_SYSTEM_UPDATE_KEY_INPUT_HPP

#include <client/component/ComponentKeyInput.hpp>

#include <anax/System.hpp>
#include <anax/World.hpp>

#include <irrlicht.h>

#include <mutex>

enum keyStatesENUM {UP, DOWN, PRESSED, RELEASED};

class KeyInput : public irr::IEventReceiver
{

	keyStatesENUM keyState[irr::KEY_KEY_CODES_COUNT];
	private:
		// A list of characters that have been recieved
		std::vector<client::KeyEvent> chars;
		std::mutex chars_mutex;

	public:
		// // This is the one method that we have to implement
		// virtual bool OnEvent(const irr::SEvent& event) {
		// 	// Remember whether each key is down or up
		// 	if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
		// 		if (event.KeyInput.Char != 0) {
		// 			std::lock_guard<std::mutex> lock(chars_mutex);
		//
		// 			chars.push_back(client::KeyEvent(event.KeyInput.Char, !event.KeyInput.PressedDown ));
		// 		}
		// 	}
		//
		// 	return false;
		// }

		virtual bool OnEvent(const irr::SEvent & event) {

		if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
				// if key is Pressed Down
				if (event.KeyInput.PressedDown == true) {
					// If key was not down before
					if (keyState[event.KeyInput.Key] != DOWN && keyState[event.KeyInput.Key] != PRESSED) {
						keyState[event.KeyInput.Key] = PRESSED; // Set to Pressed
						std::lock_guard<std::mutex> lock(chars_mutex);
						chars.push_back(client::KeyEvent(event.KeyInput.Char, true ));
					}
					else {
						// if key was down before
						keyState[event.KeyInput.Key] = DOWN; // Set to Down
					}
				}
				else {
						// if the key is down
						if (keyState[event.KeyInput.Key] != UP) {
							keyState[event.KeyInput.Key] = RELEASED; // Set to Released
							std::lock_guard<std::mutex> lock(chars_mutex);
							chars.push_back(client::KeyEvent(event.KeyInput.Char, false ));
						}
				}
		}
	}

		void init() {
			for (int i = 0; i <= irr::KEY_KEY_CODES_COUNT; i++)
			{
				keyState[i] = UP;
			}
		}


		std::vector<client::KeyEvent> getChars() {
			std::lock_guard<std::mutex> lock(chars_mutex);
			return chars;
		}

		void clearChars() {
			std::lock_guard<std::mutex> lock(chars_mutex);
			chars.clear();
		}
};

namespace client {

	class SystemUpdateKeyInput : public anax::System<anax::Requires<client::ComponentKeyInput>>
	{
	private:
		KeyInput receiver;
	public:
		void setup(irr::IrrlichtDevice* device);
		// clears and updates the keys in each component
		void addKeys();
		// possibly not needed
		void clear();
	};
} // namespace client

#endif
