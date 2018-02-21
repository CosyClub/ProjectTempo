#ifndef CLIENT_SYSTEM_UPDATE_KEY_INPUT_HPP
#define CLIENT_SYSTEM_UPDATE_KEY_INPUT_HPP

#include <client/component/ComponentKeyInput.hpp>

#include <anax/System.hpp>
#include <anax/World.hpp>

#include <irrlicht.h>

#include <mutex>

namespace {
	// Not sure how to do this any better...
	char irr_to_char(irr::EKEY_CODE code) {
		switch (code) {
		case irr::KEY_KEY_0:
			return '0';
		case irr::KEY_KEY_1:
			return '1';
		case irr::KEY_KEY_2:
			return '2';
		case irr::KEY_KEY_3:
			return '3';
		case irr::KEY_KEY_4:
			return '4';
		case irr::KEY_KEY_5:
			return '5';
		case irr::KEY_KEY_6:
			return '6';
		case irr::KEY_KEY_7:
			return '7';
		case irr::KEY_KEY_8:
			return '8';
		case irr::KEY_KEY_9:
			return '9';
		case irr::KEY_KEY_A:
			return 'a';
		case irr::KEY_KEY_B:
			return 'b';
		case irr::KEY_KEY_C:
			return 'c';
		case irr::KEY_KEY_D:
			return 'd';
		case irr::KEY_KEY_E:
			return 'e';
		case irr::KEY_KEY_F:
			return 'f';
		case irr::KEY_KEY_G:
			return 'g';
		case irr::KEY_KEY_H:
			return 'h';
		case irr::KEY_KEY_I:
			return 'i';
		case irr::KEY_KEY_J:
			return 'j';
		case irr::KEY_KEY_K:
			return 'k';
		case irr::KEY_KEY_L:
			return 'l';
		case irr::KEY_KEY_M:
			return 'm';
		case irr::KEY_KEY_N:
			return 'n';
		case irr::KEY_KEY_O:
			return 'o';
		case irr::KEY_KEY_P:
			return 'p';
		case irr::KEY_KEY_Q:
			return 'q';
		case irr::KEY_KEY_R:
			return 'r';
		case irr::KEY_KEY_S:
			return 's';
		case irr::KEY_KEY_T:
			return 't';
		case irr::KEY_KEY_U:
			return 'u';
		case irr::KEY_KEY_V:
			return 'v';
		case irr::KEY_KEY_W:
			return 'w';
		case irr::KEY_KEY_X:
			return 'x';
		case irr::KEY_KEY_Y:
			return 'y';
		case irr::KEY_KEY_Z:
			return 'z';
		}
		return 0;
	}

	class KeyInput : public irr::IEventReceiver
	{
	private:
		// A list of characters that have been recieved
		std::vector<char> chars;
		std::mutex chars_mutex;

	public:
		// This is the one method that we have to implement
		virtual bool OnEvent(const irr::SEvent& event)
		{
			// Remember whether each key is down or up
			if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
				char k = irr_to_char(event.KeyInput.Key);
				if (k != 0) {
					std::lock_guard<std::mutex> lock(chars_mutex);
					chars.push_back(k);
				}
			}

			return false;
		}

		std::vector<char> getChars() {
			std::lock_guard<std::mutex> lock(chars_mutex);
			return chars;
		}

		void clearChars() {
			std::lock_guard<std::mutex> lock(chars_mutex);
			chars.clear();
		}
	};
}

namespace client {

	class SystemUpdateKeyInput : public anax::System<anax::Requires<client::ComponentKeyInput>> {
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
