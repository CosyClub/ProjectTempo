#ifndef CLIENT_COMPONENT_KEY_INPUT_HPP
#define CLIENT_COMPONENT_KEY_INPUT_HPP

#include <anax/Component.hpp>

namespace client{
	// Entities with this component will perform actions on keyboard inputs
	struct ComponentKeyInput : anax::Component {
		std::vector<char> keysPressed;

	};
}

#endif
