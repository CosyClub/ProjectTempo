#ifndef CLIENT_COMPONENT_KEY_INPUT_HPP
#define CLIENT_COMPONENT_KEY_INPUT_HPP

#include <anax/Component.hpp>

namespace client
{
	struct KeyEvent {
		char key;   // The key that was pressed
		bool press;  // true iff. key press; false iff. key release.
	};

	// Entities with this component will perform actions on keyboard inputs
	struct ComponentKeyInput : anax::Component {
		std::vector<KeyEvent> keysPressed;
	};
}

#endif
