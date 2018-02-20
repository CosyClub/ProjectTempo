#ifndef CLIENT_COMPONENT_KEY_INPUT_HPP
#define CLIENT_COMPONENT_KEY_INPUT_HPP

#include <anax/Component.hpp>

namespace client{

struct ComponentKeyInput : anax::Component {

	std::vector<char> keysPressed;

	ComponentKeyInput();

};

}

#endif
