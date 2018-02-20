#ifndef CLIENT_SYSTEM_UPDATE_KEY_INPUT_HPP
#define CLIENT_SYSTEM_UPDATE_KEY_INPUT_HPP

#include <irrlicht.h>
#include <anax/System.hpp>
#include <client/component/ComponentKeyInput.hpp>

class KeyInput : public irr::IEventReceiver
{
private:
	// We use this array to store the current state of each key
	bool KeyIsDown[irr::KEY_KEY_CODES_COUNT];

public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const irr::SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		return false;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(irr::EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}

	KeyInput()
	{
		for (irr::u32 i=0; i<irr::KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}
};

namespace client {

	// Initalises all rendering for the static stage
	class SystemUpdateKeyInput : public anax::System<anax::Requires<client::ComponentKeyInput>> {
		KeyInput receiver;
	public:
		void setup(irr::IrrlichtDevice* device);
		void addkey();
		void clear();
	};
} // namespace client

#endif
