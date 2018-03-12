#ifndef CLIENT_COMPONENT_RENDER_BUTTON_GROUP_HPP
#define CLIENT_COMPONENT_RENDER_BUTTON_GROUP_HPP

#include <IMeshSceneNode.h>
#include <anax/Component.hpp>

#include <deque>

struct buttonRender {
	irr::scene::IMeshSceneNode *button;
	irr::scene::IMeshSceneNode *button_housing;
};

namespace client
{
struct ComponentRenderButtonGroup : anax::Component {
	// This is a deque so that it is possible to push_front
	std::deque<buttonRender> buttonsRender;
	ComponentRenderButtonGroup();
	~ComponentRenderButtonGroup();
};
}

#endif
