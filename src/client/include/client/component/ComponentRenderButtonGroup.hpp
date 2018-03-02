#ifndef CLIENT_COMPONENT_RENDER_BUTTON_GROUP_HPP
#define CLIENT_COMPONENT_RENDER_BUTTON_GROUP_HPP

#include <anax/Component.hpp>
#include <IMeshSceneNode.h>

#include <deque>

struct buttonRender {
	irr::scene::IMeshSceneNode* button;
	irr::scene::IMeshSceneNode* button_housing;
};

namespace client{

	struct ComponentRenderButtonGroup : anax::Component {
		std::deque<buttonRender> buttonsRender;
		
		ComponentRenderButtonGroup();
	};

}

#endif
