#ifndef CLIENT_SYSTEM_UPDATE_KEY_INPUT_HPP
#define CLIENT_SYSTEM_UPDATE_KEY_INPUT_HPP
#include <client/component/ComponentKeyInput.hpp>
#include <iostream>

#include <anax/System.hpp>
#include <anax/World.hpp>

#include <irrlicht.h>

#include <mutex>

enum keyStatesENUM { UP, DOWN, PRESSED, RELEASED };

class KeyInput : public irr::IEventReceiver
{
	keyStatesENUM keyState[irr::KEY_KEY_CODES_COUNT];

   private:
	// A list of characters that have been received
	std::mutex chars_mutex;

   public:
	std::vector<client::KeyEvent> chars;

	virtual bool OnEvent(const irr::SEvent &event);

	void init();


	std::vector<client::KeyEvent> getChars();

	void clearChars();
};

namespace client
{
class SystemUpdateKeyInput : public anax::System<anax::Requires<client::ComponentKeyInput>>
{
   private:
	KeyInput receiver;

   public:
	void setup(irr::IrrlichtDevice *device);
	// clears and updates the keys in each component
	void addKeys();

	std::vector<client::KeyEvent> getKeys();
	// possibly not needed
	void clear();
};
}  // namespace client

#endif
