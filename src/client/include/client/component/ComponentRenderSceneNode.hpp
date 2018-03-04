#ifndef CLIENT_COMPONENT_RENDER_SCENE_NODE_HPP
#define CLIENT_COMPONENT_RENDER_SCENE_NODE_HPP

#include <anax/Component.hpp>

#include <ISceneNode.h>

namespace client
{
struct ComponentRenderSceneNode : public anax::Component {
	irr::scene::ISceneNode *node;
	ComponentRenderSceneNode(irr::scene::ISceneNode *node);
};

}  // namespace client

#endif
