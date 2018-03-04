#ifndef CLIENT_COMPONENT_HEALTH_BAR_HPP
#define CLIENT_COMPONENT_HEALTH_BAR_HPP

#include <anax/Component.hpp>

#include <ISceneNode.h>
#include <IBillboardSceneNode.h>

namespace client {
	struct ComponentHealthBar : public anax::Component {
		irr::scene::IBillboardSceneNode* node;
		ComponentHealthBar(irr::scene::IBillboardSceneNode* node);
	};

} // namespace client

#endif
