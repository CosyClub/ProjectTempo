#ifndef CLIENT_SYSTEM_UPDATE_KEY_INPUT_HPP
#define CLIENT_SYSTEM_UPDATE_KEY_INPUT_HPP

#include <client/component/ComponentKeyInput.hpp>

#include <anax/System.hpp>
#include <anax/World.hpp>

#include <irrlicht.h>

#include <mutex>

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
			if (event.KeyInput.PressedDown && event.EventType == irr::EET_KEY_INPUT_EVENT) {
				if (event.KeyInput.Char != 0) {
					std::lock_guard<std::mutex> lock(chars_mutex);
					chars.push_back(event.KeyInput.Char);
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
