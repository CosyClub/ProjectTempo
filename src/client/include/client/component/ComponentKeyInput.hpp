#ifndef CLIENT_COMPONENT_KEY_INPUT_HPP
#define CLIENT_COMPONENT_KEY_INPUT_HPP

#include <Keycodes.h>
#include <anax/Component.hpp>
#include <deque>

namespace client
{
struct KeyEvent {
	irr::EKEY_CODE key;    // The key that was pressed
	bool           press;  // true iff. key press; false iff. key release.

	KeyEvent(irr::EKEY_CODE k, bool p)
	    : key(k)
	    , press(p)
	{
	}
};

// Entities with this component will perform actions on keyboard inputs
struct ComponentKeyInput : anax::Component {
	/// \brief The maximum age of actions to keep in the actions deque
	/// counted in beats
	static const constexpr int ACTION_AGE_LIMIT = 10;

	/// \brief Queue of keys to be processed by the system
	std::vector<client::KeyEvent> keysPressed;

	/// \brief Represents a historic keypress for this ComponentKeyInput
	struct Action {
		Action(unsigned int beat, float delta);

		/// \brief The beat upon which the action was made
		unsigned int beat;

		/// \brief The delta in seconds between the keypress and the beat
		/// Will be negative if the keypress was before the beat, and positive
		/// otherwise
		float        delta;
	};

	/// \brief Historic actions made by this component key input
	std::deque<Action> actions;

	ComponentKeyInput();
};
}

#endif
